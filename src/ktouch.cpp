/***************************************************************************
 *   ktouch.cpp                                                            *
 *   ----------                                                            *
 *   Copyright (C) 2000 by H�ard Friland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouch.h"
#include "ktouch.moc"

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
#include <kfontrequester.h>

// Own header files
//#include "ktouchpref.h"
#include "ktouchlecture.h"
#include "ktoucheditor.h"
#include "ktouchstatus.h"
#include "ktouchslideline.h"
#include "ktouchkeyboard.h"
#include "ktouchsettings.h"
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
    m_editorDlg(NULL),
    m_startNewDlg(NULL),
    m_statusWidget(NULL),
    m_keyboardWidget(NULL),
    m_statsWidget(NULL),
    m_lecture(new KTouchLecture)
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
	m_trainer = new KTouchTrainer(m_statusWidget, m_slideLineWidget, m_keyboardWidget, m_lecture);
	
	// Setup status bar
	statusBar()->show();
	statusBar()->insertItem("", 0, 1);                  // space for the messages
	m_barStatsLabel = new QLabel("",this);
	statusBar()->addWidget(m_barStatsLabel, 0, true);   // for the character statistics / counter
	
	// Setup our actions and connections
	setupActions();
	
	// read our preferences settings
	KTouchConfig().loadSettings();
	// and apply them to the widgets
	m_statusWidget->applyPreferences();
	m_slideLineWidget->applyPreferences();
	m_keyboardWidget->applyPreferences(true);  // set preferences silently here
	
	// finally create the GUI reading the ui.rc file
	//createGUI("/home/kdeinstall/ktouch/src/ktouchui.rc");
	setupGUI();
	
	// Add available lectures, keyboard layouts and colour schemes to the settings menu
	setupQuickSettings();
	
	if (!kapp->isRestored()) {
		// Read the old training state into the current session of the trainer,
		// that means last used lecture and last level
		readTrainingState(kapp->config());
		// Reload the last used training file.
		reloadLecture();
		// If the user doesn't want to restart with his old level, reset it
		if (!Prefs::rememberLevel())
		m_trainer->m_level=0;
		// now let's show the first line of the current level
		m_trainer->goFirstLine();
		// the current training session is already started and the training will start on first keypress
		changeStatusbarMessage( i18n("Starting training session: Waiting for first keypress...") );
		changeStatusbarStats( m_trainer->m_session.m_correctChars,
		m_trainer->m_session.m_totalChars, m_trainer->m_session.m_words);
		updateCaption();
	};
	// when KTouch is restored this stuff is done in readProperties()
}

KTouch::~KTouch() {
	// Note: The dialogs that are created as childs of KTouch will be deleted when the KTouch object
	//       is destructed. But I consider it good and clean style to clean up the mess you've caused :-)
	delete m_editorDlg;         // free memory of editor dialog
	delete m_startNewDlg;       // free memory of "start new session" dialog
	delete m_lecture;           // free memory of the lecture object
	delete m_trainer;           // free memory of the trainer
	// set the pointers to zero (not necessary but just a little bit on the safe side)
	m_editorDlg=NULL;
	m_startNewDlg=NULL;
	m_lecture=NULL;
	m_trainer=NULL;
}


// ********************
// *** Public slots ***
// ********************

void KTouch::applyPreferences() {
	// ok, just notify our widgets to update themselves, we don't need to know the details...
	m_statusWidget->applyPreferences();
	m_slideLineWidget->applyPreferences();
	m_keyboardWidget->applyPreferences(false);  // noisy preferences here
}

void KTouch::updateCaption() {
	// kdDebug() << "[KTouch::updateCaption]  URL = " << m_lecture->m_lectureURL << endl;
	QString caption = m_lecture->fileName();
	if (caption.isEmpty())
		caption = i18n("unknown.ktouch");
	if (m_lecture->isModified())
		caption += " " + i18n("(modified)");
	setCaption(caption);
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
	// NOTE: during session management the lecture should already be saved in a temporary file and
	//       therefore saveModified() won't pop up a dialog anyway. But we're not taking any chances
	//       here and allow KTouch to quit in any case.
	if (kapp->sessionSaving()) {
		return true;
	};
	return saveModified();
}

bool KTouch::queryExit() {
	KTouchConfig().saveSettings();          // saves preferences
	saveTrainingState(kapp->config());      // saves training state
	return true;
}


// *****************************************************
// *** Private slots (implementation of the actions) ***
// *****************************************************

void KTouch::fileNew() {
	if (!saveModified()) return;
	// set up a default lecture and show the first line of it
	m_lecture->createDefault();
	updateCaption();
	m_trainer->goFirstLevel();
	// start lecture editor since the user will probably edit this default lecture
	trainingLectureEdit();
}

void KTouch::fileOpen() {
	if (!saveModified()) return;
	trainingPause();
	KURL url = KFileDialog::getOpenURL(QString::null, QString::null, this, i18n("Open Training File"));
	if (!url.isEmpty() && url.isValid()) {
		if (!m_lecture->loadLecture(url))
		KMessageBox::information(this, i18n("Could not open the training file."));
		updateCaption();
		m_trainer->goFirstLevel();
	};
}

bool KTouch::fileSave() {
	// check if there is already a filename for the lecture, if not ask for it
	if (m_lecture->m_lectureURL.isEmpty()) {
		KURL fileUrl = KFileDialog::getSaveURL();
		if (!fileUrl.isEmpty() && fileUrl.isValid()) {
		m_lecture->m_lectureURL = fileUrl;
		m_lecture->saveLecture();
		updateCaption();
		return true;
		}
		else
		return false;
	};
	updateCaption();
	return true;
}

void KTouch::fileSaveAs() {
	trainingPause();
	KURL urlBackup = m_lecture->m_lectureURL;   // backup old filename
	m_lecture->m_lectureURL="";
	fileSave();  // will ask for the filename now
	// If the user has aborted the filedialog, reset the old URL
	if (m_lecture->m_lectureURL.isEmpty())
		m_lecture->m_lectureURL = urlBackup;
	updateCaption();
}

void KTouch::fileQuit() {
	if (saveModified())
		kapp->quit();
}

void KTouch::trainingNewSession() {
	trainingPause();
	if (m_startNewDlg==NULL)
	m_startNewDlg = new KTouchStartNewDialog(this);
	if (m_startNewDlg->exec()==QDialog::Accepted) {
		m_trainer->startNewTrainingSession(m_startNewDlg->keepLevel->isChecked());
		m_trainingPause->setEnabled(true);
		m_trainingContinue->setEnabled(false);
	}
	else
		m_trainer->continueTraining();
}

void KTouch::trainingContinue() {
	m_trainingPause->setEnabled(true);
	m_trainingContinue->setEnabled(false);
	m_trainer->continueTraining();
}

void KTouch::trainingPause() {
	m_trainingPause->setEnabled(false);
	m_trainingContinue->setEnabled(true);
	m_trainer->pauseTraining();
}

void KTouch::trainingStatistics() {
	bool sessionRunning = m_trainingPause->isEnabled();
	trainingPause();
	if (m_statsWidget==NULL)
		m_statsWidget=new KTouchStatistics(this, m_trainer);
	m_statsWidget->exec();
	if (sessionRunning)
		trainingContinue();
}

void KTouch::trainingLectureEdit() {
	trainingPause();
	// If the lecture edit dialog hasn't been used until now, create one on the heap
	// and store the pointer for later use. The memory will be freed upen destroying
	// the KTouch object.
	if (m_editorDlg==NULL)
		m_editorDlg=new KTouchEditor(this, m_lecture);
	// Update the dialogs UI widgets
	m_editorDlg->update(true);
	// And finally execute the dialog
	if (m_editorDlg->exec()==QDialog::Accepted) {
		m_editorDlg->update(false);
		updateCaption();    // because update(false) sets the modified flag in the lecture (adds (modified) to the title)
		m_trainer->goFirstLevel();
	};
}


void KTouch::optionsPreferences() {
	trainingPause();
	if ( KConfigDialog::showDialog( "settings" ) )  {
		return;
	}
	//KConfigDialog didn't find an instance of this dialog, so lets create it :
	KConfigDialog* dialog = new KConfigDialog( this, "settings",  Prefs::self() );
	KTouchPrefGeneralLayout * m_pageGeneral = new KTouchPrefGeneralLayout(0, "General");
	m_pageGeneral->kcfg_Font->setSampleText("Have a happy typing time!");
	dialog->addPage(m_pageGeneral, i18n("General Options"), "style");
	KTouchPrefTrainingLayout * m_pageTraining = new KTouchPrefTrainingLayout(0, "Training");
	dialog->addPage(m_pageTraining, i18n("Training Options"), "kalarm");
	KTouchPrefKeyboardLayout * m_pageKeyboard = new KTouchPrefKeyboardLayout(0, "Keyboard");
	dialog->addPage(m_pageKeyboard, i18n("Keyboard Settings"), "keyboard_layout");
	m_pageKeyboard->kcfg_Layout->insertStringList(KTouchConfig().m_keyboardLayouts);
	KTouchPrefColorsLayout *m_pageColors = new KTouchPrefColorsLayout(0, "Colors"); 
	dialog->addPage(m_pageColors, i18n("Color Settings"), "package_graphics");
	connect(dialog, SIGNAL(settingsChanged()), this, SLOT(updateSettings()));
	dialog->show();
}

void KTouch::updateSettings()
{
	//TODO:test if this setting has changed
	//This applies a new color scheme for the keyboard
	changeColor(Prefs::colorScheme());
	changeKeyboard(Prefs::layout());
	//not sure if testing is required before applying all that or could that be done in any case?
	m_statusWidget->applyPreferences();
    	m_slideLineWidget->applyPreferences();
}


void KTouch::changeStatusbarMessage(const QString& text) {
	statusBar()->message(text);
}

void KTouch::changeStatusbarStats(unsigned int correctChars, unsigned int totalChars, unsigned int words) {
	m_barStatsLabel->setText( i18n( "   Correct chars: %1   Total chars: %1   Words: %1   ")
		.arg(correctChars).arg(totalChars).arg(words) );
}

void KTouch::changeKeyboard(int num) {
	if (static_cast<unsigned int>(num)>=KTouchConfig().m_keyboardLayouts.count()) return;
	KTouchConfig().m_keyboardLayout = KTouchConfig().m_keyboardLayouts[num];
	Prefs::setLayout(num);
	Prefs::writeConfig();
	m_keyboardLayoutAction->setCurrentItem(Prefs::layout());  
	m_keyboardWidget->applyPreferences(false);  // noisy, pop up an error if the chosen layout file is corrupt
}

void KTouch::changeColor(int num) {
	if (static_cast<unsigned int>(num)>=KTouchConfig().m_keyboardColors.count()) return;
	KTouchConfig().m_keyboardColorScheme = num;
	Prefs::setColorScheme(num);
	Prefs::writeConfig();
	m_keyboardWidget->applyPreferences(false);
}

void KTouch::changeLecture(int num) {
	if (static_cast<unsigned int>(num)>=KTouchConfig().m_lectureList.count()) return;
	trainingPause();
	if (!saveModified()) return;
	KStandardDirs *dirs=KGlobal::dirs();
	QString fileName = dirs->findResource("appdata",KTouchConfig().m_lectureList[num] + ".ktouch");
	KURL oldLecture (m_lecture->m_lectureURL);
	if (!m_lecture->loadLecture(KURL::fromPathOrURL(fileName))) {
		KMessageBox::sorry(0, i18n("Could not find/open the lecture file '%1.ktouch'.")
		.arg(KTouchConfig().m_lectureList[num]) );
		m_lecture->loadLecture(oldLecture);
	};
	updateCaption();
	m_trainer->goFirstLevel();
}


// *******************************
// *** Private member function ***
// *******************************

void KTouch::readProperties(KConfig *config) {
	//kdDebug() << "[KTouch::readProperties]  Reading session data..." << endl;
	// The application is about to be restored due to session management.
	// Let's read all the stuff that was set when the application was terminated (during KDE logout).
	QString session = config->readEntry("Session");
	if (!session.isEmpty())
		m_trainer->m_session = KTouchTrainingSession(session);
	m_trainer->m_level = config->readNumEntry("Level", 0);
	m_trainer->m_line = config->readNumEntry("Line", 0);
	bool lectureModified = config->readBoolEntry("LectureModified", false);
	if (lectureModified) {
		kdDebug() << "[KTouch::readProperties]  Reading modified lecture data..." << endl;
		QString tmpFile = config->readPathEntry("LectureTmpFile");
		kdDebug() << "[KTouch::readProperties]  from file " << tmpFile << endl;
		m_lecture->loadLecture(KURL::fromPathOrURL(tmpFile)); // read the lecture
		m_lecture->setModified(true);    // mark lecture as modified
		m_lecture->m_lectureURL = config->readPathEntry("Lecture"); // and set correct filename again
	}
	else {
		m_lecture->m_lectureURL = config->readPathEntry("Lecture");
		reloadLecture();
	};
	updateCaption();                    // update the caption string
	m_trainer->readSessionHistory();    // read session history (excluding currently active session)
	// update the trainer object
	m_trainer->m_teacherText = m_lecture->level(m_trainer->m_level).line(m_trainer->m_line);
	m_trainer->m_studentText = config->readEntry("StudentText");
	m_trainer->continueTraining();
	changeStatusbarMessage( i18n("Restart training session: Waiting for first keypress...") );
	// update the slide line widget
	m_slideLineWidget->setNewText(m_trainer->m_teacherText, m_trainer->m_studentText);
	// update all the other widgets
	m_trainer->updateWidgets();
}

void KTouch::saveProperties(KConfig *config) {
	// kdDebug() << "[KTouch::saveProperties]  Saving session data..." << endl;
	// We are going down because of session management (most likely because of
	// KDE logout). Let's save the current status so that we can restore it
	// next logon.
	
	// first save the lecture and the edit-status
	config->writePathEntry("Lecture", m_lecture->m_lectureURL.url());
	config->writeEntry("LectureModified", m_lecture->isModified());
	if (m_lecture->isModified()) {
		// save modified lecture data to temporary file
		QString tmpFile = KGlobal::dirs()->saveLocation("appdata")+"lecture_last_session";
		kdDebug() << "[KTouch::saveProperties]  Saving changed lecture to tempfile..." << endl;
		config->writePathEntry("LectureTmpFile", tmpFile);
		m_lecture->m_lectureURL = tmpFile;
		m_lecture->saveLecture();
	};
	config->writeEntry("Level", m_trainer->m_level);
	config->writeEntry("Line", m_trainer->m_line);
	config->writeEntry("StudentText", m_trainer->m_studentText);
	config->writeEntry("Session", m_trainer->m_session.asString() );
	m_trainer->writeSessionHistory();
}

void KTouch::readTrainingState(KConfig *config) {
	config->setGroup("TrainingState");
	m_lecture->m_lectureURL = config->readPathEntry("LectureURL");
	m_trainer->m_level = config->readNumEntry("Level", 0);
	m_trainer->readSessionHistory();
}

void KTouch::saveTrainingState(KConfig *config) {
	config->setGroup("TrainingState");
	config->writePathEntry("LectureURL", m_lecture->m_lectureURL.url());
	config->writeEntry("Level", m_trainer->m_level);
	// during normal shutdown we finish the session and add it to the session history
	m_trainer->m_sessionHistory.append( m_trainer->m_session );
	m_trainer->writeSessionHistory();
}

void KTouch::reloadLecture() {
	// normal startup: last used URL is already stored in m_lecture->m_lectureURL, or - when the program
	//  is run the first time or the config file was deleted - it is empty
	KURL lectureURL = m_lecture->m_lectureURL;
	if (lectureURL.isEmpty()) {
		// no last lecture URL available, look for a default 'english' lecture file
		KStandardDirs *dirs=KGlobal::dirs();
		lectureURL = dirs->findResource("appdata","lectures/english.ktouch");
	};
	m_lecture->loadLecture(lectureURL);
	// A note about safety: In this function there are a lot of things that might go
	// wrong. What happens if the english training file can't be found? What if the
	// file cannot be opened or is corrupt? Whatever happens, the last called function
	// loadLecture() ensures, that there is at least the default mini-level in the lecture
	// so that the training won't crash.
}

void KTouch::setupActions() {
	// actions for the file menu
	KStdAction::openNew(this, SLOT(fileNew()), actionCollection());
	KStdAction::open(this, SLOT(fileOpen()), actionCollection());
	KStdAction::save(this, SLOT(fileSave()), actionCollection());
	KStdAction::saveAs(this, SLOT(fileSaveAs()), actionCollection());
	KStdAction::quit(this, SLOT(fileQuit()), actionCollection());
	
	// actions for the training menu
	new KAction(i18n("&Start New Training Session"), "launch", 0,
		this, SLOT(trainingNewSession()), actionCollection(), "training_newsession");
	m_trainingContinue = new KAction(i18n("&Continue Training Session"), "player_play", 0,
		this, SLOT(trainingContinue()), actionCollection(), "training_run");
	m_trainingPause = new KAction(i18n("&Pause Training Session"), "player_pause", 0,
		this, SLOT(trainingPause()), actionCollection(), "training_pause");
	new KAction(i18n("&Edit Lecture"), 0, this, SLOT(trainingLectureEdit()), actionCollection(), "lecture_edit");
	m_trainingContinue->setEnabled(false); // because the training session is running initially
	new KAction(i18n("Show Training S&tatistics"), "frame_chart", 0,
		this, SLOT(trainingStatistics()), actionCollection(), "training_stats");
	
	// actions for the settings menu
	KStdAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
	m_keyboardLayoutAction= new KSelectAction(i18n("&Keyboard Layouts"), 0, this, 0, actionCollection(), "keyboard_layouts");
	m_keyboardColorAction = new KSelectAction(i18n("Keyboards &Color Schemes"), 0, this, 0, actionCollection(), "keyboard_schemes");
	// actions for the Training menu
	m_defaultLectureAction = new KSelectAction(i18n("Default &Lectures"), 0, this, 0, actionCollection(), "default_lectures");
	// Finally the connections
	connect( m_trainer, SIGNAL(statusbarMessageChanged(const QString&)), this, SLOT(changeStatusbarMessage(const QString&)) );
	connect( m_trainer, SIGNAL(statusbarStatsChanged(unsigned int, unsigned int, unsigned int)),
		this, SLOT(changeStatusbarStats(unsigned int, unsigned int, unsigned int)) );
}

void KTouch::setupQuickSettings() {
	//Add layouts
	QStringList layouts_list;
	for (unsigned int i=0; i<KTouchConfig().m_keyboardLayouts.count(); ++i) {
		layouts_list.append( KTouchConfig().m_keyboardLayouts[i]);
	}
	m_keyboardLayoutAction->setItems(layouts_list);
	m_keyboardLayoutAction->setCurrentItem(Prefs::layout());  
	connect (m_keyboardLayoutAction, SIGNAL(activated(int)), this, SLOT(changeKeyboard(int)));
	// add the colour schemes
	QStringList schemes_list;
	for (unsigned int i=0; i<KTouchConfig().m_keyboardColors.count(); ++i) {
			schemes_list.append(KTouchConfig().m_keyboardColors[i].m_name);
	}
	m_keyboardColorAction->setItems(schemes_list);
	m_keyboardColorAction->setCurrentItem(Prefs::colorScheme());  
	connect (m_keyboardColorAction, SIGNAL(activated(int)), this, SLOT(changeColor(int)));
	
	// Then get the trainings menu
	QStringList lectures_list;
	for (unsigned int i=0; i<KTouchConfig().m_lectureList.count(); ++i) {
		lectures_list.append( KTouchConfig().m_lectureList[i]);
	}
	m_defaultLectureAction->setItems(lectures_list);
	m_defaultLectureAction->setCurrentItem(Prefs::layout());  
	connect (m_defaultLectureAction, SIGNAL(activated(int)), this, SLOT(changeLecture(int)));
}

bool KTouch::saveModified() {
	if (m_lecture->isModified()) {
		int result = KMessageBox::questionYesNoCancel(0,
		i18n("Your current lecture has been modified. Would you like to save it?"));
		switch (result) {
		case KMessageBox::Yes : return fileSave();
		case KMessageBox::No  : return true;
		default               : return false;
		};
	}
	else
		return true;    // no changes -> no need to ask
}
