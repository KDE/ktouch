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
#include <qcheckbox.h>
#include <qslider.h>

// KDE Header
#include <klocale.h>
#include <kiconloader.h>
#include <kcolorbutton.h>
#include <knuminput.h>
#include <kcombobox.h>
#include <kmessagebox.h>
#include <kdebug.h>

// KTouch Header
#include "ktouchprefgeneral.h"
#include "ktouchpreftraining.h"
#include "ktouchprefkeyboard.h"
#include "ktouchprefcolors.h"
#include "ktouchsettings.h"

KTouchPref::KTouchPref() : KDialogBase(IconList, i18n("KTouch Preferences"), Default|Ok|Apply|Cancel, Ok) {
    // adding page "General options"
    QFrame *frame = addPage(i18n("General"), i18n("General Options"),
        KGlobal::iconLoader()->loadIcon("style",KIcon::Panel,0,false) );
    QVBoxLayout *frameLayout = new QVBoxLayout( frame, 0, 0 );
    m_pageGeneral = new KTouchPrefGeneral(frame);
    frameLayout->addWidget(m_pageGeneral);
    // adding page "Training options"
    frame = addPage(i18n("Training"), i18n("Training Options"),
        KGlobal::iconLoader()->loadIcon("kalarm",KIcon::Panel,0,false) );
    frameLayout = new QVBoxLayout( frame, 0, 0 );
    m_pageTraining = new KTouchPrefTraining(frame);
    frameLayout->addWidget(m_pageTraining);
    // adding page "Keyboard settings"
    frame = addPage(i18n("Keyboard"), i18n("Keyboard Settings"),
        KGlobal::iconLoader()->loadIcon("keyboard_layout",KIcon::Panel,0,false) );
    frameLayout = new QVBoxLayout( frame, 0, 0 );
    m_pageKeyboard = new KTouchPrefKeyboard(frame);
    frameLayout->addWidget(m_pageKeyboard);
    // adding page "Color settings"
    frame = addPage(i18n("Colors"), i18n("Color Settings"),
        KGlobal::iconLoader()->loadIcon("package_graphics",KIcon::Panel,0,false) );
    frameLayout = new QVBoxLayout( frame, 0, 0 );
    m_pageColors = new KTouchPrefColors(frame);
    frameLayout->addWidget(m_pageColors);

    // connect all options/settings to the setModified slot
    connect(m_pageGeneral->errorBeepCheck, SIGNAL(toggled(bool)), this, SLOT(setModified()) );
    connect(m_pageGeneral->levelBeepCheck, SIGNAL(toggled(bool)), this, SLOT(setModified()) );
    connect(m_pageGeneral, SIGNAL(fontChanged()), this, SLOT(setModified()) );
    connect(m_pageGeneral->slidingSpeedSlider, SIGNAL(valueChanged(int)), this, SLOT(setModified()) );
    connect(m_pageTraining->levelChangeCheck, SIGNAL(toggled(bool)), this, SLOT(setModified()) );
    connect(m_pageTraining->downCorrectLimit, SIGNAL(valueChanged(int)), this, SLOT(setModified()) );
    connect(m_pageTraining->downSpeedLimit, SIGNAL(valueChanged(int)), this, SLOT(setModified()) );
    connect(m_pageTraining->upCorrectLimit, SIGNAL(valueChanged(int)), this, SLOT(setModified()) );
    connect(m_pageTraining->upSpeedLimit, SIGNAL(valueChanged(int)), this, SLOT(setModified()) );
    connect(m_pageTraining->rememberLevelCheck, SIGNAL(toggled(bool)), this, SLOT(setModified()) );
    connect(m_pageKeyboard->keyboardLayoutCombo, SIGNAL(activated(int)), this, SLOT(setModified()) );
    connect(m_pageKeyboard->showAnimationCheckBox, SIGNAL(toggled(bool)), this, SLOT(setModified()) );
    connect(m_pageKeyboard, SIGNAL(fontChanged()), this, SLOT(setModified()) );
    connect(m_pageColors->errorColorCheck, SIGNAL(toggled(bool)), this, SLOT(setModified()) );
    connect(m_pageColors->teacherTextBtn, SIGNAL(changed(const QColor&)), this, SLOT(setModified()) );
    connect(m_pageColors->teacherBackgroundBtn, SIGNAL(changed(const QColor&)), this, SLOT(setModified()) );
    connect(m_pageColors->studentTextBtn, SIGNAL(changed(const QColor&)), this, SLOT(setModified()) );
    connect(m_pageColors->studentBackgroundBtn, SIGNAL(changed(const QColor&)), this, SLOT(setModified()) );
    connect(m_pageColors->errorTextBtn, SIGNAL(changed(const QColor&)), this, SLOT(setModified()) );
    connect(m_pageColors->errorBackgroundBtn, SIGNAL(changed(const QColor&)), this, SLOT(setModified()) );
    connect(m_pageColors->keyboardColorCombo, SIGNAL(activated(int)), this, SLOT(setModified()) );

    // and transfer all data from the settings object into the dialog
    update(true);
}


