/***************************************************************************
 *   ktouchpreftraining.cpp                                                *
 *   ----------------------                                                *
 *   Copyright (C) 2003 by Andreas Nicolai                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchpreftraining.h"
#include "ktouchpreftraining.moc"

#include <qcheckbox.h>
#include <knuminput.h>
#include <qlabel.h>

KTouchPrefTraining::KTouchPrefTraining(QWidget *parent, const char *name, WFlags f)
 : KTouchPrefTrainingLayout(parent, name, f)
{
    connect(levelChangeCheck, SIGNAL(toggled(bool)), this, SLOT(levelChangeToggled(bool)) );
}

void KTouchPrefTraining::levelChangeToggled(bool on) {
    upSpeedLimit->setEnabled(on);
    upCorrectLimit->setEnabled(on);
    downSpeedLimit->setEnabled(on);
    downCorrectLimit->setEnabled(on);
    l1->setEnabled(on);
    l2->setEnabled(on);
    l3->setEnabled(on);
    l4->setEnabled(on);
    l5->setEnabled(on);
    l6->setEnabled(on);
    l7->setEnabled(on);
    l8->setEnabled(on);
    l9->setEnabled(on);
    l10->setEnabled(on);
}
