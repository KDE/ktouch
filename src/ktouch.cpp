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
#include <qlabel.h>

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
#include <kfontrequester.h>
#include <knuminput.h>

// Own header files
//#include "ktouchpref.h"
#include "ktouchlecture.h"
#include "ktouchlectureeditor.h"
#include "ktouchstatus.h"
#include "ktouchslideline.h"
#include "ktouchkeyboard.h"
#include "ktouchtrainer.h"
#include "ktouchstartnewdialog.h"
#include "ktouchstatistics.h"
#include "ktouchprefgenerallayout.h"
#include "ktouchpreftraininglayout.h"
#include "ktouchprefkeyboardlayout.h"
#include "ktouchprefcolorslayout.h"
#include "prefs.h"

KTouch * KTouchPtr = NULL;

KTouch::KTouch()
  : KMainWindow( 0, "KTouch" ),
    m_statusWidget(NULL),
    m_keyboardWidget(NULL),
	m_trainer(NULL)
{
	KTouchPtr = this;
	// General initialization of the program, common for all start modes
	init();
	// Setup our actions and connections
    setupActions();
    // create the GUI reading the ui.rc file
    setupGUI();
	// Init a training session
	initTrainingSession();
	
	// If session was restored, the function readProperties() was already called
    if (kapp->isRestored()) {
		kdDebug() << "KTouch() : restoring session..." << endl;
		// TODO : rewrite all the session management stuff
		//        for now we just do the same as for the standard startup
		m_lecture.loadXML(this, Prefs::currentLectureFile() );
		updateFontFromLecture();
		updateLectureActionCheck();
		m_trainer->m_level = 0;
		m_trainer->goFirstLine();
        changeStatusbarMessage( i18n("Starting training session: Waiting for first keypress...") );
        changeStatusbarStats( m_trainer->m_session.m_correctChars,
            m_trainer->m_session.m_totalChars, m_trainer->m_session.m_words);
    }
	else {
		kdDebug() << "[KTouch::KTouch]  starting standard training..." << endl;
		// A note about safety: In this function there are a lot of things that might go
		// wrong. What happens if the training file can't be found? What if the
		// file cannot be opened or is corrupt? Whatever happens, the function loadXML() 
		// ensures, that there is at least the default mini-level in the lecture
		// so that the training won't crash.
        
	    // Reload the last used training file.
		if (!m_lecture.loadXML(this, Prefs::currentLectureFile() )) {
			Prefs::setCurrentLectureFile(QString::null);
			m_defaultLectureAction->setCurrentItem(-1);
		}
		else {
			updateFontFromLecture();
			// adjust check marks in quick-select menus
			updateLectureActionCheck();
			kdDebug() << "[KTouch::KTouch]  lecture file = " << Prefs::currentLectureFile() << endl;
		}
		
		// Adjust check mark for the keyboard file
		int num = 0;
		QStringList::iterator it = m_keyboardFiles.begin();
		QString fname = Prefs::currentKeyboardFile(); 
		while (it != m_keyboardFiles.end()   &&   (*it).find(fname) == -1) {
			++it;
			++num;
		}
		if (it == m_keyboardFiles.end())	m_keyboardLayoutAction->setCurrentItem(-1);
		else					    		m_keyboardLayoutAction->setCurrentItem(num);

		// If the user doesn't want to restart with his old level, start from 0 (level 1)
        if (!Prefs::rememberLevel())	m_trainer->m_level = 0;
		else							m_trainer->m_level = Prefs::currentTrainingLevel();
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
	// This applies a new color scheme for the keyboard and also updates all other
	// changes for the keyboard widget
	changeColor(Prefs::colorScheme());
	m_slideLineWidget->applyPreferences();
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

void KTouch::configOverrideLectureFontToggled(bool on) {
	if (on) {
		m_pageGeneral->fontTextLabel->setEnabled(true);
		m_pageGeneral->kcfg_Font->setEnabled(true);
	}
	else {
		m_pageGeneral->fontTextLabel->setEnabled(false);
		m_pageGeneral->kcfg_Font->setEnabled(false);
	}	
}

void KTouch::configOverrideKeyboardFontToggled(bool on) {
	if (on) {
		m_pageKeyboard->textLabel1->setEnabled(true);
		m_pageKeyboard->kcfg_KeyboardFont->setEnabled(true);
	}
	else {
		m_pageKeyboard->textLabel1->setEnabled(false);
		m_pageKeyboard->kcfg_KeyboardFont->setEnabled(false);
	}	
}

void KTouch::configAutoLevelChangeToggled(bool on) {
	if (on) {
		m_pageTraining->l1->setEnabled(true);
		m_pageTraining->l2->setEnabled(true);
		m_pageTraining->l3->setEnabled(true);
		m_pageTraining->l4->setEnabled(true);
		m_pageTraining->l5->setEnabled(true);
		m_pageTraining->l6->setEnabled(true);
		m_pageTraining->l7->setEnabled(true);
		m_pageTraining->l8->setEnabled(true);
		m_pageTraining->l9->setEnabled(true);
		m_pageTraining->l10->setEnabled(true);
		m_pageTraining->kcfg_UpSpeedLimit->setEnabled(true);
		m_pageTraining->kcfg_UpCorrectLimit->setEnabled(true);
		m_pageTraining->kcfg_DownSpeedLimit->setEnabled(true);
		m_pageTraining->kcfg_DownCorrectLimit->setEnabled(true);
	}
	else {
		m_pageTraining->l1->setEnabled(false);
		m_pageTraining->l2->setEnabled(false);
		m_pageTraining->l3->setEnabled(false);
		m_pageTraining->l4->setEnabled(false);
		m_pageTraining->l5->setEnabled(false);
		m_pageTraining->l6->setEnabled(false);
		m_pageTraining->l7->setEnabled(false);
		m_pageTraining->l8->setEnabled(false);
		m_pageTraining->l9->setEnabled(false);
		m_pageTraining->l10->setEnabled(false);
		m_pageTraining->kcfg_UpSpeedLimit->setEnabled(false);
		m_pageTraining->kcfg_UpCorrectLimit->setEnabled(false);
		m_pageTraining->kcfg_DownSpeedLimit->setEnabled(false);
		m_pageTraining->kcfg_DownCorrectLimit->setEnabled(false);
	}	
}


// *********************************
// *** Protected member function ***
// *********************************

bool KTouch::queryClose() {
	// TODO : if in test mode, ask user whether he really wants to quit the test
	return true;
}

bool KTouch::queryExit() {
	// store some training/exam related data
	Prefs::setCurrentTrainingLevel( m_trainer->m_level );
    Prefs::writeConfig();
    return true;
}

// *****************************************************
// *** Private slots (implementation of the actions) ***
// *****************************************************

// The action File->Open lecture...
void KTouch::fileOpenLecture() {
    KURL tmp = KFileDialog::getOpenURL(QString::null, QString::null, this, i18n("Select training lecture file...") );
    if (!tmp.isEmpty()) {
		Prefs::setCurrentLectureFile(tmp.url());
		m_lecture.loadXML(this, Prefs::currentLectureFile() );
		updateFontFromLecture();
		// adjust check marks in quick-select menus
		updateLectureActionCheck();
	}
}
// ----------------------------------------------------------------------------

// The action File->Edit lecture...
void KTouch::fileEditLecture() {
	// Create and execute editor
    KTouchLectureEditor dlg(this);
    dlg.startEditor( Prefs::currentLectureFile() );
	// Reload lecture in case it was modified
	m_lecture.loadXML(this, Prefs::currentLectureFile() );
	updateFontFromLecture();
	// adjust check marks in quick-select menus
	updateLectureActionCheck();
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
        trainingContinue();
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
	m_pageGeneral = new KTouchPrefGeneralLayout(0, "General");
	dialog->addPage(m_pageGeneral, i18n("General Options"), "style");
	m_pageTraining = new KTouchPrefTrainingLayout(0, "Training");
	dialog->addPage(m_pageTraining, i18n("Training Options"), "kalarm");
	m_pageKeyboard = new KTouchPrefKeyboardLayout(0, "Keyboard");
	dialog->addPage(m_pageKeyboard, i18n("Keyboard Settings"), "keyboard_layout");
	KTouchPrefColorsLayout *m_pageColors = new KTouchPrefColorsLayout(0, "Colors"); 
	dialog->addPage(m_pageColors, i18n("Color Settings"), "package_graphics");
	connect(dialog, SIGNAL(settingsChanged()), this, SLOT(applyPreferences()));
	// TODO : Connect some other buttons/check boxes of the dialog
	connect(m_pageGeneral->kcfg_OverrideLectureFont, SIGNAL(toggled(bool)), this, SLOT(configOverrideLectureFontToggled(bool)));
	connect(m_pageKeyboard->kcfg_OverrideKeyboardFont, SIGNAL(toggled(bool)), this, SLOT(configOverrideKeyboardFontToggled(bool)));
	connect(m_pageTraining->kcfg_AutoLevelChange, SIGNAL(toggled(bool)), this, SLOT(configAutoLevelChangeToggled(bool)));
	// call the functions to enable/disable controls depending on settings
	configOverrideLectureFontToggled(Prefs::overrideLectureFont());
	configOverrideKeyboardFontToggled(Prefs::overrideKeyboardFont());
	configAutoLevelChangeToggled(Prefs::autoLevelChange());
	dialog->show();
	trainingContinue();
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
    if (static_cast<unsigned int>(num)>=m_keyboardFiles.count()) return;
    Prefs::setCurrentKeyboardFile( m_keyboardFiles[num] );
	kdDebug() << "[KTouch::changeKeyboard]  new keyboard layout = " << Prefs::currentKeyboardFile() << endl;
    m_keyboardLayoutAction->setCurrentItem(num);
    m_keyboardWidget->applyPreferences(this, false);  // noisy, pop up an error if the chosen layout file is corrupt
}
// ----------------------------------------------------------------------------

void KTouch::changeColor(int num) {
    if (static_cast<unsigned int>(num)>=m_colorSchemes.count()) return;
    Prefs::setColorScheme(num);
    m_keyboardWidget->applyPreferences(this, false);
}
// ----------------------------------------------------------------------------

void KTouch::changeLecture(int num) {
    if (static_cast<unsigned int>(num)>=m_lectureFiles.count()) return;
    trainingPause();
	KTouchLecture l;
	QString fileName = m_lectureFiles[num];
    if (!l.loadXML(this, KURL::fromPathOrURL(fileName))) {
        KMessageBox::sorry(0, i18n("Could not find/open the lecture file '%1'.").arg(fileName) );
    	m_defaultLectureAction->setCurrentItem(-1);
	}
	else {
		Prefs::setCurrentLectureFile( fileName );
		m_lecture = l;
		updateFontFromLecture();
    	m_defaultLectureAction->setCurrentItem(num);
	}
    m_trainer->goFirstLevel();
	trainingContinue();
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

/// Initialises the program during a normal startup
void KTouch::init() {
	kdDebug() << "[KTouch::init]  populating file lists..." << endl;
    updateFileLists();  // create lists with default lecture/keyboard/colour scheme files
	kdDebug() << "[KTouch::init]  " << m_lectureFiles.count() << " lectures available" << endl;
	kdDebug() << "[KTouch::init]  " << m_keyboardFiles.count() << " keyboard layouts available" << endl;
	kdDebug() << "[KTouch::init]  " << m_examinationFiles.count() << " examination files available" << endl;

	// TODO : look up a default english lecture in the m_lectureFiles string list
	QString default_lecture;
	if (m_lectureFiles.count() > 0)  default_lecture = m_lectureFiles[0];
	if (Prefs::currentLectureFile() == "default")
		Prefs::setCurrentLectureFile( default_lecture );
    // read keyboard settings
	
	QString default_keyboard = "number.keyboard";
	// look up the default english keyboard file in the m_keyboardFiles string list
	QStringList::iterator it = m_keyboardFiles.begin();
	while (it != m_keyboardFiles.end()   &&   (*it).find("en.keyboard") == -1)  ++it;
	if (it != m_keyboardFiles.end())   default_keyboard = *it;
    // if keyboard layout (loaded by Prefs is not available (e.g. the 
	// layout file has been deleted) switch to default keyboard
    if (m_keyboardFiles.contains(Prefs::currentKeyboardFile() )==0)
        Prefs::setCurrentKeyboardFile ( default_keyboard );

	// create some default colour schemes
	createDefaultColorSchemes();
}

/// Creates the layout and GUI setup for a practice session
void KTouch::initTrainingSession() {
	kdDebug() << "[KTouch::initTrainingSession]  setting up layouts and widgets for new training session..." << endl;
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
	// apply the settings to the widgets
    m_slideLineWidget->applyPreferences();
    m_keyboardWidget->applyPreferences(this, true);  // set preferences silently here

    // create our trainer, the master object for the training stuff...
	if (m_trainer != NULL)  delete m_trainer;
    m_trainer = new KTouchTrainer(m_statusWidget, m_slideLineWidget, m_keyboardWidget, &m_lecture);
	// connections for the trainer object
    connect( m_trainer, SIGNAL(statusbarMessageChanged(const QString&)), this, SLOT(changeStatusbarMessage(const QString&)) );
    connect( m_trainer, SIGNAL(statusbarStatsChanged(unsigned int, unsigned int, unsigned int)),
             this, SLOT(changeStatusbarStats(unsigned int, unsigned int, unsigned int)) );

    // Setup status bar
    statusBar()->show();
    statusBar()->insertItem("", 0, 1);                  // space for the messages
    m_barStatsLabel = new QLabel("",this);
    statusBar()->addWidget(m_barStatsLabel, 0, true);   // for the character statistics / counter
}

/// Creates the (standard) actions and entries in the menu.
void KTouch::setupActions() {
	// *** File menu ***
    new KAction(i18n("&Open lecture ..."), 0, 
		this, SLOT(fileOpenLecture()), actionCollection(), "file_openlecture");
    new KAction(i18n("&Edit lecture ..."), 0, 
		this, SLOT(fileEditLecture()), actionCollection(), "file_editlecture");
    KStdAction::quit(this, SLOT(fileQuit()), actionCollection());

	// *** Training menu ***
    new KAction(i18n("&Start New Training Session"), "launch", 0,
        this, SLOT(trainingNewSession()), actionCollection(), "training_newsession");
    m_trainingContinue = new KAction(i18n("&Continue Training Session"), "player_play", 0,
        this, SLOT(trainingContinue()), actionCollection(), "training_run");
    m_trainingPause = new KAction(i18n("&Pause Training Session"), "player_pause", 0,
        this, SLOT(trainingPause()), actionCollection(), "training_pause");
    m_trainingContinue->setEnabled(false); // because the training session is running initially
    // new KAction(i18n("Show Training S&tatistics"), "frame_chart", 0,
    //     this, SLOT(trainingStatistics()), actionCollection(), "training_stats");
 
    // Setup menu entries for the training lectures
    m_defaultLectureAction = new KSelectAction(i18n("Default &Lectures"), 0, this, 0, actionCollection(), "default_lectures");
    m_defaultLectureAction->setItems(m_lectureTitles);
    m_defaultLectureAction->setCurrentItem(0);  
    connect (m_defaultLectureAction, SIGNAL(activated(int)), this, SLOT(changeLecture(int)));

	// *** Settings menu ***    
    KStdAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
    // Setup menu entries for keyboard layouts    
    m_keyboardLayoutAction= new KSelectAction(i18n("&Keyboard Layouts"), 0, this, 0, actionCollection(), "keyboard_layouts");
    m_keyboardLayoutAction->setItems(m_keyboardFiles);
    connect (m_keyboardLayoutAction, SIGNAL(activated(int)), this, SLOT(changeKeyboard(int)));

	// Setup menu entries for colour schemes
	m_keyboardColorAction = new KSelectAction(i18n("Keyboards &Color Schemes"), 0, this, 0, actionCollection(), "keyboard_schemes");
    QStringList schemes_list;
    for (unsigned int i=0; i<m_colorSchemes.count(); ++i)
		schemes_list.append(m_colorSchemes[i].m_name);
    m_keyboardColorAction->setItems(schemes_list);
    m_keyboardColorAction->setCurrentItem(Prefs::colorScheme());  
    connect (m_keyboardColorAction, SIGNAL(activated(int)), this, SLOT(changeColor(int)));
}

/// This function updates the font used in the sliding line of a font suggestions was
/// made for the current lecture.
void KTouch::updateFontFromLecture() {
	// if the lecture requires a font, try this 
	if (!m_lecture.m_fontSuggestions.isEmpty()) {
		QFont f;
		// TODO : if multiple font suggestions are given, try one after another until a
		// suggested font is found
		if (f.fromString(m_lecture.m_fontSuggestions))	m_slideLineWidget->setFont(f);
		else if (f.fromString("Courier 10 Pitch")) 		m_slideLineWidget->setFont(f);
	}
}

/// This function populates the file lists with the installed training, keyboard and 
/// examination files.
void KTouch::updateFileLists() {
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
    m_keyboardFiles.push_front("number.keyboard");
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
			if (l.loadXML(this, url)) {
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

void KTouch::createDefaultColorSchemes() {
    KTouchColorScheme color;

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
    m_colorSchemes.push_back(color);

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
    m_colorSchemes.push_back(color);

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
    m_colorSchemes.push_back(color);

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
    m_colorSchemes.push_back(color);
}

/// Updates the check mark in the lecture-quick-selection menu depending on the 
/// lecture in Prefs::currentLectureFile().
void KTouch::updateLectureActionCheck() {
	int num = 0;
	QStringList::iterator it = m_lectureFiles.begin();
	QString fname = Prefs::currentLectureFile(); 
	while (it != m_lectureFiles.end()   &&   (*it).find(fname) == -1) {
		++it;
		++num;
	}
	if (it == m_lectureFiles.end())		m_defaultLectureAction->setCurrentItem(-1);
	else    							m_defaultLectureAction->setCurrentItem(num);
}