void KTouchPref::setDefaults() {
    // general page
    m_pageGeneral->errorBeepCheck->setChecked(true);
    m_pageGeneral->levelBeepCheck->setChecked(true);
    m_pageGeneral->updateFont(KGlobalSettings::generalFont());
    m_pageGeneral->slidingSpeedSlider->setValue(5);
    // training page
    m_pageTraining->levelChangeCheck->setChecked(true);
    m_pageTraining->rememberLevelCheck->setChecked(true);
    m_pageTraining->downCorrectLimit->setValue(60);
    m_pageTraining->downSpeedLimit->setValue(60);
    m_pageTraining->upCorrectLimit->setValue(85);
    m_pageTraining->upSpeedLimit->setValue(120);
    // keyboard page
    m_pageKeyboard->keyboardLayoutCombo->setCurrentItem(0);
    m_pageKeyboard->showAnimationCheckBox->setChecked(true);
    m_pageKeyboard->updateFont(KGlobalSettings::generalFont());
    // colors page
    m_pageColors->errorColorCheck->setChecked( true );
    m_pageColors->errorColorCheckToggled( true );
    m_pageColors->teacherTextBtn->setColor( QColor(0,0,50) );
    m_pageColors->teacherBackgroundBtn->setColor( QColor(190,190,255) );
    m_pageColors->studentTextBtn->setColor( QColor(0,50,0) );
    m_pageColors->studentBackgroundBtn->setColor( QColor(160,255, 160) );
    m_pageColors->errorTextBtn->setColor( Qt::white );
    m_pageColors->errorBackgroundBtn->setColor( QColor(170,0,25) );
    m_pageColors->keyboardColorCombo->setCurrentItem( 1 );
    setModified();
}


