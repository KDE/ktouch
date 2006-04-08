/***************************************************************************
 *   ktouchstatistics.cpp                                                  *
 *   --------------------                                                  *
 *   Copyright (C) 2000 by Håvard Frøiland, 2004 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchstatistics.h"
#include "ktouchstatistics.moc"

#include <list>
#include <vector>
#include <utility>

#include <qprogressbar.h>
#include <qlcdnumber.h>
#include <qlabel.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>

#include <kpushbutton.h>
#include <kcombobox.h>
#include <kurl.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kdebug.h>
#include <ktabwidget.h>

#include "ktouch.h"
#include "ktouchchartwidget.h"

KTouchStatistics::KTouchStatistics(QWidget* parent)
	: KTouchStatisticsDlg(parent)
{
	sessionsRadio->setChecked(true);
	WPMRadio->setChecked(true);
	eventRadio->setChecked(true);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()) );
	connect(lectureCombo, SIGNAL(activated(int)), this, SLOT(lectureActivated(int)) );
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearHistory()) );
	// connect the radio buttons with the chart update function
    connect(buttonGroup1, SIGNAL(clicked(int)), this, SLOT(updateChartTab()) );
    connect(buttonGroup2, SIGNAL(clicked(int)), this, SLOT(updateChartTab()) );
    connect(buttonGroup3, SIGNAL(clicked(int)), this, SLOT(updateChartTab()) );
	
	// TODO : temporarily remove detailed stats page and deactivate options
	levelsRadio->setEnabled(false);
}
// ----------------------------------------------------------------------------

void KTouchStatistics::run(const KURL& currentLecture, const KTouchStatisticsData& stats,
	const KTouchLevelStats& currLevelStats,
	const KTouchSessionStats& currSessionStats)
{
//	kdDebug() << "[KTouchStatistics::run]" << endl;
//	kdDebug() << "  currentLecture = '" << currentLecture << "'" << endl;

	// fill lecture combo with data
	// loop over all lecturestatistics
	lectureCombo->clear();
	QMapConstIterator<KURL, KTouchLectureStats> it = stats.m_lectureStats.begin();
	m_currentIndex = 0;
	while (it != stats.m_lectureStats.end()) {
		QString t = it.data().m_lectureTitle;
		// if current lecture, remember index and adjust text
		if (it.key() == currentLecture ||
			currentLecture.isEmpty() && it.key().url()=="default")
		{
			m_currentIndex = lectureCombo->count();
			if (t == "default")   t = i18n("Default level...");
			lectureLabel1->setText(t);
			lectureLabel2->setText(t);
			t = i18n("***current***  ") + t;
		}
		lectureCombo->insertItem(t);
		++it;
	}
	if (lectureCombo->count()==0) {
		// this shouldn't happen if the dialog is run with proper data
		KMessageBox::information(this, i18n("No statistics data available yet!"));
		return;
	}

	// remember stats
	m_allStats = stats;
	m_currLevelStats = currLevelStats;
	m_currSessionStats = currSessionStats;
	// modify current lecture entry
	lectureCombo->setCurrentItem(m_currentIndex);
	lectureActivated(m_currentIndex);
	m_lectureIndex = m_currentIndex;
	
	// update the current tabs with current session/level data
	updateCurrentSessionTab();
	updateCurrentLevelTab();
	// set current session as current tab
	tabWidget->showPage(currentTab);
	exec();
}
// ----------------------------------------------------------------------------

void KTouchStatistics::lectureActivated(int index) {
	if (m_allStats.m_lectureStats.count()==0) {
		// TODO : Reset all tabs to "empty" look
		m_lectureIndex = 0;
		return;
	}
	if (index >= static_cast<int>(m_allStats.m_lectureStats.count())) {
		kdDebug() << "Lecture index out of range: " << index << " >= " << m_allStats.m_lectureStats.count() << endl;
		return;
	}
	m_lectureIndex = index;
	//kdDebug() << "Lecture stats changed: " << it.data().m_lectureTitle << endl;
	// update the tabs
	updateChartTab();
}
// ----------------------------------------------------------------------------

void KTouchStatistics::clearHistory() {
	if (KMessageBox::warningContinueCancel(this, i18n("Erase all statistics data for the current user?"),QString::null,KStdGuiItem::del()) 
		== KMessageBox::Continue)
	{
		KTouchPtr->clearStatistics(); // clear statistics data in KTouch
		// clear and reset local copy
		m_allStats.clear();
		QString s = lectureCombo->text(m_currentIndex);
		lectureCombo->clear();
		lectureCombo->insertItem(s);
		m_currentIndex = 0;
		lectureCombo->setCurrentItem(m_currentIndex);
		lectureActivated(m_currentIndex);
		updateChartTab();
	}
}
// ----------------------------------------------------------------------------


void KTouchStatistics::updateCurrentSessionTab() {
	// session/level/info
	QString levelnums;
    int last_level = -2;
    int levels_count = 0;
	std::set<unsigned int>::iterator last_it = m_currSessionStats.m_levelNums.end();
	--last_it;
	for (std::set<unsigned int>::iterator it = m_currSessionStats.m_levelNums.begin();
		it != m_currSessionStats.m_levelNums.end(); ++it)
	{
		// do we have a level number that is not a subsequent level of the
		// previous?
		
		if ((static_cast<unsigned int>(last_level + 1) != *it) ||
            (it == last_it))
		{
			if (it != m_currSessionStats.m_levelNums.begin()) {
				if (levels_count > 1)	levelnums += "...";
				else					levelnums += ",";
			}
			levels_count = 0;
			levelnums += QString("%1").arg(*it+1);
			
		}
		else {
			++levels_count;  // increase level count
		}
		last_level = *it;
	}
	levelLabel1->setText(levelnums);
    // general stats group
    elapsedTimeLCD->display(static_cast<int>(m_currSessionStats.m_elapsedTime));
    totalCharsLCD->display(static_cast<int>(m_currSessionStats.m_totalChars) );
    wrongCharsLCD->display(static_cast<int>(m_currSessionStats.m_totalChars-m_currSessionStats.m_correctChars) );
    wordsLCD->display(static_cast<int>(m_currSessionStats.m_words) );
    // typing rate group
	if (m_currSessionStats.m_elapsedTime == 0) {
		charSpeedLCD->display(0);
		wordSpeedLCD->display(0);
	}
	else {
		charSpeedLCD->display(static_cast<int>(m_currSessionStats.m_correctChars/m_currSessionStats.m_elapsedTime*60.0) );
		wordSpeedLCD->display(static_cast<int>(m_currSessionStats.m_words/m_currSessionStats.m_elapsedTime*60.0) );
	}
    // accuracy
    correctnessBar->setProgress(static_cast<int>(
		(100.0*m_currSessionStats.m_correctChars)/m_currSessionStats.m_totalChars) );
	// create sorted list of missed characters
	std::list<KTouchCharStats> charList( m_currSessionStats.m_charStats.begin(), m_currSessionStats.m_charStats.end());
	charList.sort(higher_miss_hit_ratio);
	
	std::list<KTouchCharStats>::const_iterator it=charList.begin();
    unsigned int i=0;
    for (; i<8 && it!=charList.end(); ++i, ++it) {
        if (it->missHitRatio()==0)
            break;  // stop listing keys when their hit-miss-ration is zero
        switch (i) {
          case 0 :  charLabel1->setText( it->m_char ); charProgress1->setEnabled(true);
                    charProgress1->setProgress( it->missHitRatio() ); break;
          case 1 :  charLabel2->setText( it->m_char ); charProgress2->setEnabled(true);
                    charProgress2->setProgress( it->missHitRatio() ); break;
          case 2 :  charLabel3->setText( it->m_char ); charProgress3->setEnabled(true);
                    charProgress3->setProgress( it->missHitRatio() ); break;
          case 3 :  charLabel4->setText( it->m_char ); charProgress4->setEnabled(true);
                    charProgress4->setProgress( it->missHitRatio() ); break;
          case 4 :  charLabel5->setText( it->m_char ); charProgress5->setEnabled(true);
                    charProgress5->setProgress( it->missHitRatio() ); break;
          case 5 :  charLabel6->setText( it->m_char ); charProgress6->setEnabled(true);
                    charProgress6->setProgress( it->missHitRatio() ); break;
          case 6 :  charLabel7->setText( it->m_char ); charProgress7->setEnabled(true);
                    charProgress7->setProgress( it->missHitRatio() ); break;
          case 7 :  charLabel8->setText( it->m_char ); charProgress8->setEnabled(true);
                    charProgress8->setProgress( it->missHitRatio() ); break;
        }
    }
	// set remaining char labels and progress bars to zero
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
        }
    }
}
// ----------------------------------------------------------------------------

void KTouchStatistics::updateCurrentLevelTab() {
	// level info
	levelLabel2->setText( QString("%1").arg(m_currLevelStats.m_levelNum+1) );
    // general stats group
    elapsedTimeLCDLevel->display(static_cast<int>(m_currLevelStats.m_elapsedTime));
    totalCharsLCDLevel->display(static_cast<int>(m_currLevelStats.m_totalChars) );
    wrongCharsLCDLevel->display(static_cast<int>(m_currLevelStats.m_totalChars-m_currLevelStats.m_correctChars) );
    wordsLCDLevel->display(static_cast<int>(m_currLevelStats.m_words) );
    // typing rate group
	if (m_currLevelStats.m_elapsedTime == 0) {
		charSpeedLCDLevel->display(0);
		wordSpeedLCDLevel->display(0);
	}
	else {
		charSpeedLCDLevel->display(static_cast<int>(m_currLevelStats.m_correctChars/m_currLevelStats.m_elapsedTime*60.0) );
		wordSpeedLCDLevel->display(static_cast<int>(m_currLevelStats.m_words/m_currLevelStats.m_elapsedTime*60.0) );
	}
    // accuracy
    correctnessBarLevel->setProgress(static_cast<int>(
		(100.0*m_currLevelStats.m_correctChars)/m_currLevelStats.m_totalChars) );
	// create sorted list of missed characters
	std::list<KTouchCharStats> charList( m_currLevelStats.m_charStats.begin(), m_currLevelStats.m_charStats.end());
	charList.sort(higher_miss_hit_ratio);
	
	std::list<KTouchCharStats>::const_iterator it=charList.begin();
    unsigned int i=0;
    for (; i<8 && it!=charList.end(); ++i, ++it) {
        if (it->missHitRatio()==0)
            break;  // stop listing keys when their hit-miss-ration is zero
        switch (i) {
          case 0 :  charLabel1Level->setText( it->m_char ); charProgress1->setEnabled(true);
                    charProgress1Level->setProgress( it->missHitRatio() ); break;
          case 1 :  charLabel2Level->setText( it->m_char ); charProgress2->setEnabled(true);
                    charProgress2Level->setProgress( it->missHitRatio() ); break;
          case 2 :  charLabel3Level->setText( it->m_char ); charProgress3->setEnabled(true);
                    charProgress3Level->setProgress( it->missHitRatio() ); break;
          case 3 :  charLabel4Level->setText( it->m_char ); charProgress4->setEnabled(true);
                    charProgress4Level->setProgress( it->missHitRatio() ); break;
          case 4 :  charLabel5Level->setText( it->m_char ); charProgress5->setEnabled(true);
                    charProgress5Level->setProgress( it->missHitRatio() ); break;
          case 5 :  charLabel6Level->setText( it->m_char ); charProgress6->setEnabled(true);
                    charProgress6Level->setProgress( it->missHitRatio() ); break;
          case 6 :  charLabel7Level->setText( it->m_char ); charProgress7->setEnabled(true);
                    charProgress7Level->setProgress( it->missHitRatio() ); break;
          case 7 :  charLabel8Level->setText( it->m_char ); charProgress8->setEnabled(true);
                    charProgress8Level->setProgress( it->missHitRatio() ); break;
        }
    }
	// set remaining char labels and progress bars to zero
    for(; i<8; ++i) {
        switch (i) {
          case 0 :  charLabel1Level->setText(" "); charProgress1Level->setProgress(0); charProgress1Level->setEnabled(false); break;
          case 1 :  charLabel2Level->setText(" "); charProgress2Level->setProgress(0); charProgress2Level->setEnabled(false); break;
          case 2 :  charLabel3Level->setText(" "); charProgress3Level->setProgress(0); charProgress3Level->setEnabled(false); break;
          case 3 :  charLabel4Level->setText(" "); charProgress4Level->setProgress(0); charProgress4Level->setEnabled(false); break;
          case 4 :  charLabel5Level->setText(" "); charProgress5Level->setProgress(0); charProgress5Level->setEnabled(false); break;
          case 5 :  charLabel6Level->setText(" "); charProgress6Level->setProgress(0); charProgress6Level->setEnabled(false); break;
          case 6 :  charLabel7Level->setText(" "); charProgress7Level->setProgress(0); charProgress7Level->setEnabled(false); break;
          case 7 :  charLabel8Level->setText(" "); charProgress8Level->setProgress(0); charProgress8Level->setEnabled(false); break;
        }
    }
}
// ----------------------------------------------------------------------------

void KTouchStatistics::updateChartTab() {
	// remove all current chart objects
	chartWidget->clearObjectList();
	// if no lecture data is available, return
	if (m_allStats.m_lectureStats.count()==0 || m_lectureIndex >= m_allStats.m_lectureStats.count())  return;
	// what kind of chart is required?
	if (levelsRadio->isChecked()) {
		// TODO : nothing yet
	}
	else {
		// find correct lecture index
		QMapConstIterator<KURL, KTouchLectureStats> it = m_allStats.m_lectureStats.begin();	
		int index = m_lectureIndex;
		while (index-- > 0) ++it;
		std::vector< std::pair<double, double> > data;
		QString caption = "Session data";
		switch (buttonGroup2->selectedId()) {
			case 0 : // words per minute
				// loop over all session data entries in *it and store words per minute data
				for (QValueVector<KTouchSessionStats>::const_iterator session_it = (*it).m_sessionStats.begin();
					session_it != (*it).m_sessionStats.end(); ++session_it)
				{
					double t = (*session_it).m_elapsedTime;
					double wpm = (t == 0) ? 0 : (*session_it).m_words/t*60.0;
					double tp = (*session_it).m_timeRecorded.toTime_t()/(3600.0*24);
					data.push_back(std::make_pair(tp, wpm) );
				}
				// add current session if selected lecture matches
				if (m_currentIndex == m_lectureIndex && 
					m_currSessionStats.m_elapsedTime != 0) 
				{
					double t = m_currSessionStats.m_elapsedTime;
					double wpm = m_currSessionStats.m_words/t*60.0;
					double tp = QDateTime::currentDateTime().toTime_t()/(3600.0*24);
					data.push_back(std::make_pair(tp, wpm) );
				}
				chartWidget->LeftAxis.setLabel( i18n("Words per minute") );
				chartWidget->LeftAxis.setLabelFormat(0, 'f', 0);
				break;


			case 1 : // chars per minute
				// loop over all session data entries in *it and store chars per minute data
				for (QValueVector<KTouchSessionStats>::const_iterator session_it = (*it).m_sessionStats.begin();
					session_it != (*it).m_sessionStats.end(); ++session_it)
				{
					double t = (*session_it).m_elapsedTime;
					double cpm = (t == 0) ? 0 : (*session_it).m_correctChars/t*60.0;
					double tp = (*session_it).m_timeRecorded.toTime_t()/(3600.0*24);
					data.push_back(std::make_pair(tp, cpm) );
				}
				// add current session
				if (m_currentIndex == m_lectureIndex && 
					m_currSessionStats.m_elapsedTime != 0) 
				{
					double t = m_currSessionStats.m_elapsedTime;
					double cpm = m_currSessionStats.m_correctChars/t*60.0;
					double tp = QDateTime::currentDateTime().toTime_t()/(3600.0*24);
					data.push_back(std::make_pair(tp, cpm) );
				}
				chartWidget->LeftAxis.setLabel( i18n("Characters per minute") );
				chartWidget->LeftAxis.setLabelFormat(0, 'f', 0);
				break;


			case 2 : // correctness
				// loop over all session data entries in *it and store correctness data
				for (QValueVector<KTouchSessionStats>::const_iterator session_it = (*it).m_sessionStats.begin();
					session_it != (*it).m_sessionStats.end(); ++session_it)
				{
					double tc = (*session_it).m_totalChars;
					double corr = (tc == 0) ? 0 : (*session_it).m_correctChars/tc;
					double tp = (*session_it).m_timeRecorded.toTime_t()/(3600.0*24);
					data.push_back(std::make_pair(tp, corr) );
				}
				// add current session
				if (m_currentIndex == m_lectureIndex && 
					m_currSessionStats.m_totalChars != 0) 
				{
					double tc = m_currSessionStats.m_totalChars;
					double corr = m_currSessionStats.m_correctChars/tc;
					double tp = QDateTime::currentDateTime().toTime_t()/(3600.0*24);
					data.push_back(std::make_pair(tp, corr) );
				}
				chartWidget->LeftAxis.setLabel( i18n("Correctness") );
				chartWidget->LeftAxis.setLabelFormat(0, 'g', 1);
				break;


			case 3 : // skill
				// loop over all session data entries in *it and store correctness data
				for (QValueVector<KTouchSessionStats>::const_iterator session_it = (*it).m_sessionStats.begin();
					session_it != (*it).m_sessionStats.end(); ++session_it)
				{
					double tc = (*session_it).m_totalChars;
					double corr = (tc == 0) ? 0 : (*session_it).m_correctChars/tc;
					double t = (*session_it).m_elapsedTime;
					double cpm = (t == 0) ? 0 : (*session_it).m_correctChars/t*60.0;
					double skill = corr*cpm;
					double tp = (*session_it).m_timeRecorded.toTime_t()/(3600.0*24);
					data.push_back(std::make_pair(tp, skill) );
				}
				// add current session
				if (m_currentIndex == m_lectureIndex && 
					m_currSessionStats.m_totalChars != 0 && 
					m_currSessionStats.m_elapsedTime != 0)
				{
					double tc = m_currSessionStats.m_totalChars;
					double corr = m_currSessionStats.m_correctChars/tc;
					double t = m_currSessionStats.m_elapsedTime;
					double cpm = m_currSessionStats.m_correctChars/t*60.0;
					double skill = corr*cpm;
					double tp = QDateTime::currentDateTime().toTime_t()/(3600.0*24);
					data.push_back(std::make_pair(tp, skill) );
				}
				chartWidget->LeftAxis.setLabel( i18n("Skill") );
				chartWidget->LeftAxis.setLabelFormat(0, 'f', 0);
				break;
				
			default : return;
		}
		// Create plot object for session statistics
		KPlotObject * ob;
		if (data.size() <= 1) return;
		ob = new KPlotObject(caption, "red", KPlotObject::CURVE, 2, KPlotObject::SOLID);
		
		// Add some random points to the plot object
		double min_x = 1e20;
		double max_x = -1;
		double max_y = -1;
		for (unsigned int i=0; i<data.size(); ++i) {
			double x;
			if (timeRadio->isChecked()) {
				x = data[i].first - data[0].first;
				chartWidget->BottomAxis.setLabel( i18n( "Time since first practice session in days" ));
			}
			else {	
				x = i+1;
				chartWidget->BottomAxis.setLabel( i18n( "Sessions" ));
			}
			ob->addPoint( DPoint(x, data[i].second) );
			min_x = std::min(x, min_x);
			max_x = std::max(x, max_x);
			max_y = std::max(data[i].second, max_y);
		}
		if (max_y == 0)  max_y = 1;
		max_y *= 1.1;
		chartWidget->setLimits( min_x, max_x, -0.1*max_y, max_y );
//		kdDebug() << min_x << " " << max_x << "    " << max_y << endl;
		// Add plot object to chart
		chartWidget->addObject(ob);
	}
}
// ----------------------------------------------------------------------------

/*

void KTouchStatistics::updateChartTab() {
    if (m_trainer->m_sessionHistory.size()<2) {
        chartTypeButtonGroup->setEnabled(false);
        chartWidget->setEraseColor( Qt::gray );
    }
    else {
        chartTypeButtonGroup->setEnabled(true);
        chartWidget->setEraseColor( Qt::white );
    };
    if (charSpeedButton->isChecked())
        chartWidget->setChartType( KTouchChartWidget::CharsPerMinute );
    else if (wordSpeedButton->isChecked())
        chartWidget->setChartType( KTouchChartWidget::WordsPerMinute );
    else if (accuracyButton->isChecked())
        chartWidget->setChartType( KTouchChartWidget::Accuracy );
    else if (timeButton->isChecked())
        chartWidget->setChartType( KTouchChartWidget::ElapsedTime );
}
// ----------------------------------------------------------------------------

*/
