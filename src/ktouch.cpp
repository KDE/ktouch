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
#include <QVBoxLayout>
#include <QSignalMapper>
#include <QCheckBox>
#include <QLabel>
#include <QKeyEvent>
#include <QResizeEvent>

// KDE Header
#include <kselectaction.h>
#include <kstdaction.h>
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
#include <kicon.h>
#include <kio/netaccess.h>

// Own header files
#include "ktouchlecture.h"
#include "ktouchlectureeditor.h"
#include "ktouchstatus.h"
#include "ktouchslideline.h"
#include "ktouchkeyboardwidget.h"
#include "ktouchcoloreditor.h"
#include "ktouchtrainer.h"
#include "ktouchstatistics.h"
#include "ktouchprefgeneral.h"

#include "ktouchpreftraining.h"
#include "ktouchprefkeyboard.h"
#include "ktouchprefcolors.h"

#include "prefs.h"
#include "ktouchcolorscheme.h"
#include "ktouchusersetup.h"
#include "ktouchutils.h"

KTouch * KTouchPtr = NULL;

KTouch::KTouch()
  : KMainWindow( 0, "KTouch" ),
    m_statusWidget(NULL),
    m_keyboardWidget(NULL),
    m_trainer(NULL)
{
	m_duringStartup = true;
    setFocusPolicy(Qt::StrongFocus);
	// Set global KTouchPtr to the main KTouch Object
	KTouchPtr = this;
	// General initialization of the program, common for all start modes
	init();
	// Read user statistics
	KUrl stat_file = KGlobal::dirs()->findResource("data", "ktouch/statistics.xml");
	m_userStats.clear(); // make sure our mapping contains the "default" user
	// read previous stats from file
	// FIXME: find the source code for this function or reimplement it
	// KTouchStatisticsData::readUserStats(this, stat_file, m_userStats);
	//	kDebug() << "[KTouch::KTouch] users = " << m_userStats.count() << endl;
	// Setup our actions and connections
	setupActions();
	// create the GUI reading the ui.rc file
	if (!initialGeometrySet())
		resize( QSize(700, 510).expandedTo(minimumSizeHint()));
	setupGUI(ToolBar | Keys | StatusBar | Create);
	setAutoSaveSettings();

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
		if (Prefs::currentLectureFile().isNull() ||
		    !m_lecture.loadXML(this, Prefs::currentLectureFile() ))
		{
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
	m_duringStartup = false;
}
// ----------------------------------------------------------------------------

// Free memory of objects that are not owned by the main KTouch object
KTouch::~KTouch() {
	delete m_trainer;
	m_trainer = NULL;
}
// ----------------------------------------------------------------------------

KTouchLectureStats& KTouch::getCurrentLectureStats() {
//	kDebug() << "[KTouch::getCurrentLectureStats]  " << endl;
	KUrl lectureURL = Prefs::currentLectureFile();
	if (lectureURL.url().isEmpty())  lectureURL = "default";
//	kDebug() << "  lectureURL = '" << lectureURL << "'" << endl;
	KTouchLectureStats& stat = m_userStats[Prefs::currentUserName()].m_lectureStats[lectureURL];
	// add additional info to the statistics
	stat.m_lectureURL = lectureURL;
	stat.m_lectureTitle = m_lecture.title();
	return stat;
}
// ----------------------------------------------------------------------------

void KTouch::clearStatistics() {
	m_userStats.clear();
	m_userStats[Prefs::currentUserName()].m_userName = i18n("default user");
}
// ----------------------------------------------------------------------------

// ********************
// *** Public slots ***
// ********************

void KTouch::applyPreferences() {
	// This applies a new color scheme for the keyboard and also updates all other
	// changes for the keyboard widget
	changeColor(Prefs::currentColorScheme());
	m_slideLineWidget->applyPreferences();
	m_statusWidget->applyPreferences();
}
// ----------------------------------------------------------------------------

void KTouch::keyPressEvent(QKeyEvent *keyEvent) {
    if (keyEvent->text().isEmpty()) return;

    // if we the training session is paused, continue training now
    if (m_trainer->m_trainingPaused)  {
		m_trainingPause->setEnabled(true);
		m_trainer->continueTraining();
	}
	if (keyEvent->text().length() > 1) {
		kDebug() << "[KTouch::keyPressEvent]  text = '" << keyEvent->text() << "'" << endl;
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
		m_pageGeneral->ui.fontTextLabel->setEnabled(true);
		m_pageGeneral->ui.kcfg_Font->setEnabled(true);
	}
	else {
		m_pageGeneral->ui.fontTextLabel->setEnabled(false);
		m_pageGeneral->ui.kcfg_Font->setEnabled(false);
	}
}
// ----------------------------------------------------------------------------

void KTouch::configOverrideKeyboardFontToggled(bool on) {
	if (on) {
		m_pageKeyboard->ui.textLabel1->setEnabled(true);
		m_pageKeyboard->ui.kcfg_KeyboardFont->setEnabled(true);
	}
	else {
		m_pageKeyboard->ui.textLabel1->setEnabled(false);
		m_pageKeyboard->ui.kcfg_KeyboardFont->setEnabled(false);
	}
}
// ----------------------------------------------------------------------------

void KTouch::configAutoLevelChangeToggled(bool on) {
	if (on) {
		m_pageTraining->ui.l1->setEnabled(true);
		m_pageTraining->ui.l2->setEnabled(true);
		m_pageTraining->ui.l3->setEnabled(true);
		m_pageTraining->ui.l4->setEnabled(true);
		m_pageTraining->ui.l5->setEnabled(true);
		m_pageTraining->ui.l6->setEnabled(true);
		m_pageTraining->ui.l7->setEnabled(true);
		m_pageTraining->ui.l8->setEnabled(true);
		m_pageTraining->ui.l9->setEnabled(true);
		m_pageTraining->ui.l10->setEnabled(true);
		m_pageTraining->ui.kcfg_UpSpeedLimit->setEnabled(true);
		m_pageTraining->ui.kcfg_UpCorrectLimit->setEnabled(true);
		m_pageTraining->ui.kcfg_DownSpeedLimit->setEnabled(true);
		m_pageTraining->ui.kcfg_DownCorrectLimit->setEnabled(true);
		m_pageTraining->ui.kcfg_DisableManualLevelChange->setEnabled(true);
        m_pageTraining->ui.kcfg_NumberOfLinesWorkload->setEnabled(true);
        m_pageTraining->ui.kcfg_CompleteWholeTrainingLevel->setEnabled(true);
	}
	else {
		m_pageTraining->ui.l1->setEnabled(false);
		m_pageTraining->ui.l2->setEnabled(false);
		m_pageTraining->ui.l3->setEnabled(false);
		m_pageTraining->ui.l4->setEnabled(false);
		m_pageTraining->ui.l5->setEnabled(false);
		m_pageTraining->ui.l6->setEnabled(false);
		m_pageTraining->ui.l7->setEnabled(false);
		m_pageTraining->ui.l8->setEnabled(false);
		m_pageTraining->ui.l9->setEnabled(false);
		m_pageTraining->ui.l10->setEnabled(false);
		m_pageTraining->ui.kcfg_UpSpeedLimit->setEnabled(false);
		m_pageTraining->ui.kcfg_UpCorrectLimit->setEnabled(false);
		m_pageTraining->ui.kcfg_DownSpeedLimit->setEnabled(false);
		m_pageTraining->ui.kcfg_DownCorrectLimit->setEnabled(false);
		m_pageTraining->ui.kcfg_DisableManualLevelChange->setEnabled(false);
        m_pageTraining->ui.kcfg_NumberOfLinesWorkload->setEnabled(false);
        m_pageTraining->ui.kcfg_CompleteWholeTrainingLevel->setEnabled(false);
	}	
}
// ----------------------------------------------------------------------------

void KTouch::configCommonColorsToggled(bool on) {
	m_pageColors->ui.colorsGroup->setEnabled(on);
}
// ----------------------------------------------------------------------------

// The action File->Open text...
void KTouch::fileOpenText() {
	trainingPause();
	KUrl tmp = KFileDialog::getOpenUrl(QString(), i18n("*.txt|Text files\n*.*|All files"), this, i18n("Select Practice Text") );
    if (!tmp.isEmpty()) {
		// TODO : ask user for training options
		// create a default lecture
		KTouchLecture lec;
		// now read the file and replace the default mini level with the text in the file
		QString target;
		if (KIO::NetAccess::download(tmp, target, this)) {
			// Ok, that was successful, read the file into the first level of the lecture
			QFile infile(target);
			if ( !infile.open( QIODevice::ReadOnly ) ) {
				// FIXME : KIO::NetAccess::removeTempFile(target);
				return;
			}
			QTextStream in(&infile);
			// read text in stringlist
			int max_len = 0;
			int max_lines = 0;
			QString line = in.readLine();
			QStringList lines;
			while (!line.isNull() && ++max_lines < 1000) {
				lines.append(line.trimmed());
				max_len = qMax(max_len, line.length());
				line = in.readLine();
			}
			// store the lecture data
			lec.setTitle(i18n("Imported text from file '%1'").arg(tmp.fileName()));
			KTouchLevelData dat(i18n("generated from text file"), i18n("all available"));
			dat.setLines(lines);
			lec.setLevel(0, dat);
		
			// first store training statistics
			m_trainer->storeTrainingStatistics();
			Prefs::setCurrentLectureFile(tmp.url());
			m_lecture = lec;
			updateFontFromLecture();
			// adjust check marks in quick-select menus
			updateLectureActionCheck();
			// restart training session from level 1 here...
			m_trainer->startTraining(false);
			m_trainingPause->setEnabled(true);
		}
		KIO::NetAccess::removeTempFile(target);
	}
}
// ----------------------------------------------------------------------------

// The action File->Open lecture...
void KTouch::fileOpenLecture() {
	trainingPause();
	KUrl tmp = KFileDialog::getOpenUrl(QString(), QString(), this, i18n("Select Training Lecture File") );
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

// The action File->Edit colors...
void KTouch::fileEditColors() {
	trainingPause();
	// Create a copy of the currently editable color schemes.
	QList<KTouchColorScheme> tmp_list;
	int default_schemes = 0;
	for (QList<KTouchColorScheme>::const_iterator it = KTouchColorScheme::m_colorSchemes.constBegin();
		it != KTouchColorScheme::m_colorSchemes.constEnd(); ++it)
	{
		if (!it->m_default) 	tmp_list.append(*it);
		else 					++default_schemes;
	}
	
	KTouchColorEditor dlg(this);		// Create editor
	// start editor
	int selected;
	dlg.startEditor( tmp_list, Prefs::currentColorScheme() - default_schemes, selected);
	KTouchColorScheme::createDefaults();
	for (QList<KTouchColorScheme>::const_iterator it = tmp_list.constBegin();
		it != tmp_list.constEnd(); ++it)
	{
		KTouchColorScheme::m_colorSchemes.append(*it);
	}
	// update the quick select menu
    QStringList schemes_list;
    for (unsigned int i=0; i<KTouchColorScheme::m_colorSchemes.count(); ++i)
		schemes_list.append(KTouchColorScheme::m_colorSchemes[i].m_name);
    m_keyboardColorAction->setItems(schemes_list);
	int index = selected + default_schemes;
	if (index >=0 && index < static_cast<int>(KTouchColorScheme::m_colorSchemes.count())) {
		Prefs::setCurrentColorScheme(index);
	}
	else {
		Prefs::setCurrentColorScheme(1); // fall back on default in case active was deleted
	}
   	m_keyboardColorAction->setCurrentItem(Prefs::currentColorScheme());
	applyPreferences();
}
// ----------------------------------------------------------------------------

// The action File->Edit keyboard...
void KTouch::fileEditKeyboard() {
	trainingPause();
	// Create and execute editor
//    KTouchKeyboardEditor dlg(this);
//    dlg.startEditor( Prefs::currentKeyboardFile() );
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
		i18n("Start New Training Session"),KGuiItem(i18n("Keep Current Level")),KGuiItem(i18n("Do Not Keep")));
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
    dlg.run(Prefs::currentLectureFile(), m_userStats[Prefs::currentUserName()], kls, kss);
}
// ----------------------------------------------------------------------------

void KTouch::optionsPreferences() {

    trainingPause();
    if ( KConfigDialog::showDialog( "settings" ) )
        return;

    // KConfigDialog didn't find an instance of this dialog, so lets create it :
    KConfigDialog* dialog = new KConfigDialog(this, "settings",  Prefs::self());

    m_pageGeneral = new KTouchPrefGeneral(dialog);
    dialog->addPage(m_pageGeneral, i18n("General Options"), "style","","");

    m_pageTraining = new KTouchPrefTraining(dialog);
    dialog->addPage(m_pageTraining, i18n("Training Options"), "kalarm");

    m_pageKeyboard = new KTouchPrefKeyboard(dialog);
    dialog->addPage(m_pageKeyboard, i18n("Keyboard Settings"), "keyboard_layout");

    KTouchPrefColors *m_pageColors = new KTouchPrefColors(dialog);
    dialog->addPage(m_pageColors, i18n("Color Settings"), "package_graphics");

    connect(dialog, SIGNAL(settingsChanged(const QString &)), this, SLOT(applyPreferences()));
    // TODO : Connect some other buttons/check boxes of the dialog
/*
	connect(m_pageGeneral->kcfg_OverrideLectureFont, SIGNAL(toggled(bool)), 
		this, SLOT(configOverrideLectureFontToggled(bool)));
	connect(m_pageKeyboard->kcfg_OverrideKeyboardFont, SIGNAL(toggled(bool)), 
		this, SLOT(configOverrideKeyboardFontToggled(bool)));
	connect(m_pageTraining->kcfg_AutoLevelChange, SIGNAL(toggled(bool)), 
		this, SLOT(configAutoLevelChangeToggled(bool)));
	connect(m_pageColors->kcfg_CommonTypingLineColors, SIGNAL(toggled(bool)),
		this, SLOT(configCommonColorsToggled(bool)));
*/

    // call the functions to enable/disable controls depending on settings
//    configOverrideLectureFontToggled(Prefs::overrideLectureFont());
//    configOverrideKeyboardFontToggled(Prefs::overrideKeyboardFont());
//    configAutoLevelChangeToggled(Prefs::autoLevelChange());
//	configCommonColorsToggled(Prefs::commonTypingLineColors());

    dialog->show();
}
// ----------------------------------------------------------------------------

void KTouch::optionsSetupUsers() {
	KTouchUserSetup dlg(this);
	QStringList users = m_userStats.keys();
	// call setup users dialog
	dlg.showDialog(users);
	// loop over all users and store only the statistics data already present in our
	// statistics map
	QMap<QString, KTouchStatisticsData> new_stats;
	for (QStringList::const_iterator it = users.constBegin(); it != users.constEnd(); ++it) {
		new_stats[*it] = m_userStats[*it];
		new_stats[*it].m_userName = *it;
//		kDebug() << "User = " << *it << endl;
	}
	m_userStats = new_stats;
    m_currentUserAction->setItems(m_userStats.keys());
	updateCurrentUserActionCheck();
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
    if (static_cast<unsigned int>(num)>=m_keyboardFiles.count()) return;
    Prefs::setCurrentKeyboardFile( m_keyboardFiles[num] );
	//kDebug() << "[KTouch::changeKeyboard]  new keyboard layout = " << Prefs::currentKeyboardFile() << endl;
    m_keyboardLayoutAction->setCurrentItem(num);
	// call Apply-Preferenzes in "noisy"-mode, pop up an error if the chosen layout file is corrupt
    m_keyboardWidget->applyPreferences(this, false);
}
// ----------------------------------------------------------------------------

void KTouch::changeColor(int num) {
    if (static_cast<unsigned int>(num)>=KTouchColorScheme::m_colorSchemes.count()) return;
    Prefs::setCurrentColorScheme(num);
    m_keyboardWidget->applyPreferences(this, false);
	m_slideLineWidget->applyPreferences();
}
// ----------------------------------------------------------------------------

void KTouch::changeLecture(int num) {
    if (static_cast<unsigned int>(num)>=m_lectureFiles.count()) return;
    trainingPause();
	KTouchLecture l;
	QString fileName = m_lectureFiles[num];
    if (!l.loadXML(this, KUrl::fromPathOrUrl(fileName))) {
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

void KTouch::changeUser(int num) {
    if (static_cast<unsigned int>(num)>=m_userStats.keys().count()) return;
    trainingPause();
	m_trainer->storeTrainingStatistics();
	QString new_name = m_userStats.keys()[num];
	Prefs::setCurrentUserName(new_name);
    KMessageBox::information(this, i18n("Changing user to '%1'. Restarting training session at current level.").arg(new_name) );
	m_trainer->startTraining(true);
}
// ----------------------------------------------------------------------------


// *********************************
// *** Protected member function ***
// *********************************

bool KTouch::queryExit() {
	if (m_duringStartup) return true;
	// store config data
	Prefs::setCurrentTrainingLevel( m_trainer->m_level );
    Prefs::writeConfig();
	// update and save statistics
	m_trainer->storeTrainingStatistics();
	KUrl stat_file = KGlobal::dirs()->saveLocation("data","ktouch", true) + "statistics.xml";
	// make sure all stats have the user names
//	for (QStringList::iterator it = m_userStats.keys().begin(); it != m_userStats.keys().end(); ++it)
//		m_userStats[*it].m_userName = *it;
	KTouchStatisticsData::writeUserStats(this, stat_file, m_userStats);
	//m_userStats[Prefs::currentUserName()].write(this, stat_file);
	KUrl color_file = KGlobal::dirs()->saveLocation("data","ktouch", true) + "color_schemes.xml";
	KTouchColorScheme::writeList(this, color_file);
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

	if (Prefs::currentLectureFile() == "default") {
		Prefs::setCurrentLectureFile(QString::null);
//		/// \todo look up a lecture in the language of the KDE locale
/*		QString default_lecture = "default";
		if (m_lectureFiles.count() > 0)  default_lecture = m_lectureFiles[0];
		Prefs::setCurrentLectureFile( default_lecture );
*/
	}

    // if keyboard layout (loaded by Prefs is not available (e.g. the 
    // layout file has been deleted) switch to default keyboard
    if (m_keyboardFiles.contains(Prefs::currentKeyboardFile() )==0) {
        QString default_keyboard;
        // determine locale
        QString lang = KGlobal::locale()->language();
        QString fname = lang + ".keyboard";
        // try to find keyboard with current locale
        QStringList::const_iterator it = m_keyboardFiles.constBegin();
        while (it != m_keyboardFiles.constEnd()   &&   (*it).find(fname) == -1)  ++it;
        if (it == m_keyboardFiles.constEnd()) {
            fname = lang.left(2) + ".keyboard";
            // try to find more general version 
            it = m_keyboardFiles.constBegin();
            while (it != m_keyboardFiles.constEnd()   &&   (*it).find(fname) == -1)  ++it;
        }

        if (it != m_keyboardFiles.constEnd())
            default_keyboard = *it;
        else 
            default_keyboard = "number.keyboard";
        Prefs::setCurrentKeyboardFile ( default_keyboard );
    }

    // create some default colour schemes
    KTouchColorScheme::createDefaults();
    // read additional color schemes
	KUrl color_file = KGlobal::dirs()->findResource("data", "ktouch/color_schemes.xml");
	KTouchColorScheme::readList(this, color_file);
}
// ----------------------------------------------------------------------------

// Creates the layout and GUI setup for a practice session
void KTouch::initTrainingSession() {
	//kDebug() << "[KTouch::initTrainingSession]  setting up layouts and widgets for new training session..." << endl;
    // Build the training area. The status widget has a fixed vertical size, the slide line and the
    // keyboard grow according to their vertical stretch factors (see last argument in the constructors
    // of QSizePolicy)

    QWidget *main = new QWidget();
    QVBoxLayout * layout = new QVBoxLayout( this );
    main->setLayout(layout);

    m_statusWidget = new KTouchStatus( this );
    m_slideLineWidget = new KTouchSlideLine( this );
    m_slideLineWidget->setSizePolicy( QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding, 0, 1) );
    m_keyboardWidget = new KTouchKeyboardWidget( this );
    m_keyboardWidget->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding, 0, 3) );

    layout->addWidget(m_statusWidget);
    layout->addWidget(m_slideLineWidget);
    layout->addWidget(m_keyboardWidget);

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

    this->setCentralWidget(main);
}
// ----------------------------------------------------------------------------

