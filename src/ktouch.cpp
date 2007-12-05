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

// Qt Header
#include <QtXml>

// KDE Header
#include <kselectaction.h>
#include <kactioncollection.h>
#include <kstandardaction.h>
#include <klocale.h>
#include <kstatusbar.h>
#include <kfiledialog.h>
#include <kaction.h>
#include <kstandarddirs.h>
#include <kmessagebox.h>
#include <kdebug.h>
#include <kmenu.h>
#include <kconfigdialog.h>

#include <kcombobox.h>
#include <kfontrequester.h>
#include <knuminput.h>
#include <kicon.h>
#include <kio/netaccess.h>

// KTouch Header
#include "ktouchlecture.h"
#include "ktouchtrainer.h"

#include "ktouchstatuswidget.h"
#include "ktouchkeyboardwidget.h"
#include "ktouchtextlinewidget.h"

#include "ktouchlectureeditordialog.h"
#include "ktouchkeyboardeditordialog.h"
#include "ktouchcoloreditordialog.h"
#include "ktouchstatisticsdialog.h"
#include "ktouchusersetupdialog.h"

#include "ktouchprefgeneral.h"
#include "ktouchpreftraining.h"
#include "ktouchprefkeyboard.h"
#include "ktouchprefcolors.h"
#include "ktouchprefsound.h"

#include "prefs.h"
#include "ktouchcolorscheme.h"
#include "ktouchutils.h"

KTouch * KTouchPtr = NULL;

KTouch::KTouch() :
    m_statusWidget(NULL),
    m_keyboardWidget(NULL),
    m_trainer(NULL)
{
    setFocusPolicy(Qt::StrongFocus);

    setAttribute(Qt::WA_InputMethodEnabled, true);
 //   setAttribute(Qt::WA_KeyCompression );

    resetInputContext ();

	// Set global KTouchPtr to the main KTouch Object
	KTouchPtr = this;

	// General initialization of the program, common for all start modes
	init();

	// *** Read and initialize user statistics ***

	// first clear statistics data (also make sure our mapping contains
	// the "default" user)
	clearStatistics();
	KUrl stat_file = KGlobal::dirs()->findResource("data", "ktouch/statistics.xml");
	// read previous stats from file
	KTouchStatisticsData::readUserStats(this, stat_file, m_userStats);

	// Setup our actions and connections
	setupActions();

	// Init a training session
	initTrainingSession();

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
		//kDebug() << "[KTouch::KTouch]  lecture file = " << Prefs::currentLectureFile();
	}

    statusBarLevel = new QLabel(this);
    statusBar()->addPermanentWidget(statusBarLevel);

    statusBarSession = new QLabel(this);
    statusBar()->addPermanentWidget(statusBarSession);

    setupGUI();

    // Adjust check mark for the keyboard file
    updateKeyboardActionCheck();
    // If the user doesn't want to restart with his old level, start from 0 (level 1)
    if (!Prefs::rememberLevel())
        m_trainer->m_level = 0;
    else
        m_trainer->m_level = Prefs::currentTrainingLevel();
    // now let's start the training in the current level
    m_trainer->startTraining(true);

    setFocus();
}
// ----------------------------------------------------------------------------

// Free memory of objects that are not owned by the main KTouch object
KTouch::~KTouch() {
	delete m_trainer;
	m_trainer = NULL;
}
// ----------------------------------------------------------------------------

KTouchLectureStats& KTouch::getCurrentLectureStats() {
//	kDebug() << "[KTouch::getCurrentLectureStats]  ";
	KUrl lectureURL = Prefs::currentLectureFile();
	if (lectureURL.url().isEmpty())  lectureURL = "default";
//	kDebug() << "  lectureURL = '" << lectureURL << "'";
	KTouchLectureStats& stat = m_userStats[Prefs::currentUserName()].m_lectureStats[lectureURL];
	// add additional info to the statistics
	stat.m_lectureURL = lectureURL;
	stat.m_lectureTitle = m_lecture.title();
	return stat;
}
// ----------------------------------------------------------------------------

void KTouch::clearStatistics() {
	m_userStats.clear();
	m_userStats[Prefs::currentUserName()].m_userName = i18n("Default User");
}
// ----------------------------------------------------------------------------

// ********************
// *** Public slots ***
// ********************

