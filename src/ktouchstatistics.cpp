/***************************************************************************
 *   ktouchstatistics.cpp                                                  *
 *   --------------------                                                  *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchstatistics.h"
#include "ktouchstatistics.moc"

#include <qlcdnumber.h>
#include <qslider.h>
#include <qlabel.h>
#include <qprogressbar.h>

#include <kdebug.h>
#include <kpushbutton.h>
#include <kmessagebox.h>
#include <klocale.h>

#include <list>
#include <utility> // for std::min and std::max

#include "ktouchtrainer.h"
#include "ktouchtrainingsession.h"
#include "ktouchcharstats.h"
using std::set;
using std::list;

KTouchStatistics::KTouchStatistics(QWidget *parent, KTouchTrainer* trainer)
  : KTouchStatisticsLayout(parent), m_trainer(trainer)
{
    sessionCountSlider->setMaxValue(m_trainer->m_sessionHistory.count());
    sessionCountSlider->setValue(m_trainer->m_sessionHistory.count());
    connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()) );
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearHistory()) );
    connect(sessionCountSlider, SIGNAL(valueChanged(int)), this, SLOT(updateAverageTab()) );
};

void KTouchStatistics::showEvent(QShowEvent*) {
    // adjust slider range in case some sessions have been added
    sessionCountSlider->setMaxValue(m_trainer->m_sessionHistory.count());
    // update the tabs
    updateCurrentTab();
    updateAverageTab();
    updateChartTab();
};

void KTouchStatistics::updateCurrentTab() {
    // general stats group
    elapsedTimeLCD->display(static_cast<int>(m_trainer->m_session.m_elapsedTime/1000));
    totalCharsLCD->display(static_cast<int>(m_trainer->m_session.m_totalChars) );
    wrongCharsLCD->display(static_cast<int>(m_trainer->m_session.m_totalChars-m_trainer->m_session.m_correctChars) );
    wordsLCD->display(static_cast<int>(m_trainer->m_session.m_words) );
    // typing rate group
    charSpeedLCD->display(static_cast<int>(m_trainer->m_session.charSpeed()) );
    wordSpeedLCD->display(static_cast<int>(m_trainer->m_session.wordSpeed()) );
    // accuracy
    correctnessBar->setProgress(static_cast<int>(m_trainer->m_session.correctness()*100) );
    // worst miss/hit ratio group
    std::list<KTouchCharStats> charList( m_trainer->m_session.sortedCharStats() );
    std::list<KTouchCharStats>::const_iterator it=charList.begin();
    unsigned int i=0;
    for (; i<8 && it!=charList.end(); ++i, ++it) {
        if (it->hitMissRatio()==0)
            break;  // stop listing keys when their hit-miss-ration is zero
        switch (i) {
          case 0 :  charLabel1->setText( it->m_char ); charProgress1->setEnabled(true);
                    charProgress1->setProgress( it->hitMissRatio() ); break;
          case 1 :  charLabel2->setText( it->m_char ); charProgress2->setEnabled(true);
                    charProgress2->setProgress( it->hitMissRatio() ); break;
          case 2 :  charLabel3->setText( it->m_char ); charProgress3->setEnabled(true);
                    charProgress3->setProgress( it->hitMissRatio() ); break;
          case 3 :  charLabel4->setText( it->m_char ); charProgress4->setEnabled(true);
                    charProgress4->setProgress( it->hitMissRatio() ); break;
          case 4 :  charLabel5->setText( it->m_char ); charProgress5->setEnabled(true);
                    charProgress5->setProgress( it->hitMissRatio() ); break;
          case 5 :  charLabel6->setText( it->m_char ); charProgress6->setEnabled(true);
                    charProgress6->setProgress( it->hitMissRatio() ); break;
          case 6 :  charLabel7->setText( it->m_char ); charProgress7->setEnabled(true);
                    charProgress7->setProgress( it->hitMissRatio() ); break;
          case 7 :  charLabel8->setText( it->m_char ); charProgress8->setEnabled(true);
                    charProgress8->setProgress( it->hitMissRatio() ); break;
        };
    };
    for(; i<8; ++i) {
        switch (i) {
          case 0 :  charLabel1->setText(" "); charProgress1->setProgress(0); charProgress1->setEnabled(false); break;
          case 1 :  charLabel2->setText(" "); charProgress2->setProgress(0); charProgress2->setEnabled(false); break;
          case 2 :  charLabel3->setText(" "); charProgress3->setProgress(0); charProgress3->setEnabled(false); break;
          case 3 :  charLabel4->setText(" "); charProgress4->setProgress(0); charProgress4->setEnabled(false); break;
          case 4 :  charLabel5->setText(" "); charProgress5->setProgress(0); charProgress5->setEnabled(false); break;
          case 5 :  charLabel6->setText(" "); charProgress6->setProgress(0); charProgress6->setEnabled(false); break;
          case 6 :  charLabel7->setText(" "); charProgress7->setProgress(0); charProgress7->setEnabled(false); break;
          case 7 :  charLabel8->setText(" "); charProgress8->setProgress(0); charProgress8->setEnabled(false); break;
        };
    };
};

void KTouchStatistics::updateChartTab() {
};

void KTouchStatistics::clearHistory() {
    if (KMessageBox::questionYesNo(0, i18n("Do you really want to clear the session history?"))==KMessageBox::Yes) {
        sessionCountSlider->setValue(0);
        sessionCountSlider->setMaxValue(0);
        m_trainer->m_sessionHistory.clear();
        updateAverageTab();
    };
};

void KTouchStatistics::updateAverageTab() {
    if (sessionCountSlider->value()==0)
        sessionCountLabel->setText( i18n("training session.") );
    else
        sessionCountLabel->setText( i18n("%1 training sessions.").arg(sessionCountSlider->value()+1));
    KTouchTrainingSession sessionSum=m_trainer->m_session;
    KTouchTrainingSession sessionMin=m_trainer->m_session;
    KTouchTrainingSession sessionMax=m_trainer->m_session;
    double charSpeedSum, charSpeedMin, charSpeedMax;
    charSpeedSum=charSpeedMin=charSpeedMax=m_trainer->m_session.charSpeed();
    double wordSpeedSum, wordSpeedMin, wordSpeedMax;
    wordSpeedSum=wordSpeedMin=wordSpeedMax=m_trainer->m_session.wordSpeed();
    double correctnessSum, correctnessMin, correctnessMax;
    correctnessSum=correctnessMin=correctnessMax=m_trainer->m_session.correctness();
    std::set<KTouchCharStats> charStatsSum = m_trainer->m_session.m_charStats;
    unsigned int lastSession=m_trainer->m_sessionHistory.size()-1;
    for (int i=0; i<sessionCountSlider->value(); ++i) {
        const KTouchTrainingSession &session = m_trainer->m_sessionHistory[lastSession-i];
        unsigned int elapsedTime = session.m_elapsedTime;
        unsigned int totalChars = session.m_totalChars;
        unsigned int correctChars = session.m_correctChars;
        unsigned int words = session.m_words;
        double charSpeed = session.charSpeed();
        double wordSpeed = session.wordSpeed();
        double correctness = session.correctness();
        sessionSum.m_elapsedTime += elapsedTime;
        sessionMin.m_elapsedTime = std::min(elapsedTime, sessionMin.m_elapsedTime);
        sessionMax.m_elapsedTime = std::max(elapsedTime, sessionMax.m_elapsedTime);
        sessionSum.m_totalChars += totalChars;
        sessionMin.m_totalChars = std::min(totalChars, sessionMin.m_totalChars);
        sessionMax.m_totalChars = std::max(totalChars, sessionMax.m_totalChars);
        sessionSum.m_correctChars += correctChars;
        sessionMin.m_correctChars = std::min(correctChars, sessionMin.m_correctChars);
        sessionMax.m_correctChars = std::max(correctChars, sessionMax.m_correctChars);
        sessionSum.m_words += words;
        sessionMin.m_words = std::min(words, sessionMin.m_words);
        sessionMax.m_words = std::max(words, sessionMax.m_words);
        // sum up and store min/max of calculated stats
        charSpeedSum += charSpeed;
        charSpeedMin = std::min(charSpeed, charSpeedMin);
        charSpeedMax = std::max(charSpeed, charSpeedMax);
        wordSpeedSum += wordSpeed;
        wordSpeedMin = std::min(wordSpeed, wordSpeedMin);
        wordSpeedMax = std::max(wordSpeed, wordSpeedMax);
        correctnessSum += correctness;
        correctnessMin = std::min(correctness, correctnessMin);
        correctnessMax = std::max(correctness, correctnessMax);
        // store character stats
        for (set<KTouchCharStats>::iterator srcIt = session.m_charStats.begin(); srcIt != session.m_charStats.end(); ++srcIt) {
            set<KTouchCharStats>::iterator it = charStatsSum.find( KTouchCharStats(srcIt->m_char,0,0) );
            if (it==charStatsSum.end())
                charStatsSum.insert( *srcIt );
            else {
                const_cast<KTouchCharStats&>(*it).m_correctCount += srcIt->m_correctCount;
                const_cast<KTouchCharStats&>(*it).m_wrongCount += srcIt->m_wrongCount;
            };
        };
    };
    // calculate averaged values
    if (sessionCountSlider->value() > 0) {
        unsigned int sessionCount = sessionCountSlider->value()+1;
        // now calculate the average numbers
        sessionSum.m_elapsedTime /= sessionCount;
        sessionSum.m_totalChars /= sessionCount;
        sessionSum.m_correctChars /= sessionCount;
        sessionSum.m_words /= sessionCount;
        charSpeedSum /= sessionCount;
        wordSpeedSum /= sessionCount;
        correctnessSum /= sessionCount;
    };
    // sort char stats (worst hit-miss-ratio first in list)
    list<KTouchCharStats> charStatsList(charStatsSum.begin(), charStatsSum.end());
    charStatsList.sort( greaterHitMissRatio );

    // update widgets
    charSpeedAverageLCD->display(static_cast<int>(charSpeedSum) );
    charSpeedMinLCD->display(static_cast<int>(charSpeedMin) );
    charSpeedMaxLCD->display(static_cast<int>(charSpeedMax) );
    wordSpeedAverageLCD->display(static_cast<int>(wordSpeedSum) );
    wordSpeedMinLCD->display(static_cast<int>(wordSpeedMin) );
    wordSpeedMaxLCD->display(static_cast<int>(wordSpeedMax) );
    correctnessAverageBar->setProgress(static_cast<int>(correctnessSum*100) );
    correctnessMinLCD->display(static_cast<int>(correctnessMin*100) );
    correctnessMaxLCD->display(static_cast<int>(correctnessMax*100) );
    // update the focus characters
    list<KTouchCharStats>::const_iterator it=charStatsList.begin();
    unsigned int i=0;
    for (; i<6 && it!=charStatsList.end(); ++i, ++it) {
        if (it->hitMissRatio()==0)
            break;  // stop listing keys when their hit-miss-ration is zero
        switch (i) {
          case 0 :  charLabel1_2->setText( it->m_char ); charProgress1_2->setEnabled(true);
                    charProgress1_2->setProgress( it->hitMissRatio() ); break;
          case 1 :  charLabel2_2->setText( it->m_char ); charProgress2_2->setEnabled(true);
                    charProgress2_2->setProgress( it->hitMissRatio() ); break;
          case 2 :  charLabel3_2->setText( it->m_char ); charProgress3_2->setEnabled(true);
                    charProgress3_2->setProgress( it->hitMissRatio() ); break;
          case 3 :  charLabel4_2->setText( it->m_char ); charProgress4_2->setEnabled(true);
                    charProgress4_2->setProgress( it->hitMissRatio() ); break;
          case 4 :  charLabel5_2->setText( it->m_char ); charProgress5_2->setEnabled(true);
                    charProgress5_2->setProgress( it->hitMissRatio() ); break;
          case 5 :  charLabel6_2->setText( it->m_char ); charProgress6_2->setEnabled(true);
                    charProgress6_2->setProgress( it->hitMissRatio() ); break;
        };
    };
    for(; i<6; ++i) {
        switch (i) {
          case 0 :  charLabel1_2->setText(" "); charProgress1_2->setProgress(0); charProgress1_2->setEnabled(false); break;
          case 1 :  charLabel2_2->setText(" "); charProgress2_2->setProgress(0); charProgress2_2->setEnabled(false); break;
          case 2 :  charLabel3_2->setText(" "); charProgress3_2->setProgress(0); charProgress3_2->setEnabled(false); break;
          case 3 :  charLabel4_2->setText(" "); charProgress4_2->setProgress(0); charProgress4_2->setEnabled(false); break;
          case 4 :  charLabel5_2->setText(" "); charProgress5_2->setProgress(0); charProgress5_2->setEnabled(false); break;
          case 5 :  charLabel6_2->setText(" "); charProgress6_2->setProgress(0); charProgress6_2->setEnabled(false); break;
        };
    };
};
