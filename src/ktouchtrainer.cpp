/***************************************************************************
 *   ktouchtrainer.cpp                                                     *
 *   -----------------                                                     *
 *   Copyright (C) 2000 by H�ard Friland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchtrainer.h"
#include "ktouchtrainer.moc"

#include <qlcdnumber.h>
#include <qfile.h>

#include <kdebug.h>
#include <kpushbutton.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kaudioplayer.h>

#include "ktouchstatus.h"
#include "ktouchslideline.h"
#include "ktouchkeyboard.h"
#include "ktouchlecture.h"
#include "ktouchsettings.h"

const int UPDATE_INTERVAL = 500;    // milli seconds between updates of the speed LCD

KTouchTrainer::KTouchTrainer(KTouchStatus *status, KTouchSlideLine *slideLine, KTouchKeyboard *keyboard, KTouchLecture *lecture)
  : QObject(),
    m_trainingTimer(new QTimer),
    m_statusWidget(status),
    m_slideLineWidget(slideLine),
    m_keyboardWidget(keyboard),
    m_lecture(lecture)
{
    m_level = 0;              // level 1, but we're storing it zero based
    m_line  = 0;
    m_trainingPaused=false;
    m_waiting=true;
    m_teacherText=m_lecture->level(0).line(0);
    m_studentText="";
    m_session.reset();

    m_levelUpSound = KGlobal::dirs()->findResource("appdata","up.wav");
    m_levelDownSound = KGlobal::dirs()->findResource("appdata","down.wav");
    m_typeWriterSound = KGlobal::dirs()->findResource("appdata","typewriter.wav");

    connect(m_statusWidget->levelUpBtn, SIGNAL(clicked()), this, SLOT(levelUp()) );
    connect(m_statusWidget->levelDownBtn, SIGNAL(clicked()), this, SLOT(levelDown()) );
    connect(m_trainingTimer, SIGNAL(timeout()), this, SLOT(timerTick()) );
}

KTouchTrainer::~KTouchTrainer() {
    delete m_trainingTimer;
}

void KTouchTrainer::goFirstLine() {
    m_statusWidget->setNewChars( m_lecture->level(m_level).newChars() );
    m_line=0;
    m_incLinesCount=0;
    m_decLinesCount=0;
    newLine();
}

void KTouchTrainer::keyPressed(QChar key) {
    if (!typingAllowed())  return;
    if (m_teacherText==m_studentText) {
        // if already at end of line, don't add more chars
        // TODO : Flash the line
        QApplication::beep();
        return;
    };
    m_studentText+=key;
    // we need to find out, if the key was correct or not
    unsigned int len = m_studentText.length();
    if (m_teacherText.left(len)==m_studentText && m_teacherText.length()>=len) {
        // ok, all student text is correct
        m_session.addCorrectChar(key);
    }
    else {
        // nope, the key was wrong : beep !!!
        if (KTouchConfig().m_errorBeep)   QApplication::beep();
        // now find the key the user missed:
        if (m_teacherText.left(len-1)==m_studentText.left(len-1) && m_teacherText.length()>=len)
            m_session.addWrongChar(m_teacherText[len-1]);
        else
            m_session.addWrongChar(8);
    };
    updateWidgets(); // update all the other widgets (keyboard widget, status widget and statusbar
}

void KTouchTrainer::backspacePressed() {
    if (!typingAllowed())  return;
    unsigned int len = m_studentText.length();
    if (len) {
        if (m_teacherText.left(len)==m_studentText && m_teacherText.length()>=len) {
            // we are removing a correctly typed char
            --m_session.m_correctChars;
        };
        m_studentText = m_studentText.left(--len);

        updateWidgets(); // update all the widgets
        /*
        m_statusWidget->updateStatus(m_level, m_session.correctness());
        if (m_teacherText.left(len)==m_studentText)
            m_keyboardWidget->newKey(m_teacherText[len]);
        else
            m_keyboardWidget->newKey(QChar(8));
        */
    }
    else {
        // TODO: Flash line
        QApplication::beep();
    };
    emit statusbarStatsChanged(m_session.m_correctChars, m_session.m_totalChars, m_session.m_words);
}

