/***************************************************************************
 *   ktouch.cpp                                                            *
 *   ----------                                                            *
 *   Copyright (C) 2000 by Håvard Frøiland, 2004 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouch.h"
#include "ktouch.moc"

// Standard files
#include <algorithm>

// QT Header
#include <qvbox.h>
#include <qsignalmapper.h>
#include <qcheckbox.h>

// KDE Header
#include <klocale.h>
#include <kstatusbar.h>
#include <kfiledialog.h>
#include <kaction.h>
#include <kstandarddirs.h>
#include <kmessagebox.h>
#include <kdebug.h>
#include <kpopupmenu.h>
#include <kconfigdialog.h>
#include <kaction.h>
#include <kcombobox.h>

// Own header files
//#include "ktouchpref.h"
#include "ktouchlecture.h"
#include "ktouchlectureeditor.h"
#include "ktouchstatus.h"
#include "ktouchslideline.h"
#include "ktouchkeyboard.h"
#include "ktouchconfiguration.h"
#include "ktouchtrainer.h"
#include "ktouchstartnewdialog.h"
#include "ktouchstatistics.h"
#include "ktouchprefgenerallayout.h"
#include "ktouchpreftraininglayout.h"
#include "ktouchprefkeyboardlayout.h"
#include "ktouchprefcolorslayout.h"
#include "prefs.h"

KTouch::KTouch()
  : KMainWindow( 0, "KTouch" ),
    m_statusWidget(NULL),
    m_keyboardWidget(NULL)
{
    // NOTE: the dialogs will only be created (via new) when they are actually needed
    //       and only once for the lifetime of the program -> create-on-demand technique

    // Build the training area. The status widget has a fixed vertical size, the slide line and the
    // keyboard grow according to their vertical stretch factors (see last argument in the constructors
    // of QSizePolicy)
    QVBox * mainLayout = new QVBox( this );
    m_statusWidget = new KTouchStatus( mainLayout );
    m_slideLineWidget = new KTouchSlideLine( mainLayout );
    m_slideLineWidget->setSizePolicy( QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding, 0, 1) );
    m_keyboardWidget = new KTouchKeyboard( mainLayout );
    m_keyboardWidget->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding, 0, 3) );
    setCentralWidget(mainLayout);

    // create our trainer, the master object for the training stuff...
    m_trainer = new KTouchTrainer(m_statusWidget, m_slideLineWidget, m_keyboardWidget, &m_lecture);

    // Setup status bar
    statusBar()->show();
    statusBar()->insertItem("", 0, 1);                  // space for the messages
    m_barStatsLabel = new QLabel("",this);
    statusBar()->addWidget(m_barStatsLabel, 0, true);   // for the character statistics / counter

    // read our preferences settings, including session history and such things
    KTouchConfig().readConfiguration(this);
    // and apply them to the widgets
    m_statusWidget->applyPreferences();
    m_slideLineWidget->applyPreferences();
    m_keyboardWidget->applyPreferences(this, true);  // set preferences silently here

	// Setup our actions and connections
    setupActions();
    // create the GUI reading the ui.rc file
    setupGUI();

    // Reload the last used training file.
	m_lecture.loadXML(this, KTouchConfig().m_currentLectureFile);
	updateFontFromLecture();
	// TODO : adjust check mark in quick-select menu

	// If session was restored, the function readProperties() was already called
    if (kapp->isRestored()) {
		kdDebug() << "KTouch() : restoring session..." << endl;
		// TODO : rewrite all the session management stuff
    }
	else {
		kdDebug() << "KTouch() : standard startup..." << endl;
		// A note about safety: In this function there are a lot of things that might go
		// wrong. What happens if the training file can't be found? What if the
		// file cannot be opened or is corrupt? Whatever happens, the function loadXML() 
		// ensures, that there is at least the default mini-level in the lecture
		// so that the training won't crash.
        // If the user doesn't want to restart with his old level, reset it
        if (!Prefs::rememberLevel())
            m_trainer->m_level=0;
        // now let's show the first line of the current level
        m_trainer->goFirstLine();
        // the current training session is already started and the training will start on first keypress
        changeStatusbarMessage( i18n("Starting training session: Waiting for first keypress...") );
        changeStatusbarStats( m_trainer->m_session.m_correctChars,
            m_trainer->m_session.m_totalChars, m_trainer->m_session.m_words);
	}
}
// ----------------------------------------------------------------------------

// Free memory of objects that are not owned by the main KTouch object
KTouch::~KTouch() {
	delete m_trainer;
	m_trainer = NULL;
}
// ----------------------------------------------------------------------------


// ********************
// *** Public slots ***
// ********************

void KTouch::applyPreferences() {
    // ok, just notify our widgets to update themselves, we don't need to know the details...
    m_statusWidget->applyPreferences();
    m_slideLineWidget->applyPreferences();
    m_keyboardWidget->applyPreferences(this, false);  // noisy preferences here
}

void KTouch::keyPressEvent(QKeyEvent *keyEvent) {
    if (keyEvent->text().isEmpty()) return;
    QChar key = keyEvent->text().at(0);
    if (key.isPrint())
        m_trainer->keyPressed(key);
    else if (key==QChar(8))
        m_trainer->backspacePressed();
    else if (key==QChar(13))
        m_trainer->enterPressed();
    else
        return; // unrecognised char -> don't accept it! Maybe the key is for somebody else?
    keyEvent->accept();
}


// *********************************
// *** Protected member function ***
// *********************************

bool KTouch::queryClose() {
	// TODO : if in test mode, ask user whether he really wants to quit the test
	return true;
}

bool KTouch::queryExit() {
    KTouchConfig().writeConfiguration();	// saves preferences
    return true;
}

// *****************************************************
// *** Private slots (implementation of the actions) ***
// *****************************************************

// The action File->Edit lecture...
void KTouch::fileEditLecture() {
	// Create and execute editor
    KTouchLectureEditor dlg(this);
    dlg.startEditor(KTouchConfig().m_currentLectureFile);
	// Reload lecture in case it was modified
	m_lecture.loadXML(this, KTouchConfig().m_currentLectureFile);
	updateFontFromLecture();
}
// ----------------------------------------------------------------------------

void KTouch::fileQuit() {
    kapp->quit();
}
// ----------------------------------------------------------------------------

void KTouch::trainingNewSession() {
    trainingPause();
    KTouchStartNewDialog dlg(this);
    if (dlg.exec()==QDialog::Accepted) {
        m_trainer->startNewTrainingSession(dlg.keepLevel->isChecked());
        m_trainingPause->setEnabled(true);
        m_trainingContinue->setEnabled(false);
    }
    else
        m_trainer->continueTraining();
}
// ----------------------------------------------------------------------------

void KTouch::trainingContinue() {
    m_trainingPause->setEnabled(true);
    m_trainingContinue->setEnabled(false);
    m_trainer->continueTraining();
}
// ----------------------------------------------------------------------------

void KTouch::trainingPause() {
    m_trainingPause->setEnabled(false);
    m_trainingContinue->setEnabled(true);
    m_trainer->pauseTraining();
}
// ----------------------------------------------------------------------------

void KTouch::trainingStatistics() {
    bool sessionRunning = m_trainingPause->isEnabled();
    trainingPause();
	KTouchStatistics dlg(this, m_trainer);
    dlg.exec();
    if (sessionRunning)
        trainingContinue();
}
// ----------------------------------------------------------------------------

void KTouch::optionsPreferences() {
    trainingPause();
	if ( KConfigDialog::showDialog( "settings" ) )	return;
	//KConfigDialog didn't find an instance of this dialog, so lets create it :
	KConfigDialog* dialog = new KConfigDialog( this, "settings",  Prefs::self() );
	KTouchPrefGeneralLayout * m_pageGeneral = new KTouchPrefGeneralLayout(0, "General");
	dialog->addPage(m_pageGeneral, i18n("General Options"), "style");
	KTouchPrefTrainingLayout * m_pageTraining = new KTouchPrefTrainingLayout(0, "Training");
	dialog->addPage(m_pageTraining, i18n("Training Options"), "kalarm");
	KTouchPrefKeyboardLayout * m_pageKeyboard = new KTouchPrefKeyboardLayout(0, "Keyboard");
	dialog->addPage(m_pageKeyboard, i18n("Keyboard Settings"), "keyboard_layout");
	KTouchPrefColorsLayout *m_pageColors = new KTouchPrefColorsLayout(0, "Colors"); 
	dialog->addPage(m_pageColors, i18n("Color Settings"), "package_graphics");
	connect(dialog, SIGNAL(settingsChanged()), this, SLOT(updateSettings()));
	dialog->show();
}
// ----------------------------------------------------------------------------

void KTouch::updateSettings() {
	// TODO : test if this setting has changed
	// This applies a new color scheme for the keyboard
	changeColor(Prefs::colorScheme());
	changeKeyboard(Prefs::layout());
	// not sure if testing is required before applying all that or could that be done in any case?
	m_statusWidget->applyPreferences();
	m_slideLineWidget->applyPreferences();
}
// ----------------------------------------------------------------------------

void KTouch::changeStatusbarMessage(const QString& text) {
    statusBar()->message(text);
}
// ----------------------------------------------------------------------------

void KTouch::changeStatusbarStats(unsigned int correctChars, unsigned int totalChars, unsigned int words) {
    m_barStatsLabel->setText( i18n( "   Correct chars: %1   Total chars: %1   Words: %1   ")
        .arg(correctChars).arg(totalChars).arg(words) );
}
// ----------------------------------------------------------------------------

void KTouch::changeKeyboard(int num) {
    if (static_cast<unsigned int>(num)>=KTouchConfig().m_keyboardFiles.count()) return;
    KTouchConfig().m_currentKeyboardFile = KTouchConfig().m_keyboardFiles[num];
	kdDebug() << "New keyboard layout: " << KTouchConfig().m_currentKeyboardFile << endl;
    Prefs::setLayout(num);
    Prefs::writeConfig();
    m_keyboardLayoutAction->setCurrentItem(Prefs::layout());
    m_keyboardWidget->applyPreferences(this, false);  // noisy, pop up an error if the chosen layout file is corrupt
}
// ----------------------------------------------------------------------------

void KTouch::changeColor(int num) {
    if (static_cast<unsigned int>(num)>=KTouchConfig().m_keyboardColors.count()) return;
    KTouchConfig().m_keyboardColorScheme = num;
    Prefs::setColorScheme(num);
    Prefs::writeConfig();
    m_keyboardWidget->applyPreferences(this, false);
}
// ----------------------------------------------------------------------------

void KTouch::changeLecture(int num) {
    if (static_cast<unsigned int>(num)>=KTouchConfig().m_lectureFiles.count()) return;
    trainingPause();
	KTouchLecture l;
	QString fileName = KTouchConfig().m_lectureFiles[num];
    if (!l.loadXML(this, KURL::fromPathOrURL(fileName)))
        KMessageBox::sorry(0, i18n("Could not find/open the lecture file '%1'.").arg(fileName) );
	else {
		KTouchConfig().m_currentLectureFile = fileName;
		m_lecture = l;
		updateFontFromLecture();
	}
    m_trainer->goFirstLevel();
}
// ----------------------------------------------------------------------------

// *******************************
// *** Private member function ***
// *******************************

// Will be called when this app is restored due to session management.
// This function only stored the temperary data of the last session. All permanent
// settings should be handled by the KTouchConfiguration object.
void KTouch::readProperties(KConfig *config) {
    kdDebug() << "[KTouch::readProperties]  Reading session data..." << endl;
	// TODO : Session management rewrite
    config->setGroup("TrainingState");
	
/*
	// The application is about to be restored due to session management.
    // Let's read all the stuff that was set when the application was terminated (during KDE logout).
    QString session = config->readEntry("Session");
    if (!session.isEmpty())
        m_trainer->m_session = KTouchTrainingSession(session);
    m_trainer->m_level = config->readNumEntry("Level", 0);
    m_trainer->m_line = config->readNumEntry("Line", 0);
    m_currentLectureFile = config->readPathEntry("Lecture");
    m_trainer->readSessionHistory();    // read session history (excluding currently active session)
    // update the trainer object
    m_trainer->m_teacherText = m_lecture.level(m_trainer->m_level).line(m_trainer->m_line);
    m_trainer->m_studentText = config->readEntry("StudentText");
    m_trainer->continueTraining();
    changeStatusbarMessage( i18n("Restarting training session: Waiting for first keypress...") );
    // update the slide line widget
    m_slideLineWidget->setNewText(m_trainer->m_teacherText, m_trainer->m_studentText);
    // update all the other widgets
    m_trainer->updateWidgets();
	// Read training state
    config->setGroup("TrainingState");
    m_currentLectureURL = config->readPathEntry("LectureURL");
    m_trainer->m_level = config->readNumEntry("Level", 0);
*/
}

