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
#include <q3vbox.h>
#include <qsignalmapper.h>
#include <qcheckbox.h>
#include <qlabel.h>
//Added by qt3to4:
#include <QKeyEvent>
#include <QResizeEvent>
#include <kstdaction.h>

// KDE Header
#include <klocale.h>
#include <kstatusbar.h>
#include <kfiledialog.h>
#include <kaction.h>
#include <kstandarddirs.h>
#include <kmessagebox.h>
#include <kdebug.h>
#include <kmenu.h>
#include <kconfigdialog.h>
#include <kaction.h>
#include <kcombobox.h>
#include <kfontrequester.h>
#include <knuminput.h>

// Own header files
#include "ktouchlecture.h"
#include "ktouchlectureeditor.h"
#include "ktouchstatus.h"
#include "ktouchslideline.h"
#include "ktouchkeyboardwidget.h"
#include "ktouchkeyboardeditor.h"
#include "ktouchtrainer.h"
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
	setFocusPolicy(Qt::StrongFocus);
	// Set global KTouchPtr to the main KTouch Object
	KTouchPtr = this;
	// General initialization of the program, common for all start modes
	init();
	// Setup our actions and connections
	setupActions();
	// create the GUI reading the ui.rc file
	if (!initialGeometrySet())
		resize( QSize(700, 510).expandedTo(minimumSizeHint()));
	setupGUI(ToolBar | Keys | StatusBar | Create);
	setAutoSaveSettings();
	// Read user statistics
	KUrl stat_file = KGlobal::dirs()->findResource("data", "ktouch/statistics.xml");
	//kDebug() << "[KTouch::KTouch]  readings statistics from file '" << stat_file << "'" << endl;
	if (!m_stats.read(this, stat_file))
		m_stats.clear();	// if we can't read it, start with empty statistics

	// Init a training session
	initTrainingSession();

	// If session was restored, the function readProperties() was already called
    if (kapp->isSessionRestored()) {
		kDebug() << "[KTouch::KTouch]  restoring session..." << endl;
		/// \todo Rewrite all the session management stuff.
		///       For now we just do the same as for the standard startup.
	}
//	else {
		//kDebug() << "[KTouch::KTouch]  starting standard training..." << endl;
		// A note about safety: In this function there are a lot of things that might go
		// wrong. What happens if the training file can't be found? What if the
		// file cannot be opened or is corrupt? Whatever happens, the function loadXML()
		// ensures, that there is at least the default mini-level in the lecture
		// so that the training won't crash.

		// Load statistics data from statistics-file

	    // Reload the last used training file.
		if (!m_lecture.loadXML(this, Prefs::currentLectureFile() )) {
			Prefs::setCurrentLectureFile(QString());
			m_defaultLectureAction->setCurrentItem(-1);
		}
		else {
			updateFontFromLecture();
			// adjust check marks in quick-select menus
			updateLectureActionCheck();
			//kDebug() << "[KTouch::KTouch]  lecture file = " << Prefs::currentLectureFile() << endl;
		}

		// Adjust check mark for the keyboard file
		updateKeyboardActionCheck();
		// If the user doesn't want to restart with his old level, start from 0 (level 1)
        if (!Prefs::rememberLevel())	m_trainer->m_level = 0;
		else							m_trainer->m_level = Prefs::currentTrainingLevel();
        // now let's start the training in the current level
		m_trainer->startTraining(true);
//	}
}
// ----------------------------------------------------------------------------

// Free memory of objects that are not owned by the main KTouch object
KTouch::~KTouch() {
	delete m_trainer;
	m_trainer = NULL;
}
// ----------------------------------------------------------------------------

KTouchLectureStats& KTouch::getCurrentLectureStats() {
	KTouchLectureStats& stat = m_stats.m_lectureStats[Prefs::currentLectureFile()];
	// add additional info to the statistics
	if (stat.m_lectureURL.isEmpty())
		stat.m_lectureURL = Prefs::currentLectureFile();
	if (stat.m_lectureTitle.isEmpty())
		stat.m_lectureTitle = m_lecture.m_title;
	return stat;
}
// ----------------------------------------------------------------------------

