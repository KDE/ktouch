/***************************************************************************
 *   ktouchstatus.cpp                                                      *
 *   ----------------                                                      *
 *   Copyright (C) 2000 by Håvard Frøiland, 2004 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchstatus.h"
#include "ktouchstatus.moc"

#include <tqlcdnumber.h>
#include <tqlabel.h>
#include <tqprogressbar.h>
#include <kdebug.h>
#include <kpushbutton.h>
#include "prefs.h"

KTouchtqStatus::KTouchtqStatus(TQWidget *parent)
 : KTouchtqStatusLayout(parent)
{
    applyPreferences();
}

void KTouchtqStatus::applyPreferences() {
	if (Prefs::autoLevelChange()) {
		levelUpBtn->setEnabled( !Prefs::disableManualLevelChange() );
		levelDownBtn->setEnabled( !Prefs::disableManualLevelChange() );
	}
	else {
		levelUpBtn->setEnabled( true );
		levelDownBtn->setEnabled( true );
	}
}

void KTouchtqStatus::setNewChars(const TQString& newChars) {
    newCharsLabel->setText(newChars);
}

void KTouchtqStatus::updatetqStatus(unsigned int level, double correctness) {
    levelLCD->display(static_cast<int>(level)+1); // +1 because our level variable ranges from 0...n-1
    correctnessBar->setProgress(static_cast<int>(correctness*100) );
}
