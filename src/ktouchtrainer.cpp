/***************************************************************************
 *   ktouchtrainer.cpp                                                     *
 *   -----------------                                                     *
 *   Copyright (C) 2000 by Håvard Frøiland, 2006 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchtrainer.h"
#include "ktouchtrainer.moc"

#include <QTimer>

#include <KMessageBox>
#include <kdebug.h>
#include <kpushbutton.h>
#include <klocale.h>
#include <kstandarddirs.h>

#include "ktouch.h"
#include "ktouchstatuswidget.h"
#include "ktouchtextlinewidget.h"
#include "ktouchkeyboardwidget.h"
#include "ktouchlecture.h"
#include "ktouchdefaults.h"
#include "ktouchlevelsummarydialog.h"
#include "prefs.h"


KTouchTrainer::KTouchTrainer(KTouchStatusWidget *status, KTouchTextLineWidget *textLine, KTouchKeyboardWidget *keyboard, KTouchLecture *lecture)
  : QObject(),
    m_trainingTimer(new QTimer),
    m_statusWidget(status),
    m_textLineWidget(textLine),
    m_keyboardWidget(keyboard),
    m_lecture(lecture)
{
    m_level = 0;              // level 1, but we're storing it zero based
    m_line  = 0;
    m_wordsInCurrentLine = 0;
    m_trainingPaused=true;    // we start in pause mode
    m_teacherText=m_lecture->level(0).line(0);
    m_studentText="";

    m_decLinesCount=0;
    m_incLinesCount=0;

    m_player = Phonon::createPlayer( Phonon::GameCategory );
    m_player->setParent(this);

    // reset statistics
    m_levelStats.clear();
    m_sessionStats.clear();

    /// \todo preload sounds and improve sound playback system
    m_levelUpSound = KGlobal::dirs()->findResource("appdata","up.wav");
    m_levelDownSound = KGlobal::dirs()->findResource("appdata","down.wav");
    m_typeWriterSound = KGlobal::dirs()->findResource("appdata","typewriter.wav");

    connect(m_statusWidget->levelUpBtn, SIGNAL(clicked()), this, SLOT(levelUp()) );
    connect(m_statusWidget->levelDownBtn, SIGNAL(clicked()), this, SLOT(levelDown()) );
    connect(m_trainingTimer, SIGNAL(timeout()), this, SLOT(timerTick()) );
}
// ----------------------------------------------------------------------------

KTouchTrainer::~KTouchTrainer() {
    delete m_trainingTimer;
}
// ----------------------------------------------------------------------------

void KTouchTrainer::gotoFirstLine() {
    m_statusWidget->setNewChars( m_lecture->level(m_level).newChars() );
    m_line=0;
    newLine();
	// finally tell the keyboard widget, that we have a new set of new chars
	m_keyboardWidget->setKnownChars( m_lecture->knownCharsInLevel(m_level) );
}
// ----------------------------------------------------------------------------

void KTouchTrainer::keyPressed(QChar key) {
	// NOTE : In this function we need to distinguish between left and right
	//        typing. Use the config setting Prefs::right2LeftTyping() for that.
    if(Prefs::soundOnKeypress()){
        m_player->setCurrentSource(m_typeWriterSound.url());
        m_player->play();
    }
	if (m_trainingPaused)  continueTraining();
    if (m_teacherText==m_studentText) {
        // if already at end of line, don't add more chars
        /// \todo Flash the line when line complete
        if (Prefs::beepOnError())   QApplication::beep();
        return;
    }
	// remember length of student text without added character
    int old_student_text_len = m_studentText.length();
    // compose new student text depending in typing direction
    QString new_student_text = m_studentText;
    new_student_text += key;

	// don´t allow excessive amounts of characters per line
	if (new_student_text.length() > m_teacherText.length()) {
        if (Prefs::beepOnError())
            QApplication::beep();
        return;
	}
    // store the new student text
    m_studentText = new_student_text;
    // we need to find out, if the key was correct or not
    if (studentLineCorrect())
		statsAddCorrectChar(key);	// ok, all student text is correct
    else {
        // nope, the key was wrong : beep !!!
        if (Prefs::beepOnError())  QApplication::beep();
        // check if the key is the first wrong key that was mistyped. Only then add it
        // to the wrong char statistics.
        if (m_teacherText.left(old_student_text_len)==m_studentText.left(old_student_text_len) &&
             m_teacherText.length() > old_student_text_len)
        {
        // add the key the student ought to press to the wrong character stats
        int next_key_index = old_student_text_len;
        statsAddWrongChar( m_teacherText[next_key_index] );
        }
    }
        /// \todo   Implement option whether subsequent mistyped keys should be remembered as missed
        ///         keys as well.
    updateWidgets(); // update all the other widgets (keyboard widget, status widget and statusbar)
	// automatically press enter when option selected
	if (m_studentText == m_teacherText) {
		if (!Prefs::pressEnterOnLineEnd())
			enterPressed();
	}
}
// ----------------------------------------------------------------------------

void KTouchTrainer::backspacePressed() {
    if(Prefs::soundOnKeypress()){
        m_player->setCurrentSource(m_typeWriterSound.url());
        m_player->play();
    }
	if (m_trainingPaused)  continueTraining();
	/// \todo Implement the "remove space character = remove word count" feature

    int len = m_studentText.length();
    if (len) {
        if(Prefs::beepOnError()){
            if (m_teacherText.startsWith(m_studentText)) {
                // we are removing a correctly typed char
                QApplication::beep();
            }
        }
        m_studentText = m_studentText.left(--len);

        updateWidgets(); // update all the widgets and the word count
        if (m_teacherText.startsWith(m_studentText))
            m_keyboardWidget->newKey(m_teacherText[len]);
        else
            m_keyboardWidget->newKey(QChar(8));
    }
    else {
        if(Prefs::beepOnError())
            QApplication::beep();
	}
}
// ----------------------------------------------------------------------------

void KTouchTrainer::enterPressed() {
    if(Prefs::soundOnKeypress()){
        m_player->setCurrentSource(m_typeWriterSound.url());
        m_player->play();
    }

    if (m_trainingPaused)
        continueTraining();

    if (m_studentText != m_teacherText) {
        if (Prefs::beepOnError()){
            QApplication::beep();
        }
        return;
    }

    updateWordCount();
    m_levelStats.m_words += m_wordsInCurrentLine;
    m_sessionStats.m_words += m_wordsInCurrentLine;

	bool increase_level = false;
	bool decrease_level = false;

	// check if level increase criterion was fulfilled
	if (Prefs::upCorrectLimit() <= m_levelStats.correctness()*100 && Prefs::upSpeedLimit() <= m_levelStats.charSpeed()) {
		m_decLinesCount=0;
		++m_incLinesCount;
		increase_level = true;
	}
	// otherwise check if level decrease criterion was fulfilled
	else if (Prefs::downCorrectLimit() > m_levelStats.correctness()*100 || Prefs::downSpeedLimit() > m_levelStats.charSpeed()) {
		decrease_level = true;
		m_incLinesCount=0;
		++m_decLinesCount;
	}

	// Level completed? (are we in the last line of the level?)
    if (m_line+1 >= m_lecture->level(m_level).count()) {
		// ok, lets do all the checks at the end of the level

		// show the level summary dialog with the highscores
//#define LEVELSUMMARYDIALOG_ENABLED
#ifdef LEVELSUMMARYDIALOG_ENABLED
		KTouchLevelSummaryDialog dlg(m_textLineWidget);
		dlg.showInfo(increase_level, decrease_level, m_levelStats);
#endif // LEVELSUMMARYDIALOG_ENABLED

		if (Prefs::autoLevelChange()) {
			if (increase_level) {
				levelUp();
				return;
			}
			else if (decrease_level) {
            	levelDown();
            	return;
			}
		}

		// restart the level
        m_line=0;
    }
    else {
		// we are still in the middle of the level
		// let's check for mid-level advance/fall back
    	if (Prefs::autoLevelChange() && !Prefs::completeWholeTrainingLevel()) {
			// we are allowed to advance early, or go back to the last/lower level

			// first check if we can advance
			if (increase_level && m_incLinesCount >= Prefs::numberOfLinesWorkload()) {
				levelUp();
				return;
			}
			// now check if we need to go back to the last level
        	if (decrease_level && m_decLinesCount >= Prefs::numberOfLinesWorkload() && m_level != 0) {
            	levelDown();
            	return;
        	}
		}

		// no level change, simply go to next line
        ++m_line;
    }

    newLine();
}



// ----------------------------------------------------------------------------

void KTouchTrainer::updateWidgets() {
    // update status widget
    m_statusWidget->updateStatus(m_level, m_levelStats.correctness());
    // update slide line widget
    m_textLineWidget->setStudentText(m_studentText);
    // update keyboard widget -> show next to be pressed char.
    // we have to find out first whether the student text is correct or not.
    if (studentLineCorrect()) {
        // ok, all student text is correct
        if (m_teacherText.length()==m_studentText.length())
            m_keyboardWidget->newKey(QChar(13));        // we have reached the end of the line
        else{
            m_keyboardWidget->newKey(m_teacherText[m_studentText.length()]);
        }
    }
    else {
        m_keyboardWidget->newKey(QChar(8)); // wrong key, user must now press backspace
    }
	updateWordCount();	// here we first update the word count
	updateStatusBar();	// and then the status bar
}
// ----------------------------------------------------------------------------

void KTouchTrainer::startTraining(bool keepLevel) {
	// Here we start a new training session.

	// keep the current level if flag is set
	if (!keepLevel)
		m_level=0;
	// reset the level and session statistics
	m_levelStats.clear();
	m_sessionStats.clear();
	// transfer level to level statistics
	m_levelStats.m_levelNum = m_level;
	// remember level in session stats
	m_sessionStats.m_levelNums.insert(m_level);
	// go to first line in level (also resets word count)
	gotoFirstLine();
	updateStatusBarMessage(i18n("Starting training session: Waiting for first keypress...") );
	updateStatusBar();
	m_statusWidget->updateStatus(m_level, 1);
	m_statusWidget->updateSpeed( 0 );
	m_trainingPaused=true;		// Go into "Pause" mode
	m_trainingTimer->stop();    // Training timer will be started on first keypress.

    updateLevelChangeButtons();
}
// ----------------------------------------------------------------------------

// Pauses the current training session.
// This function is called from class KTouch (when the Pause action is executed).
void KTouchTrainer::pauseTraining() {
	m_trainingPaused=true;
	m_trainingTimer->stop();
	m_textLineWidget->setActive(false);
	m_statusWidget->updateStatus(m_level, m_levelStats.correctness());
	m_statusWidget->updateSpeed((int) m_levelStats.charSpeed() );
	updateStatusBarMessage(i18n("Training session paused. Training continues on next keypress...") );
	updateStatusBar();
}
// ----------------------------------------------------------------------------

// Continues the current training session.
// This function is called from class KTouch when a user presses a normal key
// while the training is in pause mode.
void KTouchTrainer::continueTraining() {
	m_trainingPaused=false;
	m_textLineWidget->setActive(true);
	m_statusWidget->updateStatus(m_level, m_levelStats.correctness() );
	m_statusWidget->updateSpeed((int) m_levelStats.charSpeed() );
	updateStatusBarMessage(i18n("Training session! The time is running...") );
	updateStatusBar();
	m_trainingTimer->start(LCD_UPDATE_INTERVAL);    // start the timer
}
// ----------------------------------------------------------------------------

void KTouchTrainer::updateLevelChangeButtons() {
    if (!Prefs::disableManualLevelChange()) {
	m_statusWidget->levelUpBtn->setEnabled(m_level < m_lecture->levelCount() - 1);
	m_statusWidget->levelDownBtn->setEnabled(m_level > 0);
    }
}

void KTouchTrainer::storeTrainingStatistics() {
	// at first get a reference to the statistics data of the current lecture
	KTouchLectureStats& data = KTouchPtr->getCurrentLectureStats();
	// update word count
	updateWordCount();
	// add word count to level and session statistics
	m_levelStats.m_words += m_wordsInCurrentLine;
	m_sessionStats.m_words += m_wordsInCurrentLine;
	// are there level stats to be stored?
	if (m_levelStats.m_elapsedTime != 0)  {
		//kDebug() << "[KTouchTrainer::storeTrainingStatistics]  Storing level statistics!";
		m_levelStats.m_timeRecorded = QDateTime::currentDateTime();
		data.m_levelStats.push_back( m_levelStats );
	}
	if (m_sessionStats.m_elapsedTime != 0) {
	   //kDebug() << "[KTouchTrainer::storeTrainingStatistics]  Storing session statistics!";
		m_sessionStats.m_timeRecorded = QDateTime::currentDateTime();
		data.m_sessionStats.push_back( m_sessionStats );
	}
}
// ----------------------------------------------------------------------------

bool KTouchTrainer::studentLineCorrect() const {
    if (m_teacherText.isEmpty())
        return m_studentText.isEmpty();

    return m_teacherText.startsWith(m_studentText);
}
// ----------------------------------------------------------------------------


// *** Public slots ***

void KTouchTrainer::levelUp() {
    if(Prefs::soundOnLevel()){
        m_player->setCurrentSource(m_levelUpSound.url());
        m_player->play();
    }

    if (m_level < m_lecture->levelCount() - 1) {
        m_level++;
    }
    else {
        levelAllComplete();
    }

    m_incLinesCount = 0;
    m_decLinesCount = 0;

    // Store level statistics if level is increased
    statsChangeLevel();
    gotoFirstLine();
    updateLevelChangeButtons();
}
// ----------------------------------------------------------------------------

void KTouchTrainer::levelDown() {
	if (m_level>0) {
       --m_level;
        if(Prefs::soundOnLevel()){
            m_player->setCurrentSource(m_levelDownSound.url());
            m_player->play();
        }
    }
    m_incLinesCount = 0;
    m_decLinesCount = 0;

    // Store level statistics if level is increased
    statsChangeLevel();
    gotoFirstLine();
    updateLevelChangeButtons();
}

void KTouchTrainer::levelAllComplete() {
    pauseTraining();
    KMessageBox::information(0,
                   i18n("You have finished this training exercise.\n"
                        "This training session will start from the beginning."), i18n("You rock!"));
    statsChangeLevel();
    startTraining(false);
}

void KTouchTrainer::timerTick() {
    if (m_trainingPaused) return;
    // Add the timer interval. I think we can neglect the error we make when the session is
    // paused and continued... it's not a scientific calculation, isn't it?
	statsAddTime(LCD_UPDATE_INTERVAL*0.001);
	// update only the widgets that are affected by progressing time
    m_statusWidget->updateSpeed((int) m_levelStats.charSpeed() );
}
// ----------------------------------------------------------------------------



// *** Private functions ***

void KTouchTrainer::newLine() {
    m_teacherText = m_lecture->level(m_level).line(m_line);
    m_studentText="";
	m_wordsInCurrentLine = 0;
    m_keyboardWidget->newKey(m_teacherText[0]);
    m_textLineWidget->setTeacherText(m_teacherText);
	updateStatusBar();	// update status bar
}
// ----------------------------------------------------------------------------

void KTouchTrainer::updateStatusBar() const {
	KTouchPtr->changeStatusbarStats(m_levelStats.m_correctChars, m_levelStats.m_totalChars,
		m_levelStats.m_words + m_wordsInCurrentLine,
		m_sessionStats.m_correctChars, m_sessionStats.m_totalChars,
		m_sessionStats.m_words + m_wordsInCurrentLine);
}
// ----------------------------------------------------------------------------

void KTouchTrainer::updateStatusBarMessage(const QString& message) const {
	KTouchPtr->changeStatusbarMessage(message);
}
// ----------------------------------------------------------------------------

void KTouchTrainer::updateWordCount() {
	// now update the m_wordsInCurrentLine variable
	if (!studentLineCorrect()) return;	// if error, don't update
	int words = 0;
	bool space = true;
	for (int i=0; i<m_studentText.length(); ++i) {
		bool is_space = (m_studentText[i] == QChar(' '));
		if (is_space) {
		 	if (space) continue; // two spaces after each other... ignore
			else {
				++words;
				space = true;
			}
		}
		else {
			if (!space) continue; // two chars after each other... ignore
			else {
				space = false;	// no need to add a word here.
			}
		}
	}
	// check if line is completely typed and add a word then
	if (m_studentText == m_teacherText) ++words;
	m_wordsInCurrentLine = words;
}
// ----------------------------------------------------------------------------

void KTouchTrainer::statsAddCorrectChar(QChar key) {
	m_levelStats.addCorrectChar(key);
	m_sessionStats.addCorrectChar(key);
}
// ----------------------------------------------------------------------------

void KTouchTrainer::statsAddWrongChar(QChar key) {
	m_levelStats.addWrongChar(key);
	m_sessionStats.addWrongChar(key);
}
// ----------------------------------------------------------------------------

void KTouchTrainer::statsRemoveCorrectChar(QChar) {
	m_levelStats.removeCorrectChar();
	m_sessionStats.removeCorrectChar();
}
// ----------------------------------------------------------------------------

void KTouchTrainer::statsAddTime(double dt) {
	m_levelStats.m_elapsedTime += dt;
	m_sessionStats.m_elapsedTime += dt;
}
// ----------------------------------------------------------------------------

void KTouchTrainer::statsChangeLevel() {
	//kDebug() << "[KTouchTrainer::statsChangeLevel]  First!";
	// first update word count and store data in
	updateWordCount();
	//kDebug() << "[KTouchTrainer::statsChangeLevel]  Adding word count of " << m_wordsInCurrentLine;
	m_levelStats.m_words += m_wordsInCurrentLine;
	m_sessionStats.m_words += m_wordsInCurrentLine;
	// get a reference to the statistics data of the current lecture
	KTouchLectureStats& data = KTouchPtr->getCurrentLectureStats();
	// are there level stats to be stored?
	if (m_levelStats.m_elapsedTime != 0)  {
		//kDebug() << "[KTouchTrainer::storeTrainingStatistics]  Storing level statistics!";
		m_levelStats.m_timeRecorded = QDateTime::currentDateTime();
		data.m_levelStats.push_back( m_levelStats );
	}
	// clear level stats
	m_levelStats.clear();
	// transfer current level to level statistics
	m_levelStats.m_levelNum = m_level;
	// remember level in session stats
	m_sessionStats.m_levelNums.insert(m_level);
	// show new level (in status widet) and 100% correctness
	m_statusWidget->updateStatus(m_level, 1);
}
// ----------------------------------------------------------------------------