void KTouch::applyPreferences() {
	// This applies a new color scheme for the keyboard and also updates all other
	// changes for the keyboard widget
	changeColor(Prefs::currentColorScheme());
	m_textLineWidget->applyPreferences();
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

    QChar key = keyEvent->text().at(0); // get first unicode character

    if (key.isPrint())
        m_trainer->keyPressed(key);
    else if (key==QChar(8))
        m_trainer->backspacePressed();
    else if (key==QChar(13))
        m_trainer->enterPressed();
    else
    {
        keyEvent->ignore(); // unrecognised char -> don't accept it! Maybe the key is for somebody else?
        return;
    }
    keyEvent->accept();
}
// -----------------------------------------------------------------------------

void KTouch::inputMethodEvent( QInputMethodEvent* m ){
    m_trainer->keyPressed(m->commitString().at(0));
    m->accept();
}
// -----------------------------------------------------------------------------

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
			lec.setTitle(i18n("Imported text from file '%1'", tmp.fileName()));
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
    KTouchLectureEditorDialog dlg(this);
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

	KTouchColorEditorDialog dlg(this);		// Create editor
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
    for (int i=0; i<KTouchColorScheme::m_colorSchemes.count(); ++i)
		schemes_list.append(KTouchColorScheme::m_colorSchemes[i].m_name);
    m_keyboardColorAction->setItems(schemes_list);
	int index = selected + default_schemes;
	if (index >=0 && index < KTouchColorScheme::m_colorSchemes.count()) {
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
    KTouchKeyboardEditorDialog dlg(this);
    if (dlg.startEditor( Prefs::currentKeyboardFile() )) {
		// Reload keyboard in case it was modified
		m_keyboardWidget->applyPreferences(this, false);
		// adjust check marks in quick-select menus
		updateKeyboardActionCheck();
	}
}
// ----------------------------------------------------------------------------

void KTouch::fileQuit() {
    qApp->quit();
}
// ----------------------------------------------------------------------------

void KTouch::trainingNewSession() {
    trainingPause();
	int result = KMessageBox::questionYesNoCancel(this,
		i18n("Would you like to keep the current level for the new training session?"),
		i18n("Start new training session"),KGuiItem(i18n("Keep current level")),KGuiItem(i18n("Start from first level")));
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
	KTouchStatisticsDialog dlg(this);
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
    dialog->addPage(m_pageGeneral, i18n("General Options"), "configure-toolbars","","");

    m_pageTraining = new KTouchPrefTraining(dialog);
    dialog->addPage(m_pageTraining, i18n("Training Options"), "chronometer");

    m_pageKeyboard = new KTouchPrefKeyboard(dialog);
    dialog->addPage(m_pageKeyboard, i18n("Keyboard Settings"), "preferences-desktop-keyboard");

    m_pageColors = new KTouchPrefColors(dialog);
    dialog->addPage(m_pageColors, i18n("Color Settings"), "preferences-desktop-color");

    m_pageSound = new KTouchPrefSound(dialog);
    dialog->addPage(m_pageSound, i18n("Sound Settings"), "preferences-desktop-sound");

    connect(dialog, SIGNAL(settingsChanged(const QString &)), this, SLOT(applyPreferences()));

    dialog->show();
}
// ----------------------------------------------------------------------------

void KTouch::optionsSetupUsers() {
	KTouchUserSetupDialog dlg(this);
	QStringList users = m_userStats.keys();
	// call setup users dialog
	dlg.showDialog(users);
	// loop over all users and store only the statistics data already present in our
	// statistics map
	QMap<QString, KTouchStatisticsData> new_stats;
	for (QStringList::const_iterator it = users.constBegin(); it != users.constEnd(); ++it) {
		new_stats[*it] = m_userStats[*it];
		new_stats[*it].m_userName = *it;
	}
	m_userStats = new_stats;
    m_currentUserAction->setItems(m_userStats.keys());
	updateCurrentUserActionCheck();
}
// ----------------------------------------------------------------------------

void KTouch::changeStatusbarMessage(const QString& text) {
    statusBar()->showMessage(text);
}
// ----------------------------------------------------------------------------

void KTouch::changeStatusbarStats(unsigned int level_correct, unsigned int level_total, unsigned int level_words,
	unsigned int session_correct, unsigned int session_total, unsigned int session_words)
{
    statusBarLevel->setText(i18n( "Level:  Correct/Total chars: %1/%2  Words: %3", level_correct, level_total, level_words));
    statusBarSession->setText(i18n( "Session: Correct/Total chars: %1/%2  Words: %3", session_correct, session_total, session_words));
}
// ----------------------------------------------------------------------------