void KTouch::clearStatistics() {
	m_stats.clear();
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
// ----------------------------------------------------------------------------

void KTouch::keyPressEvent(QKeyEvent *keyEvent) {
    if (keyEvent->text().isEmpty()) return;
    // if we the training session is paused, continue training now
    if (m_trainer->m_trainingPaused)  {
		m_trainingPause->setEnabled(true);
		m_trainer->continueTraining();
	}
    QChar key = keyEvent->text().at(0); // get first unicode character
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
// ----------------------------------------------------------------------------

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
// ----------------------------------------------------------------------------

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
// ----------------------------------------------------------------------------

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
// ----------------------------------------------------------------------------

// The action File->Open lecture...
void KTouch::fileOpenLecture() {
	trainingPause();
	KUrl tmp = KFileDialog::getOpenURL(QString(), QString(), this, i18n("Select Training Lecture File") );
    if (!tmp.isEmpty()) {
		// first store training statistics
		m_trainer->storeTrainingStatistics();
		Prefs::setCurrentLectureFile(tmp.url());
		m_lecture.loadXML(this, Prefs::currentLectureFile() );
		updateFontFromLecture();
		// adjust check marks in quick-select menus
		updateLectureActionCheck();
	}
	// restart training session from level 1 here...
	m_trainer->startTraining(false);
	m_trainingPause->setEnabled(true);
}
// ----------------------------------------------------------------------------

// The action File->Edit lecture...
void KTouch::fileEditLecture() {
	trainingPause();
	m_trainer->storeTrainingStatistics();
	// Create and execute editor
    KTouchLectureEditor dlg(this);
    dlg.startEditor( Prefs::currentLectureFile() );
	// Reload lecture in case it was modified
	m_lecture.loadXML(this, Prefs::currentLectureFile() );
	updateFontFromLecture();
	// adjust check marks in quick-select menus
	updateLectureActionCheck();
	// restart training session here (keep level)...
	m_trainer->startTraining(true);
	m_trainingPause->setEnabled(true);
}
// ----------------------------------------------------------------------------

// The action File->Edit keyboard...
void KTouch::fileEditKeyboard() {
	trainingPause();
	// Create and execute editor
    KTouchKeyboardEditor dlg(this);
    dlg.startEditor( Prefs::currentKeyboardFile() );
	// Reload lecture in case it was modified
	//m_keyboard.loadXML(this, Prefs::currentKeyboardFile() );
	//updateFontFromLecture();
	// adjust check marks in quick-select menus
	//updateKeyboardActionCheck();
}
// ----------------------------------------------------------------------------

void KTouch::fileQuit() {
    kapp->quit();
}
// ----------------------------------------------------------------------------

void KTouch::trainingNewSession() {
    trainingPause();
	int result = KMessageBox::questionYesNoCancel(this,
		i18n("Would you like to keep the current level for the new training session?"),
		i18n("Start New Training Session"),i18n("Keep Current Level"),i18n("Do Not Keep"));
	if (result == KMessageBox::Cancel) return;
	// store the statistics obtained so far in the trainer object
	m_trainer->storeTrainingStatistics();
	// start new training session here
	m_trainer->startTraining(result == KMessageBox::Yes);
    m_trainingPause->setEnabled(true);
}
// ----------------------------------------------------------------------------

void KTouch::trainingPause() {
    m_trainingPause->setEnabled(false);
    m_trainer->pauseTraining();
}
// ----------------------------------------------------------------------------

void KTouch::trainingStatistics() {
    trainingPause();
	KTouchStatistics dlg(this);
	// TODO : this is somewhat messy: we have to get the words in the
    //        current line (since they are not stored in the current
    //        level and session stats, because the student may delete
    //        the whole line again) and add them manually to copies
    //        of the currents stats
	KTouchSessionStats kss = m_trainer->m_sessionStats;
	KTouchLevelStats kls = m_trainer->m_levelStats;
	kss.m_words += m_trainer->wordsInCurrentLine();
	kls.m_words += m_trainer->wordsInCurrentLine();
	// by calling getCurrentLectureStats we ensure that there is
    // data for the current lecture present for the dialog to function
	// properly
	getCurrentLectureStats();
    dlg.run(Prefs::currentLectureFile(), m_stats, kls, kss);
}
// ----------------------------------------------------------------------------

void KTouch::optionsPreferences() {
    trainingPause();
	if ( KConfigDialog::showDialog( "settings" ) )	return;
	// KConfigDialog didn't find an instance of this dialog, so lets create it :
	KConfigDialog* dialog = new KConfigDialog( this, "settings",  Prefs::self() );
	m_pageGeneral = new KTouchPrefGeneralLayout(0, "General");
	dialog->addPage(m_pageGeneral, i18n("General Options"), "style");
	m_pageTraining = new KTouchPrefTrainingLayout(0, "Training");
	dialog->addPage(m_pageTraining, i18n("Training Options"), "kalarm");
	m_pageKeyboard = new KTouchPrefKeyboardLayout(0, "Keyboard");
	dialog->addPage(m_pageKeyboard, i18n("Keyboard Settings"), "keyboard_layout");
	KTouchPrefColorsLayout *m_pageColors = new KTouchPrefColorsLayout(0, "Colors");
	dialog->addPage(m_pageColors, i18n("Color Settings"), "package_graphics");
	connect(dialog, SIGNAL(settingsChanged(const QString &)), this, SLOT(applyPreferences()));
	// TODO : Connect some other buttons/check boxes of the dialog
	connect(m_pageGeneral->kcfg_OverrideLectureFont, SIGNAL(toggled(bool)),
		this, SLOT(configOverrideLectureFontToggled(bool)));
	connect(m_pageKeyboard->kcfg_OverrideKeyboardFont, SIGNAL(toggled(bool)),
		this, SLOT(configOverrideKeyboardFontToggled(bool)));
	connect(m_pageTraining->kcfg_AutoLevelChange, SIGNAL(toggled(bool)),
		this, SLOT(configAutoLevelChangeToggled(bool)));
	// call the functions to enable/disable controls depending on settings
	configOverrideLectureFontToggled(Prefs::overrideLectureFont());
	configOverrideKeyboardFontToggled(Prefs::overrideKeyboardFont());
	configAutoLevelChangeToggled(Prefs::autoLevelChange());
	dialog->show();
}
// ----------------------------------------------------------------------------

void KTouch::changeStatusbarMessage(const QString& text) {
    statusBar()->message(text);
}
// ----------------------------------------------------------------------------

void KTouch::changeStatusbarStats(unsigned int level_correct, unsigned int level_total, unsigned int level_words,
	unsigned int session_correct, unsigned int session_total, unsigned int session_words)
{
	statusBar()->changeItem(i18n( "Level:  Correct/Total chars: %1/%2  Words: %3",
			 level_correct, level_total, level_words), 1);
	statusBar()->changeItem(i18n( "Session: Correct/Total chars: %1/%2  Words: %3",
			 session_correct, session_total, session_words), 2);
}
// ----------------------------------------------------------------------------

void KTouch::changeKeyboard(int num) {
    if (num>=m_keyboardFiles.count()) return;
    Prefs::setCurrentKeyboardFile( m_keyboardFiles[num] );
	//kDebug() << "[KTouch::changeKeyboard]  new keyboard layout = " << Prefs::currentKeyboardFile() << endl;
    m_keyboardLayoutAction->setCurrentItem(num);
	// call Apply-Preferenzes in "noisy"-mode, pop up an error if the chosen layout file is corrupt
    m_keyboardWidget->applyPreferences(this, false);
}
// ----------------------------------------------------------------------------

void KTouch::changeColor(int num) {
    if (num>=m_colorSchemes.count()) return;
    Prefs::setColorScheme(num);
    m_keyboardWidget->applyPreferences(this, false);
}
// ----------------------------------------------------------------------------

void KTouch::changeLecture(int num) {
    if (num>=m_lectureFiles.count()) return;
    trainingPause();
	KTouchLecture l;
	QString fileName = m_lectureFiles[num];
    if (!l.loadXML(this, KUrl::fromPathOrURL(fileName))) {
        KMessageBox::sorry(0, i18n("Could not find/open the lecture file '%1'.", fileName) );
    	m_defaultLectureAction->setCurrentItem(-1);
	}
	else {
		// store the statistics obtained so far in the trainer object
		m_trainer->storeTrainingStatistics();
		// set new lecture as current
		Prefs::setCurrentLectureFile( fileName );
		m_lecture = l;
		updateFontFromLecture();
    	m_defaultLectureAction->setCurrentItem(num);
    	// now let's start the training in the first level of the training lecture
		m_trainer->startTraining(false);
		m_trainingPause->setEnabled(true);
	}
}
// ----------------------------------------------------------------------------


// *********************************
// *** Protected member function ***
// *********************************

bool KTouch::queryExit() {
	// store config data
	Prefs::setCurrentTrainingLevel( m_trainer->m_level );
    Prefs::writeConfig();
	// update and save statistics
	m_trainer->storeTrainingStatistics();
	KUrl stat_file = KGlobal::dirs()->saveLocation("data","ktouch", true) + "statistics.xml";
	//kDebug() << "[KTouch::queryExit]  Writing statistics to file: '" << stat_file << "'" << endl;
	m_stats.write(this, stat_file);
    return true;
}
// ----------------------------------------------------------------------------

void KTouch::resizeEvent(QResizeEvent * event) {
	changeStatusbarStats(m_trainer->m_levelStats.m_correctChars, m_trainer->m_levelStats.m_totalChars, m_trainer->m_levelStats.m_words,
		m_trainer->m_sessionStats.m_correctChars, m_trainer->m_sessionStats.m_totalChars, m_trainer->m_sessionStats.m_words);
	KMainWindow::resizeEvent(event);
}
// ----------------------------------------------------------------------------


// *******************************
// *** Private member function ***
// *******************************

// Will be called when this app is restored due to session management.
// This function only stored the temperary data of the last session. All permanent
// settings should be handled by the KTouchConfiguration object.
void KTouch::readProperties(KConfig *config) {
    kDebug() << "[KTouch::readProperties]  Reading session data..." << endl;
	// TODO : Session management rewrite
    config->setGroup("TrainingState");

/*
	// The application is about to be restored due to session management.
    // Let's read all the stuff that was set when the application was terminated (during KDE logout).
    QString session = config->readEntry("Session");
    if (!session.isEmpty())
        m_trainer->m_session = KTouchTrainingSession(session);
    m_trainer->m_level = config->readEntry("Level", 0);
    m_trainer->m_line = config->readEntry("Line", 0);
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
    m_trainer->m_level = config->readEntry("Level", 0);
*/
}
// ----------------------------------------------------------------------------

// Will be called when the app should save its state for session management purposes.
void KTouch::saveProperties(KConfig *config) {
    kDebug() << "[KTouch::saveProperties]  Saving session data..." << endl;
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
// ----------------------------------------------------------------------------

// Initialises the program during a normal startup
void KTouch::init() {
	//kDebug() << "[KTouch::init]  populating file lists..." << endl;
    updateFileLists();  // create lists with default lecture/keyboard/examination files/colour scheme files
	//kDebug() << "[KTouch::init]  " << m_lectureFiles.count() << " lectures available" << endl;
	//kDebug() << "[KTouch::init]  " << m_keyboardFiles.count() << " keyboard layouts available" << endl;
	//kDebug() << "[KTouch::init]  " << m_examinationFiles.count() << " examination files available" << endl;

	/// \todo look up a default english lecture in the m_lectureFiles string list
	QString default_lecture;
	if (m_lectureFiles.count() > 0)  default_lecture = m_lectureFiles[0];
	if (Prefs::currentLectureFile() == "default")
		Prefs::setCurrentLectureFile( default_lecture );

    // read keyboard layouts
	QString default_keyboard = "number.keyboard";
	// look up the default english keyboard file in the m_keyboardFiles string list
	QStringList::iterator it = m_keyboardFiles.begin();
	while (it != m_keyboardFiles.end()   &&   !(*it).contains("en.keyboard"))  ++it;
	if (it != m_keyboardFiles.end())   default_keyboard = *it;
    // if keyboard layout (loaded by Prefs is not available (e.g. the
	// layout file has been deleted) switch to default keyboard
    if (m_keyboardFiles.contains(Prefs::currentKeyboardFile() )==0)
        Prefs::setCurrentKeyboardFile ( default_keyboard );

	// create some default colour schemes
	createDefaultColorSchemes();
}
// ----------------------------------------------------------------------------

// Creates the layout and GUI setup for a practice session
void KTouch::initTrainingSession() {
	//kDebug() << "[KTouch::initTrainingSession]  setting up layouts and widgets for new training session..." << endl;
    // Build the training area. The status widget has a fixed vertical size, the slide line and the
    // keyboard grow according to their vertical stretch factors (see last argument in the constructors
    // of QSizePolicy)
    Q3VBox * mainLayout = new Q3VBox( this );
    m_statusWidget = new KTouchStatus( mainLayout );
    m_slideLineWidget = new KTouchSlideLine( mainLayout );
    m_slideLineWidget->setSizePolicy( QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding, 0, 1) );
    m_keyboardWidget = new KTouchKeyboardWidget( mainLayout );
    m_keyboardWidget->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding, 0, 3) );
    setCentralWidget(mainLayout);
	// apply the settings to the widgets
    m_slideLineWidget->applyPreferences();
    m_keyboardWidget->applyPreferences(this, true);  // set preferences silently here

    // create our trainer, the master object for the training stuff...
	if (m_trainer != NULL)  delete m_trainer;
    m_trainer = new KTouchTrainer(m_statusWidget, m_slideLineWidget, m_keyboardWidget, &m_lecture);

    // Setup status bar
    statusBar()->show();
	statusBar()->insertPermanentItem("Level", 1, 0);
	statusBar()->insertPermanentItem("Session", 2, 0);
}
// ----------------------------------------------------------------------------

