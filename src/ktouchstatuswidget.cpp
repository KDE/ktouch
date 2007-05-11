/***************************************************************************
 *   ktouchstatuswidget.cpp                                                *
 *   ----------------------                                                *
 *   Copyright (C) 2000 by Håvard Frøiland, 2004 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchstatuswidget.h"
#include "ktouchstatuswidget.moc"

#include <kdebug.h>
#include <kpushbutton.h>

#include "prefs.h"

KTouchStatusWidget::KTouchStatusWidget(QWidget *parent)
 : QWidget(parent)
{
    setupUi(this);
    applyPreferences();
}

void KTouchStatusWidget::applyPreferences() {
	if (Prefs::autoLevelChange()) {
		levelUpBtn->setEnabled( !Prefs::disableManualLevelChange() );
		levelDownBtn->setEnabled( !Prefs::disableManualLevelChange() );
	}
	else {
		levelUpBtn->setEnabled( true );
		levelDownBtn->setEnabled( true );
	}
}

void KTouchStatusWidget::setNewChars(const QString& newChars) {
    newCharsLabel->setText(newChars);
}

void KTouchStatusWidget::updateStatus(unsigned int level, double correctness) {
    levelLCD->display(static_cast<int>(level)+1); // +1 because our level variable ranges from 0...n-1

    int correctnessProcent = static_cast<int>(correctness*100);
    if(Prefs::upCorrectLimit() < correctnessProcent)
	correctnessLedGreen->on();
    else
	correctnessLedGreen->off();

    if(Prefs::downCorrectLimit() > correctnessProcent)
	correctnessLedRed->on();
    else
	correctnessLedRed->off();

    correctnessBar->setValue(correctnessProcent);
}

void KTouchStatusWidget::updateSpeed(int speed) {
    if(Prefs::upSpeedLimit() < speed)
	speedLedGreen->on();
    else
	speedLedGreen->off();

    if(Prefs::downSpeedLimit() > speed)
	speedLedRed->on();
    else
	speedLedRed->off();

    speedLCD->display(speed);
}
