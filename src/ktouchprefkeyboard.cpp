/***************************************************************************
 *   ktouchprefkeyboard.h                                                  *
 *   --------------------                                                  *
 *   Copyright (C) 2003 by Andreas Nicolai                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchprefkeyboard.h"
#include "ktouchprefkeyboard.moc"

#include <kcombobox.h>
#include <klocale.h>
#include <qcheckbox.h>
#include <kfontdialog.h>
#include <kpushbutton.h>
#include "ktouchsettings.h"

KTouchPrefKeyboard::KTouchPrefKeyboard(QWidget *parent, const char *name, WFlags f)
 : KTouchPrefKeyboardLayout(parent, name, f)
{
    // fill keyboard layout combo
    keyboardLayoutCombo->clear();
    for (QStringList::const_iterator it=KTouchConfig().m_keyboardLayouts.begin();
                                    it!=KTouchConfig().m_keyboardLayouts.end();
                                    ++it)
    {
        keyboardLayoutCombo->insertItem(*it);
    };
}

void KTouchPrefKeyboard::updateFont(const QFont& font) {
    m_font = font;
}

void KTouchPrefKeyboard::fontChooseBtnClicked() {
    QFont tmpFont(m_font);
    int result = KFontDialog::getFont(tmpFont);
    if (result==KFontDialog::Accepted) {
    		updateFont(tmpFont);
        emit fontChanged();
    };
}