void KTouchTrainer::enterPressed() {
    if (!typingAllowed())  return;
    if (m_studentText!=m_teacherText) {
        QApplication::beep();
        return;
    };

    if (KTouchConfig().m_autoLevelChange) {
        // if level increase criterion was fulfilled, increase line counter
        if (KTouchConfig().m_upCorrectLimit <= m_session.correctness()*100 &&
            KTouchConfig().m_upSpeedLimit <= m_session.charSpeed())
        {
            m_decLinesCount=0;
            ++m_incLinesCount;
        }
        else  if (KTouchConfig().m_downCorrectLimit > m_session.correctness()*100 ||
                  KTouchConfig().m_downSpeedLimit > m_session.charSpeed())
        {
            m_incLinesCount=0;
            ++m_decLinesCount;
        };
        // Automatic level change after a number of lines can happen, if you fulfilled the
        // requirements in the last 5 lines.
        if (m_incLinesCount>=2) {
            levelUp();
            return;
        }
        if (m_decLinesCount>=2 && m_level!=0) {
            levelDown();
            return;
        };
    };
    // Now let's increase the line
    ++m_line;
    if (m_line >= m_lecture->level(m_level).lineCount()) {
        if (KTouchConfig().m_autoLevelChange) {
            // adjust level if limits exceeded
            if (KTouchConfig().m_upCorrectLimit<=m_session.correctness()*100 &&
                KTouchConfig().m_upSpeedLimit<=m_session.charSpeed())
            {
                levelUp();
                return;
            }
            else  if (KTouchConfig().m_downCorrectLimit>m_session.correctness()*100 ||
                    KTouchConfig().m_downSpeedLimit>m_session.charSpeed())
            {
                levelDown();
                return;
            }
        };
        goFirstLine();  // otherwise restart level
    }
    else
        newLine();
}

void KTouchTrainer::updateWidgets() {
    // update status widget
    m_statusWidget->updateStatus(m_level, m_session.correctness());
    // update slide line widget
    m_slideLineWidget->setStudentText(m_studentText);
    // update keyboard widget -> show next to be pressed char.
    // That means we have to find out, whether the student text is correct or not!
    unsigned int len = m_studentText.length();
    if (m_teacherText.left(len)==m_studentText && m_teacherText.length()>=len) {
        // ok, all student text is correct
        if (m_teacherText.length()==m_studentText.length())
            m_keyboardWidget->newKey(QChar(13));        // we have reached the end of the line
        else
            m_keyboardWidget->newKey(m_teacherText[len]);
    }
    else {
        m_keyboardWidget->newKey(QChar(8)); // wrong key, user must now press backspace
    };
    emit statusbarStatsChanged(m_session.m_correctChars, m_session.m_totalChars, m_session.m_words);
}

void KTouchTrainer::readSessionHistory() {
    QFile historyFile(KGlobal::dirs()->saveLocation("appdata")+"sessions.txt");
    historyFile.open( IO_ReadOnly );
    QTextStream in(&historyFile);
    QString line = in.readLine();
    while (!line.isEmpty() && !line.isNull()) {
        if (line[0]!='#') // don't try to read comments
            m_sessionHistory.append( KTouchTrainingSession(line) );
        line = in.readLine();
    };
}