// Creates the (standard) actions and entries in the menu.
void KTouch::setupActions() {
	// *** File menu ***
    KAction *action = new KAction(KIcon("open_plaintext"), i18n("&Open plain text file..."), actionCollection(), "file_opentext");
    connect(action, SIGNAL(triggered(bool)), SLOT(fileOpenText()));
    action = new KAction(KIcon("open_lecture"), i18n("&Open lecture..."), actionCollection(), "file_openlecture");
    connect(action, SIGNAL(triggered(bool)), SLOT(fileOpenLecture()));
    action = new KAction(KIcon("edit_lecture"), i18n("&Edit lecture..."), actionCollection(), "file_editlecture");
    connect(action, SIGNAL(triggered(bool)), SLOT(fileEditLecture()));
    action = new KAction(KIcon("edit_colors"), i18n("&Edit color scheme..."), actionCollection(), "file_editcolors");
    connect(action, SIGNAL(triggered(bool)), SLOT(fileEditColors()));
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
    m_defaultLectureAction = new KSelectAction(i18n("Default &Lectures"), actionCollection(), "default_lectures");
	m_defaultLectureAction->setMenuAccelsEnabled(false);
    m_defaultLectureAction->setItems(m_lectureTitles);
    m_defaultLectureAction->setCurrentItem(0);
    connect (m_defaultLectureAction, SIGNAL(activated(int)), this, SLOT(changeLecture(int)));

	// *** Options menu ***
    KStdAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
    // Setup menu entries for keyboard layouts
    m_keyboardLayoutAction = new KSelectAction(i18n("&Keyboard Layouts"), actionCollection(), "keyboard_layouts");
	m_keyboardLayoutAction->setMenuAccelsEnabled(false);
    m_keyboardLayoutAction->setItems(m_keyboardTitles);
    connect (m_keyboardLayoutAction, SIGNAL(activated(int)), this, SLOT(changeKeyboard(int)));

	// Setup menu entries for colour schemes
	m_keyboardColorAction = new KSelectAction(i18n("Keyboards &Color Schemes"), actionCollection(), "keyboard_schemes");
    QStringList schemes_list;
    for (unsigned int i=0; i<KTouchColorScheme::m_colorSchemes.count(); ++i)
		schemes_list.append(KTouchColorScheme::m_colorSchemes[i].m_name);
	m_keyboardColorAction->setMenuAccelsEnabled(false);
    m_keyboardColorAction->setItems(schemes_list);
	if (static_cast<unsigned int>(Prefs::currentColorScheme()) >=  schemes_list.count())
		Prefs::setCurrentColorScheme(1);
   	m_keyboardColorAction->setCurrentItem(Prefs::currentColorScheme());
    connect (m_keyboardColorAction, SIGNAL(activated(int)), this, SLOT(changeColor(int)));

    // *** User settings ***
    action = new KAction(KIcon("kdmconfig"), i18n("&Setup users..."), actionCollection(), "settings_setup_users");
    connect(action, SIGNAL(triggered(bool)), SLOT(optionsSetupUsers()));

	m_currentUserAction = new KSelectAction(i18n("&Current user"), actionCollection(), "settings_current_user");
	m_currentUserAction->setMenuAccelsEnabled(false);
    m_currentUserAction->setItems(m_userStats.keys());
	updateCurrentUserActionCheck();
    connect (m_currentUserAction, SIGNAL(activated(int)), this, SLOT(changeUser(int)));
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
		else if (f.fromString("Monospace")) 		m_slideLineWidget->setFont(f);
	}
}
// ----------------------------------------------------------------------------

