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
	// setup the dialog according to the arguments
	if (increaseLevel) {
	}
}