void KTouch::changeKeyboard(int num) {
    if (num > m_keyboardFiles.count()) return;
	if (num == m_keyboardFiles.count()) {
		// TODO : open dialog with user provided keyboard files
		return;
	}
	if (m_keyboardFiles[num] != Prefs::currentKeyboardFile()) {
		KMessageBox::information(this, i18n("Note that in different countries touch typing is tought slightly different which "
			"usually affects only the top row of keys. For instance, in the United States only the leftmost key and the key "
			"with the 1 are pressed by the little finger of the left hand. In Germany the little "
			"finger also presses the 2 key, and thus the fingers shift one key to the right on the top row.\n"
			"Normally this only makes a difference for split or ergonomic keyboards."), QString(), "KeyboardLayoutChangeInfo");
		Prefs::setCurrentKeyboardFile( m_keyboardFiles[num] );
		//kDebug() << "[KTouch::changeKeyboard]  new keyboard layout = " << Prefs::currentKeyboardFile();
		m_keyboardLayoutAction->setCurrentItem(num);
		// call Apply-Preferenzes in "noisy"-mode, pop up an error if the chosen layout file is corrupt
		m_keyboardWidget->applyPreferences(this, false);
	}
}
// ----------------------------------------------------------------------------

void KTouch::changeColor(int num) {
    if (num >= KTouchColorScheme::m_colorSchemes.count()) return;
    Prefs::setCurrentColorScheme(num);
    m_keyboardWidget->applyPreferences(this, false);
	m_textLineWidget->applyPreferences();
}
// ----------------------------------------------------------------------------