// This function populates the file lists with the installed training, keyboard and
// examination files.
void KTouch::updateFileLists() {
    KStandardDirs *dirs = KGlobal::dirs();

    // first search for all installed keyboard files
	// TODO : search in i18n() directories
    m_keyboardFiles = dirs->findAllResources("data","ktouch/*.keyboard");

    // remove the number layout, since this is the necessary default layout and will be
    // added anyway
    QStringList::iterator it = m_keyboardFiles.find("number.keyboard");
	if (it!=m_keyboardFiles.end())		m_keyboardFiles.remove(it);

    m_keyboardTitles.clear();
    for (QStringList::const_iterator cit = m_keyboardFiles.constBegin();
        cit != m_keyboardFiles.constEnd(); ++cit)
    {
        // extract titles from keyboard files and store them in the
        // m_keyboardTitles string list

        // get the filename alone
        QString fname = KUrl(*cit).fileName();
        // get the filename without the .keyboard
        fname.truncate(fname.length() - 9);
        // get everything in front of the first .
        QString lang_iso = fname.section('.',0,0);
        // get language description of file names
        QString lang_name = KGlobal::locale()->twoAlphaToLanguageName(lang_iso);
//        kDebug() << fname << " | " << lang_iso << " | " << lang_name << endl;
        if (lang_name.isEmpty())
            lang_name = KGlobal::locale()->twoAlphaToCountryName(lang_iso);
        if (!lang_name.isEmpty())
            lang_name += " (" + fname + ")";
        else
            lang_name = fname;
        m_keyboardTitles.append( lang_name );
//        kDebug() << m_keyboardTitles.back() << endl;
    }

    // now sort the files and titles accordingly
    sort_lists(m_keyboardTitles, m_keyboardFiles);
    // and add the number keypad to the front
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
            KUrl url(KUrl::fromPath(*it));
			KTouchLecture l;
			// only add lecture if we can actually load it
			if (l.loadXML(this, url)) {
				// since we could read the lecture, we remember the URL
				m_lectureFiles.push_back(*it);
				// store the title of the lecture
				if (l.title().isEmpty())
					m_lectureTitles.push_back(i18n("untitled lecture") + " - (" + url.fileName() + ")");
            	else
					m_lectureTitles.push_back(l.title());
			}
        }
        sort_lists(m_lectureTitles, m_lectureFiles);
    }

	// Now find predefined files with colour schemes
    QStringList colour_schemes = dirs->findAllResources("data","ktouch/*.colour_scheme");
	// TODO : read in colour schemes and populate QValueList<KTouchColorScheme>
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

