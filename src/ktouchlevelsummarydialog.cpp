/***************************************************************************
 *   ktouchlevelsummarydialog.cpp                                          *
 *   ----------------------------                                          *
 *   Copyright (C) 2007 by Andreas Nicolai                                 *
 *   <ghorwin@users.sourceforge.net>                                       * 
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchlevelsummarydialog.h"
#include "ktouchlevelsummarydialog.moc"


#include <kdebug.h>

#include "prefs.h"
#include "ktouch.h"

KTouchLevelSummaryDialog::KTouchLevelSummaryDialog(QWidget* parent) : QDialog(parent) {
	setupUi(this);
}

KTouchLevelSummaryDialog::~KTouchLevelSummaryDialog() {
}

void KTouchLevelSummaryDialog::on_retryButton_clicked() {
	accept();
}

void KTouchLevelSummaryDialog::on_continueButton_clicked() {
	reject();
}

void KTouchLevelSummaryDialog::showInfo(bool & increaseLevel, bool & decreaseLevel, const KTouchLevelStats& levelStats) {
	Q_UNUSED(decreaseLevel)
	// setup the dialog according to the arguments
	if (increaseLevel) {
	}

	//WARNING: not needed annma 05/2009 const QStringList& userList = KTouchPtr->userList();
	const QMap<QString, KTouchStatisticsData>& allUserStats = KTouchPtr->userStats();

	// get the statistics data for this lecture and the current user
	QString currentUser = Prefs::currentUserName();
	QString currentFile = Prefs::currentLectureFile();

	// get the statistics data for the current user
	const KTouchStatisticsData& currentUserStats = allUserStats[currentUser];
	// extract the data for the current lecture
	const KTouchLectureStats& currentLectureStats = currentUserStats.m_lectureStats[currentFile];

	kDebug() << "Level done" << currentUser;
	kDebug() << currentLectureStats.m_levelStats.count() << " Level stats available";

	// get all stats for this lesson and put them in the table
	for (int i=0; i<currentLectureStats.m_levelStats.count(); ++i) {
		// skip all that are not this level
		if (currentLectureStats.m_levelStats[i].m_levelNum != levelStats.m_levelNum) continue;
		

	}

	exec();
}