// Creates the (standard) actions and entries in the menu.
void KTouch::setupActions() {
	// *** File menu ***
    KAction *action = new KAction(KIcon("open_lecture"), i18n("&Open Lecture..."), actionCollection(), "file_openlecture");
    connect(action, SIGNAL(triggered(bool)), SLOT(fileOpenLecture()));
    action = new KAction(KIcon("edit_lecture"), i18n("&Edit Lecture..."), actionCollection(), "file_editlecture");
    connect(action, SIGNAL(triggered(bool)), SLOT(fileEditLecture()));
//    new KAction(i18n("&Edit Keyboard..."), "edit_keyboard", 0,
//		this, SLOT(fileEditKeyboard()), actionCollection(), "file_editkeyboard");
    KStdAction::quit(this, SLOT(fileQuit()), actionCollection());

	// *** Training menu ***
    action = new KAction(KIcon("launch"), i18n("&Start New Session"), actionCollection(), "training_newsession");
    connect(action, SIGNAL(triggered(bool)), SLOT(trainingNewSession()));
    m_trainingPause = new KAction(KIcon("player_pause"), i18n("&Pause Session"), actionCollection(), "training_pause");
    connect(m_trainingPause, SIGNAL(triggered(bool)), SLOT(trainingPause()));
    action = new KAction(KIcon("kalarm"), i18n("&Lecture Statistics"), actionCollection(), "training_stats");
    connect(action, SIGNAL(triggered(bool)), SLOT(trainingStatistics()));

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
    for (int i=0; i<m_colorSchemes.count(); ++i)
		schemes_list.append(m_colorSchemes[i].m_name);
    m_keyboardColorAction->setItems(schemes_list);
    m_keyboardColorAction->setCurrentItem(Prefs::colorScheme());
    connect (m_keyboardColorAction, SIGNAL(activated(int)), this, SLOT(changeColor(int)));
}
// ----------------------------------------------------------------------------