void KTouch::updateCurrentUserActionCheck() {
    QStringList user_list = m_userStats.keys();
	QStringList::const_iterator it = user_list.find(Prefs::currentUserName());
	// if not found, fall back on default user
	if (it == user_list.end()) {
		Prefs::setCurrentUserName(i18n("default user"));
		it = user_list.find(Prefs::currentUserName());
	}
	QStringList::const_iterator it2 = user_list.begin();
	// find index
	unsigned int index = 0;
	while (it2 != it) {
		++index;
		++it2;
	}
	if (index >= user_list.count()) {
		// we must be missing the default user in the list,
		// this shouldn't happen, though...
		kDebug() << "Missing default user in list..." << endl;
		user_list.append(i18n("default user"));
		index = user_list.count() -1;
	}
   	m_currentUserAction->setCurrentItem(index);
}
// ----------------------------------------------------------------------------

/*
void KTouch::imStartEvent(QIMEvent *e) {
	kDebug() << "[KTouch::imStartEvent]  text = '" << e->text() << "'" << endl;
  e->accept();
}
// ----------------------------------------------------------------------------

void KTouch::imComposeEvent(QIMEvent *e) {
	kDebug() << "[KTouch::imComposeEvent]  text = '" << e->text() << "'" << endl;
  e->accept();
}
// ----------------------------------------------------------------------------

void KTouch::imEndEvent(QIMEvent *e) {
	kDebug() << "[KTouch::imEndEvent]  text = '" << e->text() << "'" << endl;
  if (!e->text().isEmpty()) {
    if (e->text() == "^") {
      QKeyEvent *ev = new QKeyEvent (QEvent::KeyPress,
                                    Qt::Key_AsciiCircum, '^', 0,
                                    QString("^"));
      keyPressEvent(ev);
      delete ev;
    }
  }
  e->accept();
}
// ----------------------------------------------------------------------------
*/

