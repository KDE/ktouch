/***************************************************************************
 *   ktouchprefgeneral.cpp                                                 *
 *   ---------------------                                                 *
 *   Copyright (C) 2003 by Andreas Nicolai                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchprefgeneral.h"
#include "ktouchprefgeneral.moc"

#include <qcheckbox.h>
#include <qlabel.h>
#include <kpushbutton.h>
#include <kfontdialog.h>
#include <klocale.h>

KTouchPrefGeneral::KTouchPrefGeneral(QWidget *parent, const char *name, WFlags f)
 : KTouchPrefGeneralLayout(parent, name, f)
{
    connect(fontChooseBtn, SIGNAL(clicked()), this, SLOT(fontChooseBtnClicked()) );
    fontDisplayLabel->setText( i18n("Have a happy typing time!") );
}

void KTouchPrefGeneral::updateFont(const QFont& font) {
    m_font = font;
    m_font.setPointSize(14);
    fontDisplayLabel->setFont( m_font );
}

void KTouchPrefGeneral::fontChooseBtnClicked() {
    QFont tmpFont(m_font);
    int result = KFontDialog::getFont(tmpFont);
    if (result==KFontDialog::Accepted) {
        updateFont(tmpFont);
        emit fontChanged();
    };
}
