/***************************************************************************
 *   ktouchlevelsummarydialog.h                                            *
 *   --------------------------                                            *
 *   Copyright (C) 2007 by Andreas Nicolai                                 *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHLEVELSUMMARYDIALOG_H
#define KTOUCHLEVELSUMMARYDIALOG_H

#include "ui_ktouchlevelsummarydialog.h"
#include <QDialog>

#include "ktouchstatisticsdata.h"

/// @brief The summary/highscore dialog after a level was completed.
class KTouchLevelSummaryDialog : public QDialog, public Ui_KTouchLevelSummaryDialog {
  Q_OBJECT

public:
	/// Constructor
	KTouchLevelSummaryDialog(QWidget* parent = 0);
	/// Destructor
	~KTouchLevelSummaryDialog();

	void showInfo(bool & increaseLevel, bool & decreaseLevel, const KTouchLevelStats& levelStats);

public slots:
	void on_retryButton_clicked();
	void on_continueButton_clicked();

};

#endif // KTOUCHLEVELSUMMARYDIALOG_H

