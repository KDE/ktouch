/***************************************************************************
 *   ktouchpref.cpp                                                        *
 *   --------------                                                        *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchpref.h"
#include "ktouchpref.moc"

// QT Header
#include <qlayout.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qcolor.h>
#include <qcheckbox.h>
#include <qframe.h>
#include <qslider.h>

// KDE Header
#include <klocale.h>
#include <kiconloader.h>
#include <kglobal.h>
#include <kcolorbutton.h>
#include <kpushbutton.h>
#include <knuminput.h>
#include <kcombobox.h>
#include <kmessagebox.h>
#include <kglobalsettings.h>
#include <kdebug.h>

// KTouch Header
#include "ktouchprefgeneral.h"
#include "ktouchprefkeyboard.h"
#include "ktouchpreftraining.h"
#include "ktouchsettings.h"

KTouchPref::KTouchPref() : KDialogBase(IconList, i18n("KTouch Preferences"), Default|Ok|Apply|Cancel, Ok) {
    // adding page "General options"
    QFrame *frame = addPage(i18n("General"), i18n("General options"),
        KGlobal::iconLoader()->loadIcon("style",KIcon::Panel,0,false) );
    QVBoxLayout *frameLayout = new QVBoxLayout( frame, 0, 0 );
    m_pageGeneral = new KTouchPrefGeneral(frame);
    frameLayout->addWidget(m_pageGeneral);
    // adding page "Keyboard settings"
    frame = addPage(i18n("Keyboard"), i18n("Keyboard settings"),
        KGlobal::iconLoader()->loadIcon("keyboard_layout",KIcon::Panel,0,false) );
    frameLayout = new QVBoxLayout( frame, 0, 0 );
    m_pageKeyboard = new KTouchPrefKeyboard(frame);
    frameLayout->addWidget(m_pageKeyboard);
    // adding page "Training options"
    frame = addPage(i18n("Training"), i18n("Training options"),
        KGlobal::iconLoader()->loadIcon("kalarm",KIcon::Panel,0,false) );
    frameLayout = new QVBoxLayout( frame, 0, 0 );
    m_pageTraining = new KTouchPrefTraining(frame);
    frameLayout->addWidget(m_pageTraining);

    // connect all options/settings to the setModified slot
    connect(m_pageGeneral->errorBeepCheck, SIGNAL(toggled(bool)), this, SLOT(setModified()) );
    connect(m_pageGeneral->levelBeepCheck, SIGNAL(toggled(bool)), this, SLOT(setModified()) );
    connect(m_pageGeneral->errorColorCheck, SIGNAL(toggled(bool)), this, SLOT(setModified()) );
    connect(m_pageGeneral->errorColorBtn, SIGNAL(changed(const QColor&)), this, SLOT(setModified()) );
    connect(m_pageGeneral, SIGNAL(fontChanged()), this, SLOT(setModified()) );
    connect(m_pageGeneral->slidingSpeedSlider, SIGNAL(valueChanged(int)), this, SLOT(setModified()) );
    connect(m_pageKeyboard->colorSchemeCombo, SIGNAL(activated(int)), this, SLOT(setModified()) );
    connect(m_pageKeyboard->keyboardLayoutCombo, SIGNAL(activated(int)), this, SLOT(setModified()) );
    connect(m_pageTraining->levelChangeCheck, SIGNAL(toggled(bool)), this, SLOT(setModified()) );
    connect(m_pageTraining->downCorrectLimit, SIGNAL(valueChanged(int)), this, SLOT(setModified()) );
    connect(m_pageTraining->downSpeedLimit, SIGNAL(valueChanged(int)), this, SLOT(setModified()) );
    connect(m_pageTraining->upCorrectLimit, SIGNAL(valueChanged(int)), this, SLOT(setModified()) );
    connect(m_pageTraining->upSpeedLimit, SIGNAL(valueChanged(int)), this, SLOT(setModified()) );
    connect(m_pageTraining->rememberLevelCheck, SIGNAL(toggled(bool)), this, SLOT(setModified()) );

    // and transfer all data from the settings object into the dialog
    update(true);
}


void KTouchPref::setDefaults() {
    // general page
    m_pageGeneral->errorBeepCheck->setChecked(true);
    m_pageGeneral->levelBeepCheck->setChecked(true);
    m_pageGeneral->errorColorCheck->setChecked(true);
    m_pageGeneral->errorColorToggled(true);
    m_pageGeneral->errorColorBtn->setColor( QColor(170,0,25) );
    m_pageGeneral->updateFont( KGlobalSettings::generalFont() );
    m_pageGeneral->slidingSpeedSlider->setValue(5);
    // keyboard page
    m_pageKeyboard->colorSchemeCombo->setCurrentItem(1); // classic scheme
    m_pageKeyboard->keyboardLayoutCombo->setCurrentItem(0);
    // training page
    m_pageTraining->levelChangeCheck->setChecked(true);
    m_pageTraining->rememberLevelCheck->setChecked(true);
    m_pageTraining->downCorrectLimit->setValue(80);
    m_pageTraining->downSpeedLimit->setValue(70);
    m_pageTraining->upCorrectLimit->setValue(95);
    m_pageTraining->upSpeedLimit->setValue(120);
    setModified();
}


void KTouchPref::update(bool toDialog) {
    if (toDialog) {
        // general options page
        m_pageGeneral->errorBeepCheck->setChecked(KTouchConfig().m_errorBeep);
        m_pageGeneral->levelBeepCheck->setChecked(KTouchConfig().m_levelBeep);
        m_pageGeneral->errorColorCheck->setChecked(KTouchConfig().m_useErrorColor);
        m_pageGeneral->errorColorToggled(KTouchConfig().m_useErrorColor);
        m_pageGeneral->errorColorBtn->setColor( KTouchConfig().m_errorColor );
        m_pageGeneral->updateFont(  KTouchConfig().m_font );
        m_pageGeneral->slidingSpeedSlider->setValue( KTouchConfig().m_slideSpeed );
        // keyboard page
        m_pageKeyboard->colorSchemeCombo->setCurrentItem(KTouchConfig().m_keyboardColorScheme);
        // set currently selected keyboard layout
        int itemIndex = KTouchConfig().m_keyboardLayouts.findIndex(KTouchConfig().m_keyboardLayout);
        if (itemIndex < m_pageKeyboard->keyboardLayoutCombo->count())
            m_pageKeyboard->keyboardLayoutCombo->setCurrentItem(itemIndex);
        else
            m_pageKeyboard->keyboardLayoutCombo->setCurrentItem(0); // select "number" by default
        // training page
        m_pageTraining->levelChangeCheck->setChecked(KTouchConfig().m_autoLevelChange);
        m_pageTraining->levelChangeToggled( KTouchConfig().m_autoLevelChange );
        m_pageTraining->downCorrectLimit->setValue(KTouchConfig().m_downCorrectLimit);
        m_pageTraining->downSpeedLimit->setValue(KTouchConfig().m_downSpeedLimit);
        m_pageTraining->upCorrectLimit->setValue(KTouchConfig().m_upCorrectLimit);
        m_pageTraining->upSpeedLimit->setValue(KTouchConfig().m_upSpeedLimit);
        m_pageTraining->rememberLevelCheck->setChecked(KTouchConfig().m_rememberLevel);
    }
    else {
        // general options page
        KTouchConfig().m_errorBeep          = m_pageGeneral->errorBeepCheck->isChecked();
        KTouchConfig().m_levelBeep          = m_pageGeneral->levelBeepCheck->isChecked();
        KTouchConfig().m_useErrorColor      = m_pageGeneral->errorColorCheck->isChecked();
        KTouchConfig().m_errorColor         = m_pageGeneral->errorColorBtn->color();
        KTouchConfig().m_font               = m_pageGeneral->m_font;
        KTouchConfig().m_slideSpeed         = m_pageGeneral->slidingSpeedSlider->value();
        // keyboard page
        KTouchConfig().m_keyboardColorScheme = m_pageKeyboard->colorSchemeCombo->currentItem();
        KTouchConfig().m_keyboardLayout      = m_pageKeyboard->keyboardLayoutCombo->currentText();
        // training page
        KTouchConfig().m_autoLevelChange    = m_pageTraining->levelChangeCheck->isChecked();
        KTouchConfig().m_downCorrectLimit   = m_pageTraining->downCorrectLimit->value();
        KTouchConfig().m_downSpeedLimit     = m_pageTraining->downSpeedLimit->value();
        KTouchConfig().m_upCorrectLimit     = m_pageTraining->upCorrectLimit->value();
        KTouchConfig().m_upSpeedLimit       = m_pageTraining->upSpeedLimit->value();
        KTouchConfig().m_rememberLevel      = m_pageTraining->rememberLevelCheck->isChecked();
        KTouchConfig().saveSettings();
    };
    enableButtonApply(false);   // Disable apply button
};


void KTouchPref::slotDefault() {
    if (KMessageBox::warningContinueCancel(this, i18n("This will set the default options "
        "in ALL pages of the preferences dialog! Continue?"), i18n("Set default options?"),
        i18n("Set defaults"))==KMessageBox::Continue)
    {
        setDefaults();
    }
};


void KTouchPref::slotApply() {
    update(false);              // transfer data from dialog into settings object
    emit applyPreferences();    // emit signal to update widgets
};


void KTouchPref::setModified() {
    enableButtonApply(true);    // simply turn on the apply button
};