void KTouch::changeLecture(int num) {
    if (num >= m_lectureFiles.count()) return;
    trainingPause();
	KTouchLecture l;
	QString fileName = m_lectureFiles[num];
    if (!l.loadXML(this, KUrl(fileName))) {
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
    if (num >= m_userStats.keys().count()) return;
    trainingPause();
	m_trainer->storeTrainingStatistics();
	QString new_name = m_userStats.keys()[num];
	Prefs::setCurrentUserName(new_name);
    KMessageBox::information(this, i18n("Changing user to '%1'. Restarting training session at current level.", new_name) );
	m_trainer->startTraining(true);
}
// ----------------------------------------------------------------------------


// *********************************
// *** Protected member function ***
// *********************************

bool KTouch::queryExit() {
	// store config data
	Prefs::setCurrentTrainingLevel( m_trainer->m_level );
    Prefs::self()->writeConfig();
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
	KXmlGuiWindow::resizeEvent(event);
}
// ----------------------------------------------------------------------------


// *******************************
// *** Private member function ***
// *******************************


// Initialises the program during a normal startup
void KTouch::init() {
	//kDebug() << "[KTouch::init]  populating file lists...";
    updateFileLists();  // create lists with default lecture/keyboard/examination files/colour scheme files
	//kDebug() << "[KTouch::init]  " << m_lectureFiles.count() << " lectures available";
	//kDebug() << "[KTouch::init]  " << m_keyboardFiles.count() << " keyboard layouts available";
	//kDebug() << "[KTouch::init]  " << m_examinationFiles.count() << " examination files available";

	if (Prefs::currentLectureFile() == "default") {
		Prefs::setCurrentLectureFile(QString());
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
        while (it != m_keyboardFiles.constEnd()   &&   (*it).indexOf(fname) == -1)  ++it;
        if (it == m_keyboardFiles.constEnd()) {
            fname = lang.left(2) + ".keyboard";
            // try to find more general version
            it = m_keyboardFiles.constBegin();
            while (it != m_keyboardFiles.constEnd()   &&   (*it).indexOf(fname) == -1)  ++it;
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
	//kDebug() << "[KTouch::initTrainingSession]  setting up layouts and widgets for new training session...";
    // Build the training area. The status widget has a fixed vertical size, the slide line and the
    // keyboard grow according to their vertical stretch factors (see last argument in the constructors
    // of QSizePolicy)

    QWidget *main = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(main);

    m_statusWidget = new KTouchStatusWidget( main );
    m_textLineWidget = new KTouchTextLineWidget( main );
	QSizePolicy sp(QSizePolicy::Preferred, QSizePolicy::Expanding);
	sp.setVerticalStretch(1);
    sp.setHorizontalStretch(10);
    m_textLineWidget->setSizePolicy( sp );
    m_keyboardWidget = new KTouchKeyboardWidget( main );
	sp.setVerticalStretch(3);
    m_keyboardWidget->setSizePolicy( sp );

    layout->addWidget(m_statusWidget);
    layout->addWidget(m_textLineWidget);
    layout->addWidget(m_keyboardWidget);

    // apply the settings to the widgets
    m_textLineWidget->applyPreferences();
    m_keyboardWidget->applyPreferences(this, true);  // set preferences silently here

    // create our trainer, the master object for the training stuff...
    if (m_trainer != NULL)  delete m_trainer;
    m_trainer = new KTouchTrainer(m_statusWidget, m_textLineWidget, m_keyboardWidget, &m_lecture);

    setCentralWidget(main);
}
// ----------------------------------------------------------------------------

// Creates the (standard) actions and entries in the menu.
void KTouch::setupActions() {
	// here we create actions for the menu and toolbar
	// the placement is defined in the ktouchui.rc file

	// *** File menu ***
    QAction *action;
    action = actionCollection()->addAction( "file_opentext" );
    action->setText( i18n("&Open Plain Text File...") );
    action->setIcon( KIcon("document-open") );

    connect(action, SIGNAL(triggered(bool)), SLOT(fileOpenText()));
    action = actionCollection()->addAction( "file_openlecture" );
    action->setText( i18n("&Open Lecture...") );
    action->setIcon( KIcon("document-open") );
    connect(action, SIGNAL(triggered(bool)), SLOT(fileOpenLecture()));

    action = actionCollection()->addAction( "file_editlecture" );
    action->setText( i18n("&Edit Lecture...") );
    action->setIcon( KIcon("object-edit") );
    connect(action, SIGNAL(triggered(bool)), SLOT(fileEditLecture()));

    action = actionCollection()->addAction( "file_editcolors" );
    action->setText( i18n("&Edit Color Scheme...") );
    action->setIcon( KIcon("color-fill") );
    connect(action, SIGNAL(triggered(bool)), SLOT(fileEditColors()));

    action = actionCollection()->addAction( "file_editkeyboard" );
    action->setText( i18n("&Edit Keyboard Layout...") );
    action->setIcon( KIcon("object-edit") );
    connect(action, SIGNAL(triggered(bool)), SLOT(fileEditKeyboard()));

    KStandardAction::quit(this, SLOT(fileQuit()), actionCollection());

	// *** Training menu ***
    action = actionCollection()->addAction( "training_newsession" );
    action->setText( i18n("&Start New Session") );
    action->setIcon( KIcon("media-playback-start") );
    connect(action, SIGNAL(triggered(bool)), SLOT(trainingNewSession()));

    m_trainingPause=actionCollection()->addAction( "training_pause" );
    m_trainingPause->setText( i18n("&Pause Session") );
    m_trainingPause->setIcon(KIcon("media-playback-pause") );
    connect(m_trainingPause, SIGNAL(triggered(bool)), SLOT(trainingPause()));

    action = actionCollection()->addAction("training_stats" );
    action->setText( i18n("&Lecture Statistics") );
    action->setIcon( KIcon("datashowchart") );
    connect(action, SIGNAL(triggered(bool)), SLOT(trainingStatistics()));

    // Setup menu entries for the training lectures
    m_defaultLectureAction =  actionCollection()->add<KSelectAction>( "training_default_lectures" );
    m_defaultLectureAction->setText( i18n("Default &Lectures") );
    m_defaultLectureAction->setMenuAccelsEnabled(false);
    m_defaultLectureAction->setItems(m_lectureTitles);
    m_defaultLectureAction->setCurrentItem(0);
    connect (m_defaultLectureAction, SIGNAL(triggered(int)), this, SLOT(changeLecture(int)));

	// *** Options menu ***
    KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());
    // Setup menu entries for keyboard layouts
    m_keyboardLayoutAction= actionCollection()->add<KSelectAction>( "keyboard_layouts" );
    m_keyboardLayoutAction->setText( i18n("&Keyboard Layouts") );
    m_keyboardLayoutAction->setMenuAccelsEnabled(false);
//  TODO : add this back in when we have the KNewStuff feature implemented, were users
//         can anonymously upload their keyboards and lectures which will then be offered in
//         the diaolg that pops up for the "More keyboard layouts..." menu entry
//	m_keyboardTitles << i18n("More keyboard layouts...");
    m_keyboardLayoutAction->setItems(m_keyboardTitles);
    connect (m_keyboardLayoutAction, SIGNAL(triggered(int)), this, SLOT(changeKeyboard(int)));

	// Setup menu entries for colour schemes
    m_keyboardColorAction = actionCollection()->add<KSelectAction>( "keyboard_schemes" );
    m_keyboardColorAction->setText( i18n("Keyboards &Color Schemes") );
    QStringList schemes_list;
    for (int i=0; i<KTouchColorScheme::m_colorSchemes.count(); ++i)
		schemes_list.append(KTouchColorScheme::m_colorSchemes[i].m_name);
	m_keyboardColorAction->setMenuAccelsEnabled(false);
    m_keyboardColorAction->setItems(schemes_list);
	if (Prefs::currentColorScheme() >=  schemes_list.count())
		Prefs::setCurrentColorScheme(1);
   	m_keyboardColorAction->setCurrentItem(Prefs::currentColorScheme());
    connect (m_keyboardColorAction, SIGNAL(triggered(int)), this, SLOT(changeColor(int)));

    // *** User settings ***
    action = actionCollection()->addAction( "settings_setup_users" );
    action->setText( i18n("&Setup Users...") );
    action->setIcon( KIcon("users") );
    connect(action, SIGNAL(triggered(bool)), SLOT(optionsSetupUsers()));

    m_currentUserAction = actionCollection()->add<KSelectAction>( "settings_current_user" );
    m_currentUserAction->setText( i18n("&Current User") );
	m_currentUserAction->setMenuAccelsEnabled(false);
    m_currentUserAction->setItems(m_userStats.keys());
	updateCurrentUserActionCheck();
    connect (m_currentUserAction, SIGNAL(triggered(int)), this, SLOT(changeUser(int)));
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
		if (f.fromString(m_lecture.m_fontSuggestions))	m_textLineWidget->setFont(f);
		else if (f.fromString("Monospace")) 			m_textLineWidget->setFont(f);
	}
}
// ----------------------------------------------------------------------------

// This function populates the file lists with the installed training, keyboard and
// examination files.
void KTouch::updateFileLists() {
    // first search for all installed new format keyboard files
    KStandardDirs *dirs = KGlobal::dirs();
    m_keyboardFiles = dirs->findAllResources("data","ktouch/*.keyboard.xml");

	// extract titles from keyboard files and store them in the
	// m_keyboardTitles string list
    m_keyboardTitles.clear();
	kDebug() << "Keyboard files in XML format:";
    for (QStringList::const_iterator cit = m_keyboardFiles.constBegin();
        cit != m_keyboardFiles.constEnd(); ++cit)
    {
		KUrl url = (*cit);
    	QString target;
		// try to read language code and keyboard name from file
		if (!KIO::NetAccess::download(url, target, this)) continue;
		// Ok, that was successful, store the lectureURL and read the file
		QFile infile(target);
		if ( !infile.open( QIODevice::ReadOnly ) ) 	continue;
		QDomDocument doc;
		doc.setContent( &infile );
		QDomElement root = doc.documentElement();
		if (root.isNull() || root.tagName() != "KTouchKeyboard")  continue;
		
		// get the title
		QString title;
		QDomElement e = root.firstChildElement("Title");
		if (!e.isNull())	title = e.firstChild().nodeValue();
		else				title = i18n("untitled keyboard layout");
		QString langid;
		// retrieve the language id
		e = root.firstChildElement("Language");
		if (!e.isNull())	langid = e.firstChild().nodeValue();
		// compose title for main menu
		if (!langid.isEmpty())
			title = QString("%1 (%2)").arg(title).arg(langid);
        m_keyboardTitles.append( title );
        kDebug() << "  " << m_keyboardTitles.back() << "\t [" << url << "]";
    }

    // now sort the files and titles accordingly
    sort_lists(m_keyboardTitles, m_keyboardFiles);
    // and add the number keypad to the front

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
					m_lectureTitles.push_back(i18n("untitled lecture") + " - (" + url.fileName() + ')');
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
	int index = user_list.indexOf(Prefs::currentUserName());
	// if not found, fall back on default user
	if (index == -1) {
		Prefs::setCurrentUserName(i18n("Default User"));
		index = user_list.indexOf(Prefs::currentUserName());
	}
	if (index == -1) {
		// we must be missing the default user in the list,
		// this shouldn't happen, though...
		kDebug() << "Missing default user in list. Adding default user to list.";
		user_list.append(i18n("Default User"));
		index = user_list.count() -1;
	}
   	m_currentUserAction->setCurrentItem(index);
}
// ----------------------------------------------------------------------------