void KTouchPref::update(bool toDialog) {
    if (toDialog) {
        // general options page
        m_pageGeneral->errorBeepCheck->setChecked(KTouchConfig().m_errorBeep);
        m_pageGeneral->levelBeepCheck->setChecked(KTouchConfig().m_levelBeep);
        m_pageGeneral->updateFont(  KTouchConfig().m_font );
        m_pageGeneral->slidingSpeedSlider->setValue( KTouchConfig().m_slideSpeed );
        // training page
        m_pageTraining->levelChangeCheck->setChecked(KTouchConfig().m_autoLevelChange);
        m_pageTraining->levelChangeToggled( KTouchConfig().m_autoLevelChange );
        m_pageTraining->downCorrectLimit->setValue(KTouchConfig().m_downCorrectLimit);
        m_pageTraining->downSpeedLimit->setValue(KTouchConfig().m_downSpeedLimit);
        m_pageTraining->upCorrectLimit->setValue(KTouchConfig().m_upCorrectLimit);
        m_pageTraining->upSpeedLimit->setValue(KTouchConfig().m_upSpeedLimit);
        m_pageTraining->rememberLevelCheck->setChecked(KTouchConfig().m_rememberLevel);
        // keyboard page
        int itemIndex = KTouchConfig().m_keyboardLayouts.findIndex(KTouchConfig().m_keyboardLayout);
        if (itemIndex < m_pageKeyboard->keyboardLayoutCombo->count())
            m_pageKeyboard->keyboardLayoutCombo->setCurrentItem(itemIndex);
        else
            m_pageKeyboard->keyboardLayoutCombo->setCurrentItem(0); // select "number" by default
        m_pageKeyboard->showAnimationCheckBox->setChecked(KTouchConfig().m_showAnimation);
        m_pageKeyboard->updateFont(  KTouchConfig().m_keyboardFont );
        // colors page
        m_pageColors->errorColorCheck->setChecked(KTouchConfig().m_useErrorColor);
        m_pageColors->errorColorCheckToggled(KTouchConfig().m_useErrorColor);
        m_pageColors->teacherTextBtn->setColor( KTouchConfig().m_teacherTextColor );
        m_pageColors->teacherBackgroundBtn->setColor( KTouchConfig().m_teacherBackground );
        m_pageColors->studentTextBtn->setColor( KTouchConfig().m_studentTextColor );
        m_pageColors->studentBackgroundBtn->setColor( KTouchConfig().m_studentBackground );
        m_pageColors->errorTextBtn->setColor( KTouchConfig().m_errorTextColor );
        m_pageColors->errorBackgroundBtn->setColor( KTouchConfig().m_errorBackground );
        m_pageColors->keyboardColorCombo->setCurrentItem(KTouchConfig().m_keyboardColorScheme);
    }
    else {
        // general options page
        KTouchConfig().m_errorBeep          = m_pageGeneral->errorBeepCheck->isChecked();
        KTouchConfig().m_levelBeep          = m_pageGeneral->levelBeepCheck->isChecked();
        KTouchConfig().m_font               = m_pageGeneral->m_font;
        KTouchConfig().m_slideSpeed         = m_pageGeneral->slidingSpeedSlider->value();
        // training page
        KTouchConfig().m_autoLevelChange    = m_pageTraining->levelChangeCheck->isChecked();
        KTouchConfig().m_downCorrectLimit   = m_pageTraining->downCorrectLimit->value();
        KTouchConfig().m_downSpeedLimit     = m_pageTraining->downSpeedLimit->value();
        KTouchConfig().m_upCorrectLimit     = m_pageTraining->upCorrectLimit->value();
        KTouchConfig().m_upSpeedLimit       = m_pageTraining->upSpeedLimit->value();
        KTouchConfig().m_rememberLevel      = m_pageTraining->rememberLevelCheck->isChecked();
        KTouchConfig().saveSettings();
        // keyboard page
        KTouchConfig().m_keyboardLayout      = m_pageKeyboard->keyboardLayoutCombo->currentText();
        KTouchConfig().m_showAnimation       = m_pageKeyboard->showAnimationCheckBox->isChecked();
        KTouchConfig().m_keyboardFont        = m_pageKeyboard->m_font;
        // colors page
        KTouchConfig().m_useErrorColor       = m_pageColors->errorColorCheck->isChecked();
        KTouchConfig().m_teacherTextColor    = m_pageColors->teacherTextBtn->color();
        KTouchConfig().m_teacherBackground   = m_pageColors->teacherBackgroundBtn->color();
        KTouchConfig().m_studentTextColor    = m_pageColors->studentTextBtn->color();
        KTouchConfig().m_studentBackground   = m_pageColors->studentBackgroundBtn->color();
        KTouchConfig().m_errorTextColor      = m_pageColors->errorTextBtn->color();
        KTouchConfig().m_errorBackground     = m_pageColors->errorBackgroundBtn->color();
        KTouchConfig().m_keyboardColorScheme = m_pageColors->keyboardColorCombo->currentItem();
    };
    enableButtonApply(false);   // Disable apply button
}


void KTouchPref::slotDefault() {
    if (KMessageBox::warningContinueCancel(this, i18n("This will set the default options "
        "in ALL pages of the preferences dialog; continue?"), i18n("Set default options?"),
        i18n("Set defaults"))==KMessageBox::Continue)
    {
        setDefaults();
    }
}


void KTouchPref::slotApply() {
    update(false);              // transfer data from dialog into settings object
    emit applyPreferences();    // emit signal to update widgets
}


void KTouchPref::setModified() {
    enableButtonApply(true);    // simply turn on the apply button
}

