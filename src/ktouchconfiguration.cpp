/***************************************************************************
 *   ktouchconfiguration.cpp                                               *
 *   -----------------------                                               *
 *   Copyright (C) 2000 by Håvard Frøiland, 2004 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchconfiguration.h"

#include <kconfig.h>
#include <kapplication.h>
#include <kglobalsettings.h>
#include <kglobal.h>
#include <kstandarddirs.h>
#include <kurl.h>
#include <kdebug.h>
#include <klocale.h>
#include "prefs.h"
#include "ktouchlecture.h"

KTouchConfiguration& KTouchConfig() {
    static KTouchConfiguration theConfig;
    return theConfig;
}

void KTouchConfiguration::readConfiguration(QWidget * window) {
    updateFileLists(window);  // create lists with default lecture/keyboard/colour scheme files

    // now we're reading the configuration
    KConfig *config=kapp->config();

    // read general options
    // TODO : replace directly the values by the Prefs:: functions in the files where they are used
    QFont defaultFont = KGlobalSettings::generalFont();
    m_font = config->readFontEntry("Font", &defaultFont);

    // read training options
    config->setGroup("Training");
	// TODO : look up the default english lecture in the m_lectureFiles string list
	QString default_lecture = "english_default";
	if (m_lectureFiles.count() > 0)  default_lecture = m_lectureFiles[0];
	m_currentLectureFile = config->readEntry("CurrentLectureFile", default_lecture);
    // read keyboard settings
    config->setGroup("Keyboard");
    m_keyboardFont = config->readFontEntry("KeyboardFont", &defaultFont);
	
	QString default_keyboard = "number.keyboard";
	// look up the default english keyboard file in the m_keyboardFiles string list
	QStringList::iterator it = m_keyboardFiles.begin();
	while (it != m_keyboardFiles.end()   &&   (*it).find("en.keyboard") == -1)  ++it;
	if (it != m_keyboardFiles.end())   default_keyboard = *it;
    m_currentKeyboardFile = config->readEntry("CurrentKeyboardFile", default_keyboard);
    // if keyboard layout it not available (e.g. the layout file has been deleted) switch to default
    if (m_keyboardFiles.contains(m_currentKeyboardFile)==0)
        m_currentKeyboardFile="number.keyboard";
    
/* 
    Commented code from Anne-Marie - we need the file names here
	
	m_keyboardLayout = m_keyboardLayouts[Prefs::layout()];
	// if keyboard layout it not available (e.g. the layout file has been deleted) switch to default
	if (m_keyboardLayouts .contains(m_keyboardLayout)==0)
		m_keyboardLayout="number";
	// switch to user's KDE langugae or 'en' if available
	if (m_keyboardLayout=="number"  &&  (m_keyboardLayouts .contains("en")>0) )
		m_keyboardLayout="en";
	Prefs::setLayout(m_keyboardLayouts.findIndex(m_keyboardLayout));
*/

	// create some default colour schemes
	createDefaultKeyboardColors();
	// although the keyboard color scheme is selected in the color dialog, it is a property of
	// the keyboard and thus it stays in the "Keybord" group
	m_keyboardColorScheme = Prefs::colorScheme();
}


void KTouchConfiguration::writeConfiguration() {
    KConfig *config=kapp->config();

    // write general options
    config->setGroup("General");
    config->writeEntry("GlobalFont",			m_font);
    // write training settings
    config->setGroup("Training");
	config->writeEntry("CurrentLectureFile", 	m_currentLectureFile);
    // write keyboard settings
    config->setGroup("Keyboard");
    config->writeEntry("CurrentKeyboardFile",   m_currentKeyboardFile);
    config->writeEntry("KeyboardFont",      	m_keyboardFont);

/*    
	TODO : sort and remove old configuration settings

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
    config->writeEntry("KeyboardFont",      m_keyboardFont);

    // write color settings
    config->setGroup("Colors");
    config->writeEntry("ColorOnError",              m_useErrorColor);
    config->writeEntry("TeacherTextColor",          m_teacherTextColor);
    config->writeEntry("TeacherBackgroundColor",    m_teacherBackground);
    config->writeEntry("StudentTextColor",          m_studentTextColor);
    config->writeEntry("StudentBackgroundColor",    m_studentBackground);
    config->writeEntry("ErrorTextColor",            m_errorTextColor);
    config->writeEntry("ErrorBackgroundColor",      m_errorBackground);
*/    
}

void KTouchConfiguration::updateFileLists(QWidget * window) {
    KStandardDirs *dirs = KGlobal::dirs();
	
    // first search for all installed keyboard files
	// TODO : search in i18n() directories
    QStringList keyboardFiles = dirs->findAllResources("data","ktouch/*.keyboard");
	// TODO : extract titles from keyboard files and store them in the m_keyboardTitles string list
	m_keyboardFiles = keyboardFiles;

    // remove the number layout, since this is the necessary default layout and will be
    // added anyway
    QStringList::iterator it = m_keyboardFiles.find("number.keyboard");
	if (it!=m_keyboardFiles.end())		m_keyboardFiles.remove(it);
    m_keyboardFiles.push_front("number.ktouch");
    m_keyboardTitles.push_front(i18n("Keypad/Number block"));

    // Now lets find the lecture files.
	// TODO : search in i18n() directories
    QStringList lectureFiles = dirs->findAllResources("data","ktouch/*.ktouch.xml");
	// Now extract the titles of the lecture files and populate the string lists used in the program
	m_lectureFiles.clear();
	m_lectureTitles.clear();
    if (!lectureFiles.isEmpty()) {
        // extract the prefixes
        for (QStringList::iterator it=lectureFiles.begin(); it!=lectureFiles.end(); ++it) {
            KURL url(*it);
			KTouchLecture l;
			// only add lecture if we can actually load it
			if (l.loadXML(window, url)) {
				// since we could read the lecture, we remember the URL
				m_lectureFiles.push_back(*it);	
				// store the title of the lecture
				if (l.m_title.isEmpty())
					m_lectureTitles.push_back(i18n("untitled lecture") + " - (" + url.fileName() + ")");
            	else
					m_lectureTitles.push_back(l.m_title);
			}
        }
    }

	// Now find predefined files with colour schemes
    QStringList colour_schemes = dirs->findAllResources("data","ktouch/*.colour_scheme");
	// TODO : read in colour schemes and populate QValueList<KTouchColorScheme>
}

void KTouchConfiguration::createDefaultKeyboardColors() {
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
    m_keyboardColors.push_back(color);

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
    m_keyboardColors.push_back(color);

    color.m_name = "Deep Blue";
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
    m_keyboardColors.push_back(color);

    color.m_name = "Stripy";
    color.m_frame = Qt::black;
    for (int i=0; i<8; i=i+2)
        color.m_background[i] = QColor( 39, 70, 127);
		for (int i=1; i<8; i=i+2)
        color.m_background[i] = Qt::darkGray;
    color.m_text = Qt::black;
    color.m_backgroundH = QColor( 39, 70, 227);
    color.m_textH = Qt::white;
    color.m_cBackground = Qt::gray;
    color.m_cText = Qt::black;
    color.m_cBackgroundH = QColor( 39, 70, 227);
    color.m_cTextH = Qt::black;
    m_keyboardColors.push_back(color);
}

