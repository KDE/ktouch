/***************************************************************************
 *   ktouchsettings.cpp                                                    *
 *   ------------------                                                    *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchsettings.h"

#include <kconfig.h>
#include <kapplication.h>
#include <kglobalsettings.h>
#include <kglobal.h>
#include <kstandarddirs.h>
#include <kurl.h>
#include <kdebug.h>

KTouchSettings& KTouchConfig() {
    static KTouchSettings theSettings;
    return theSettings;
};

void KTouchSettings::loadSettings() {
    // First we need to find all keyboard layouts
    KStandardDirs *dirs = KGlobal::dirs();
    m_keyboardLayouts = dirs->findAllResources("data","ktouch/*.keyboard");
    if (!m_keyboardLayouts.isEmpty()) {
        // extract the prefixes
        for (QStringList::iterator it=m_keyboardLayouts.begin(); it!=m_keyboardLayouts.end(); ++it) {
            KURL url(*it);
            *it = url.filename().remove(".keyboard");
        };
        // remove the number layout, since this is the necessary default layout and will be
        // added anyway
        QStringList::iterator it = m_keyboardLayouts.find("number");
        if (it!=m_keyboardLayouts.end())
            m_keyboardLayouts.remove(it);
    };
    m_keyboardLayouts.push_front("number");

    // Now lets find the lecture files.
    m_lectureList = dirs->findAllResources("data","ktouch/*.ktouch");
    if (!m_lectureList.isEmpty()) {
        // extract the prefixes
        for (QStringList::iterator it=m_lectureList.begin(); it!=m_lectureList.end(); ++it) {
            KURL url(*it);
            *it = url.filename().remove(".ktouch");
        };
    };

    // now we're reading the configuration
    KConfig *config=kapp->config();

    // read general options
    config->setGroup("General");
    m_errorBeep = config->readBoolEntry("BeepOnError", true);
    m_levelBeep = config->readBoolEntry("SoundOnLevelChange", true);
    QFont defaultFont = QFont("Courier 10 Pitch");
    m_font = config->readFontEntry("Font", &defaultFont);
    m_slideSpeed = config->readNumEntry("Sliding speed", 5);

    // read training options
    config->setGroup("Training");
    m_autoLevelChange = config->readBoolEntry("AutoLevelChange", true);
    m_downCorrectLimit = config->readNumEntry("CorrectLimitDown", 60);
    m_downSpeedLimit = config->readNumEntry("SpeedLimitDown", 60);
    m_upCorrectLimit = config->readNumEntry("CorrectLimitUp", 85);
    m_upSpeedLimit = config->readNumEntry("SpeedLimitUp", 120);
    m_rememberLevel = config->readBoolEntry("Remember level", true);

    // read keyboard settings
    config->setGroup("Keyboard");
    m_keyboardLayout = config->readEntry("Layout", "number");
    // if keyboard layout it not available (e.g. the layout file has been deleted) switch to default
    if (m_keyboardLayouts .contains(m_keyboardLayout)==0)
        m_keyboardLayout="number";
    // switch to 'en' if available
    if (m_keyboardLayout=="number"  &&  (m_keyboardLayouts .contains("en")>0) )
        m_keyboardLayout="en";
    // create some default colour schemes
    createDefaultKeyboardColors();
    m_showAnimation = config->readBoolEntry("ShowAnimation", true);
    // although the keyboard color scheme is selected in the color dialog, it is a property of
    // the keyboard and thus it stays in the "Keybord" group
    m_keyboardColorScheme = config->readNumEntry("Colorscheme", 1);

    // read color options
    config->setGroup("Colors");
    m_useErrorColor = config->readBoolEntry("ColorOnError", true);
    QColor defaultColor(0,00,50);
    m_teacherTextColor = config->readColorEntry("TeacherTextColor", &defaultColor);
    defaultColor.setRgb(190,190,255);
    m_teacherBackground = config->readColorEntry("TeacherBackgroundColor", &defaultColor);
    defaultColor.setRgb(0,50,0);
    m_studentTextColor = config->readColorEntry("StudentTextColor", &defaultColor);
    defaultColor.setRgb(160,255,160);
    m_studentBackground = config->readColorEntry("StudentBackgroundColor", &defaultColor);
    defaultColor.setRgb(255,255,255);
    m_errorTextColor = config->readColorEntry("ErrorTextColor", &defaultColor);
    defaultColor.setRgb(170,0,25);
    m_errorBackground = config->readColorEntry("ErrorBackgroundColor", &defaultColor);
};


void KTouchSettings::saveSettings() {
    KConfig *config=kapp->config();

    // write general options
    config->setGroup("General");
    config->writeEntry("BeepOnError",       m_errorBeep);
    config->writeEntry("SoundOnLevel",      m_levelBeep);
    config->writeEntry("Font",              m_font);
    config->writeEntry("SlidingSpeed",      m_slideSpeed);

    // write training settings
    config->setGroup("Training");
    config->writeEntry("AutoLevelChange",   m_autoLevelChange);
    config->writeEntry("CorrectLimitDown",  m_downCorrectLimit);
    config->writeEntry("SpeedLimitDown",    m_downSpeedLimit);
    config->writeEntry("CorrectLimitUp",    m_upCorrectLimit);
    config->writeEntry("SpeedLimitUp",      m_upSpeedLimit);
    config->writeEntry("RememberLevel",     m_rememberLevel);

    // write keyboard settings
    config->setGroup("Keyboard");
    config->writeEntry("Colorscheme",       m_keyboardColorScheme);
    config->writeEntry("Layout",            m_keyboardLayout);
    config->writeEntry("ShowAnimation",     m_showAnimation);

    // write color settings
    config->setGroup("Colors");
    config->writeEntry("ColorOnError",              m_useErrorColor);
    config->writeEntry("TeacherTextColor",          m_teacherTextColor);
    config->writeEntry("TeacherBackgroundColor",    m_teacherBackground);
    config->writeEntry("StudentTextColor",          m_studentTextColor);
    config->writeEntry("StudentBackgroundColor",    m_studentBackground);
    config->writeEntry("ErrorTextColor",            m_errorTextColor);
    config->writeEntry("ErrorBackgroundColor",      m_errorBackground);
};


void KTouchSettings::createDefaultKeyboardColors() {
    KTouchKeyboardColor color;
    color.m_name = "Black'n White";
    color.m_frame = Qt::black;
    for (int i=0; i<8; ++i)
        color.m_background[i] = Qt::white;
    color.m_text = Qt::black;
    color.m_backgroundH = Qt::black;
    color.m_textH = Qt::white;
    color.m_cBackground = Qt::gray;
    color.m_cText = Qt::black;
    color.m_cBackgroundH = Qt::white;
    color.m_cTextH = Qt::black;
    m_keyboardColors.append(color);
    color.m_name = "Classic";
    color.m_frame = Qt::black;
    color.m_background[0] = QColor(255,238,  7);     color.m_background[4] = QColor(247,138,247);
    color.m_background[1] = QColor( 14,164,239);     color.m_background[5] = QColor(158,255,155);
    color.m_background[2] = QColor(158,255,155);     color.m_background[6] = QColor( 14,164,239);
    color.m_background[3] = QColor(252,138,138);     color.m_background[7] = QColor(255,238,  7);
    color.m_text = Qt::black;
    color.m_backgroundH = Qt::darkBlue;
    color.m_textH = Qt::white;
    color.m_cBackground = Qt::gray;
    color.m_cText = Qt::black;
    color.m_cBackgroundH = Qt::white;
    color.m_cTextH = Qt::black;
    m_keyboardColors.append(color);
    color.m_name = "Deep blue";
    color.m_frame = QColor(220,220,220);
    color.m_background[0] = QColor(  0, 39, 80);     color.m_background[4] = QColor( 24, 19, 72);
    color.m_background[1] = QColor( 39, 59,127);     color.m_background[5] = QColor(  8, 44,124);
    color.m_background[2] = QColor(  4, 39, 53);     color.m_background[6] = QColor( 10, 82,158);
    color.m_background[3] = QColor( 40, 32,121);     color.m_background[7] = QColor( 43, 60,124);
    color.m_text = Qt::white;
    color.m_backgroundH = QColor(125,180,255);
    color.m_textH = Qt::darkBlue;
    color.m_cBackground = Qt::black;
    color.m_cText = Qt::white;
    color.m_cBackgroundH = QColor(111,121,73);
    color.m_cTextH = Qt::white;
    m_keyboardColors.append(color);
};