void KTouchTrainer::writeSessionHistory() {
    QFile historyFile(KGlobal::dirs()->saveLocation("appdata")+"sessions.txt");
    historyFile.open( IO_WriteOnly );
    QTextStream out(&historyFile);
    out << "# KTouch Training Session History" << endl;
    out << "# Each line is one session, entries: "<< endl;
    out << "#   elapsed time (milliseconds)" << endl;
    out << "#   total characters typed" << endl;
    out << "#   correct characters typed" << endl;
    out << "#   words typed" << endl;
    out << "#   character stats, in number triples" << endl;
    out << "#     character unicode number" << endl;
    out << "#     how often the character was typed correctly" << endl;
    out << "#     how often the character was missed" << endl;
    for (QValueList<KTouchTrainingSession>::const_iterator it=m_sessionHistory.begin(); it!=m_sessionHistory.end(); ++it) {
        if ((*it).m_elapsedTime==0) continue;   // don't save empty sessions
        out << (*it).asString() << endl;
    };
}

void KTouchTrainer::levelUp() {
    KAudioPlayer::play(m_levelUpSound);
    m_incLinesCount=m_decLinesCount=0;
    ++m_level;  // increase the level
    if (m_level>=m_lecture->levelCount()) {
        // already at max level? Let's stay there
        m_level=m_lecture->levelCount()-1;
    };
    goFirstLine();
}

void KTouchTrainer::levelDown() {
    KAudioPlayer::play(m_levelUpSound);
    m_incLinesCount=m_decLinesCount=0;
    if (m_level>0) {
       --m_level;
    }
    goFirstLine();
}

void KTouchTrainer::startNewTrainingSession(bool keepLevel) {
    // store the old training session in the history (but only if the time was running)
    if (m_session.m_elapsedTime>0)
        m_sessionHistory.push_back( m_session );
    m_session.reset();  // reset session
    if (!keepLevel)
        m_level=0;
    goFirstLine();
    emit statusbarMessageChanged(i18n("Starting training session: Waiting for first keypress...") );
    emit statusbarStatsChanged(0, 0, 0);
    m_trainingPaused=false;
    m_waiting=true;
    m_trainingTimer->stop();    // Training timer will be started on first keypress.
}

void KTouchTrainer::pauseTraining() {
    m_trainingTimer->stop();
    m_trainingPaused=true;
    m_slideLineWidget->setCursorTimerEnabled(false);
    emit statusbarMessageChanged(i18n("Training session paused.") );
}

void KTouchTrainer::continueTraining() {
    m_trainingPaused=false;
    m_waiting=true;
    m_slideLineWidget->setCursorTimerEnabled(true);
    emit statusbarMessageChanged(i18n("Training session continues on next keypress...") );
    emit statusbarStatsChanged(m_session.m_correctChars, m_session.m_totalChars, m_session.m_words);
    m_statusWidget->updateStatus(m_level, m_session.correctness());
    m_statusWidget->speedLCD->display( m_session.charSpeed() );
}

void KTouchTrainer::timerTick() {
    if (m_trainingPaused) return;
    // Add the timer interval. I think we can neglect the error we make when the session is
    // paused and continued... it's not a scientific calculation, isn't it?
    m_session.m_elapsedTime+=UPDATE_INTERVAL;
    m_statusWidget->speedLCD->display( m_session.charSpeed() );
}

void KTouchTrainer::newLine() {
    m_teacherText = m_lecture->level(m_level).line(m_line);
    m_studentText="";
    m_statusWidget->updateStatus(m_level, m_session.correctness());
    m_keyboardWidget->newKey(m_teacherText[0]);
    m_slideLineWidget->setNewText(m_teacherText, m_studentText);
}

bool KTouchTrainer::typingAllowed() {
    if (m_trainingPaused) {
        emit statusbarMessageChanged(i18n("Training is halted! Restart (continue) session first...") );
        return false;
    };
    if (m_waiting) {
        // we were waiting for a keypress, now we are actually starting the session
        m_waiting=false;                            // we are no longer waiting
        m_trainingTimer->start(UPDATE_INTERVAL);    // start the timer
        emit statusbarMessageChanged(i18n("Training session! The time is running...") );
    };
    return true;
}