// This function updates the font used in the sliding line of a font suggestions was
// made for the current lecture.
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
// ----------------------------------------------------------------------------

// This function populates the file lists with the installed training, keyboard and
// examination files.
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
            KUrl url(*it);
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
// ----------------------------------------------------------------------------

void KTouch::createDefaultColorSchemes() {
    KTouchColorScheme color;

    color.m_name = i18n("Black && White");
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

    color.m_name = i18n("Classic");
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

    color.m_name = i18n("Deep Blue");
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

    color.m_name = i18n("Stripy");
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
// ----------------------------------------------------------------------------

void KTouch::updateLectureActionCheck() {
	int num = 0;
	QStringList::iterator it = m_lectureFiles.begin();
	QString fname = Prefs::currentLectureFile();
	while (it != m_lectureFiles.end()   &&   !(*it).contains(fname)) {
		++it;
		++num;
	}
	if (it == m_lectureFiles.end())		m_defaultLectureAction->setCurrentItem(-1);
	else    							m_defaultLectureAction->setCurrentItem(num);
}
// ----------------------------------------------------------------------------

void KTouch::updateKeyboardActionCheck() {
	int num = 0;
	QStringList::iterator it = m_keyboardFiles.begin();
	QString fname = Prefs::currentKeyboardFile();
	while (it != m_keyboardFiles.end()   &&   !(*it).contains(fname)) {
		++it;
		++num;
	}
	if (it == m_keyboardFiles.end())	m_keyboardLayoutAction->setCurrentItem(-1);
	else					    		m_keyboardLayoutAction->setCurrentItem(num);
}
// ----------------------------------------------------------------------------

