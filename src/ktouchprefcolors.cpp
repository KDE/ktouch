/***************************************************************************
 *   ktouchprefcolors.cpp                                                  *
 *   --------------------                                                  *
 *   Copyright (C) 2003 by Andreas Nicolai                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "ktouchprefcolors.h"
#include "ktouchprefcolors.moc"

#include <qcheckbox.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <kcolorbutton.h>
#include "ktouchsettings.h"

KTouchPrefColors::KTouchPrefColors(QWidget *parent, const char *name, WFlags f)
 : KTouchPrefColorsLayout(parent, name, f)
{
    // fill color scheme combo
    keyboardColorCombo->clear();
    for (QValueList<KTouchKeyboardColor>::const_iterator it=KTouchConfig().m_keyboardColors.begin();
                                                        it!=KTouchConfig().m_keyboardColors.end();
                                                        ++it)
    {
        keyboardColorCombo->insertItem((*it).m_name);
    };
    connect(errorColorCheck, SIGNAL(toggled(bool)), this, SLOT(errorColorCheckToggled(bool)) );
}

KTouchPrefColors::~KTouchPrefColors()
{
}

void KTouchPrefColors::errorColorCheckToggled(bool on) {
    errorColorCheck->setChecked(on);
    errorTextLabel->setEnabled(on);
    errorBackgroundLabel->setEnabled(on);
    errorTextBtn->setEnabled(on);
    errorBackgroundBtn->setEnabled(on);
}