// Will be called when the app should save its state for session management purposes.
void KTouch::saveProperties(KConfig *config) {
    kdDebug() << "[KTouch::saveProperties]  Saving session data..." << endl;
    // We are going down because of session management (most likely because of
    // KDE logout). Let's save the current status so that we can restore it
    // next logon.

	// TODO : Session management rewrite
    config->setGroup("TrainingState");
/*
    // first write the current lecture URL and the training position
    config->writePathEntry("Lecture", m_currentLectureURL.url());
    config->writeEntry("Level", m_trainer->m_level);
    config->writeEntry("Line", m_trainer->m_line);
    config->writeEntry("StudentText", m_trainer->m_studentText);
    config->writeEntry("Session", m_trainer->m_session.asString() );
	// store the session history so far
    m_trainer->writeSessionHistory();
    
	config->setGroup("TrainingState");
    config->writePathEntry("LectureURL", m_currentLectureURL.url());
    config->writeEntry("Level", m_trainer->m_level);
    // during normal shutdown we finish the session and add it to the session history
    m_trainer->m_sessionHistory.append( m_trainer->m_session );
    m_trainer->writeSessionHistory();
*/
}

void KTouch::setupActions() {
	// *** File menu ***
    KStdAction::quit(this, SLOT(fileQuit()), actionCollection());
    new KAction(i18n("&Edit lecture ..."), 0, this, SLOT(fileEditLecture()), actionCollection(), "file_editlecture");

	// *** Training menu ***
    new KAction(i18n("&Start New Training Session"), "launch", 0,
        this, SLOT(trainingNewSession()), actionCollection(), "training_newsession");
    m_trainingContinue = new KAction(i18n("&Continue Training Session"), "player_play", 0,
        this, SLOT(trainingContinue()), actionCollection(), "training_run");
    m_trainingPause = new KAction(i18n("&Pause Training Session"), "player_pause", 0,
        this, SLOT(trainingPause()), actionCollection(), "training_pause");
    m_trainingContinue->setEnabled(false); // because the training session is running initially
    new KAction(i18n("Show Training S&tatistics"), "frame_chart", 0,
        this, SLOT(trainingStatistics()), actionCollection(), "training_stats");
 
    // Setup menu entries for the training lectures
    m_defaultLectureAction = new KSelectAction(i18n("Default &Lectures"), 0, this, 0, actionCollection(), "default_lectures");
    m_defaultLectureAction->setItems(KTouchConfig().m_lectureTitles);
    m_defaultLectureAction->setCurrentItem(Prefs::layout());  
    connect (m_defaultLectureAction, SIGNAL(activated(int)), this, SLOT(changeLecture(int)));

	// *** Settings menu ***    
    KStdAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
    // Setup menu entries for keyboard layouts    
    m_keyboardLayoutAction= new KSelectAction(i18n("&Keyboard Layouts"), 0, this, 0, actionCollection(), "keyboard_layouts");
    m_keyboardLayoutAction->setItems(KTouchConfig().m_keyboardFiles);
    m_keyboardLayoutAction->setCurrentItem(Prefs::layout());  
    connect (m_keyboardLayoutAction, SIGNAL(activated(int)), this, SLOT(changeKeyboard(int)));

	// Setup menu entries for colour schemes
	m_keyboardColorAction = new KSelectAction(i18n("Keyboards &Color Schemes"), 0, this, 0, actionCollection(), "keyboard_schemes");
    QStringList schemes_list;
    for (unsigned int i=0; i<KTouchConfig().m_keyboardColors.count(); ++i)
		schemes_list.append(KTouchConfig().m_keyboardColors[i].m_name);
    m_keyboardColorAction->setItems(schemes_list);
    m_keyboardColorAction->setCurrentItem(Prefs::colorScheme());  
    connect (m_keyboardColorAction, SIGNAL(activated(int)), this, SLOT(changeColor(int)));
    
	
    // *** Other connections ***

	// connections for the trainer object
    connect( m_trainer, SIGNAL(statusbarMessageChanged(const QString&)), this, SLOT(changeStatusbarMessage(const QString&)) );
    connect( m_trainer, SIGNAL(statusbarStatsChanged(unsigned int, unsigned int, unsigned int)),
             this, SLOT(changeStatusbarStats(unsigned int, unsigned int, unsigned int)) );
}

void KTouch::updateFontFromLecture() {
	// if the lecture requires a font, try this 
	if (!m_lecture.m_fontSuggestions.isEmpty()) {
		QFont f;
		// TODO : multiple font suggestions
		if (f.fromString(m_lecture.m_fontSuggestions))	m_slideLineWidget->setFont(f);
		else if (f.fromString("Courier 10 Pitch")) 		m_slideLineWidget->setFont(f);
	}
}
