/***************************************************************************
                          ktouch.cpp  -  description
                             -------------------
    begin                : Tue Mar 13 15:33:49 EST 2001
    copyright            : (C) 2001 by Haavard Froeiland
    email                : haavard@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


// include files for QT
#include <qdir.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qsizepolicy.h>
#include <qgroupbox.h>

// include files for KDE
#include <kapplication.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <kstdaction.h>
#include <qvbox.h>
#include <kdebug.h>

// application specific includes
#include "ktouch.h"
#include <kstatusbar.h>
#include <touchStatus.h>
#include <qpushbutton.h>

#define ID_STATUS_MSG 1

#include "ktouch.moc"

KTouch::KTouch():KMainWindow()
{
	config=kapp->config();
	dirs = KGlobal::dirs();

	touchLecture = new TouchLecture();
	touchStat    = new TouchStat();


//Object::connect(	touchTrain->line,    SIGNAL(isError(QChar)),
//				touchStat,    SLOT(gotError(QChar)));
//Object::connect(	touchTrain->line,    SIGNAL(isOk(QChar)),
//				touchStat,    SLOT(gotOk(QChar)));

//Object::connect(	touchTrain->status,  SIGNAL(levelUp()),
//				touchLecture, SLOT(levelUp()));
//Object::connect(	touchTrain->status,  SIGNAL(levelDown()),
//				touchLecture, SLOT(levelDown()));



//Object::connect(	touchTrain->status,  SIGNAL(stop()),
//				touchStat,    SLOT(stop()));
//Object::connect(	touchTrain->status,  SIGNAL(start()),
//				touchStat,    SLOT(start()));

/*Object::connect(	touchTrain->status,  SIGNAL(forceNextLine()),
						touchTrain->line,    SLOT(getNextLine()));
	QObject::connect(	touchLecture, SIGNAL(levelMessage(const QString&)),
						touchTrain->status,  SLOT(setLevelMessage(const QString&)));
	QObject::connect(	touchLecture, SIGNAL(levelChanged(int)),
						touchTrain->status,  SLOT(setLevel(int)));
	QObject::connect(	touchTrain->line,    SIGNAL(nextKey(const QChar&)),
						touchTrain->keyboard, SLOT(newKey(const QChar&)));

	QObject::connect(	touchTrain->status,  SIGNAL(showStat()),
						this,         SLOT(showStat()));
	QObject::connect(	touchTrain->status,  SIGNAL(hideStat()),
						this,         SLOT(hideStat()));
*/


	///////////////////////////////////////////////////////////////////
	// call inits to invoke all other construction parts
	initStatusBar();
	initActions();


	slotModeTrain();
	readOptions();
	touchLine->getNextLine();
}

KTouch::~KTouch()
{
  delete touchLecture;
  delete touchStat;
  delete touchStatus;
  delete touchLine;  
  delete touchKeyboard;
}


void KTouch::initActions()
{
	fileOpen = KStdAction::open(this, SLOT(slotFileOpen()), actionCollection());
	fileSave = KStdAction::save(this, SLOT(slotFileSave()), actionCollection());
	fileSaveAs = KStdAction::saveAs(this, SLOT(slotFileSaveAs()), actionCollection());
	fileOpenRecent = KStdAction::openRecent(this, SLOT(slotFileOpenRecent(const KURL&)), actionCollection());
	fileQuit = KStdAction::quit(this, SLOT(slotFileQuit()), actionCollection());
	viewToolBar = KStdAction::showToolbar(this, SLOT(slotViewToolBar()), actionCollection());
	viewStatusBar = KStdAction::showStatusbar(this, SLOT(slotViewStatusBar()), actionCollection());

	new KAction(i18n("Keyboard"),0,this,SLOT(slotOptionKeyboard()), actionCollection(),"optionKeyboard");
	new KAction(i18n("Training"),0,this,SLOT(slotOptionTraining()), actionCollection(),"optionTraining");
	new KAction(i18n("General"),0,this,SLOT(slotOptionGeneral()), actionCollection(),"optionGeneral");

	new KAction(i18n("Train"),1,this,SLOT(slotModeTrain()), actionCollection(),"train");
	new KAction(i18n("Edit"),2,this,SLOT(slotModeEdit()), actionCollection(),"edit");
	new KAction(i18n("Test"),3,this,SLOT(slotModeTest()), actionCollection(),"test");

	fileOpen->setStatusText(i18n("Opens an existing document"));
	fileOpenRecent->setStatusText(i18n("Opens a recently used file"));

	fileQuit->setStatusText(i18n("Quits the application"));
	viewToolBar->setStatusText(i18n("Enables/disables the toolbar"));
	viewStatusBar->setStatusText(i18n("Enables/disables the statusbar"));

	// use the absolute path to your ktouchui.rc file for testing purpose in createGUI();
	//createGUI("ktouchui.rc");
	createGUI();
}


void KTouch::initStatusBar()
{
	///////////////////////////////////////////////////////////////////
	// STATUSBAR
	// TODO: add your own items you need for displaying current application status.

	statusBar()->insertItem(i18n("Ready."), ID_STATUS_MSG);
}

void KTouch::openDocumentFile(const KURL& url)
{
	slotStatusMsg(i18n("Opening file..."));
	if(!url.isEmpty())
	{
		touchLecture->load(url.directory(false) + url.fileName());
		touchTrain->line->getNextLine();
		setCaption(url.fileName(), false);
		fileOpenRecent->addURL( url );
	}
	fileOpenRecent->addURL( url );
	slotStatusMsg(i18n("Ready."));
}

void KTouch::saveOptions()
{
	touchTrain->saveOptions();

	//** General Options ********************************************************
	config->setGroup("General Options");
	config->writeEntry("Geometry", size());
	config->writeEntry("Show Toolbar", viewToolBar->isChecked());
	config->writeEntry("Show Statusbar",viewStatusBar->isChecked());
	config->writeEntry("ToolBarPos", (int) toolBar("mainToolBar")->barPos());
	fileOpenRecent->saveEntries(config,"Recent Files");


}

void KTouch::readOptions()
{
	touchTrain->readOptions();


	//** General Options ********************************************************
	config->setGroup("General Options");

	// bar status settings
	bool bViewToolbar = config->readBoolEntry("Show Toolbar", false);
	viewToolBar->setChecked(bViewToolbar);
	slotViewToolBar();

	bool bViewStatusbar = config->readBoolEntry("Show Statusbar", true);
	viewStatusBar->setChecked(bViewStatusbar);
	slotViewStatusBar();

	// bar position settings
	KToolBar::BarPosition toolBarPos;
	toolBarPos=(KToolBar::BarPosition) config->readNumEntry("ToolBarPos", KToolBar::Top);
	toolBar("mainToolBar")->setBarPos(toolBarPos);

	// initialize the recent file list
	fileOpenRecent->loadEntries(config,"Recent Files");

	// set the size of KTouch
	QSize size(550,420);
	size=config->readSizeEntry("Geometry",&size);
	resize(size);

	// ** Recent files *******************************************************
	config->setGroup("Recent Files");
	KURL url=config->readEntry("File1",dirs->findResource("data","ktouch/english.ktouch"));

	touchLecture->load(url.directory(false,true)+url.fileName());
	
	touchTrain->line->getNextLine();

}

bool KTouch::queryExit()
{
	saveOptions();
	touchStat->saveStat();
	return true;
}

/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////

void KTouch::slotFileOpen()
{
	slotStatusMsg(i18n("Opening file..."));
	KURL url=KFileDialog::getOpenURL(
		dirs->findResourceDir("data","ktouch/english.ktouch")+"ktouch/",
		i18n("*.ktouch|KTouch training files"), this, i18n("Open File..."));
	if(!url.isEmpty())
	{
		touchLecture->load(url.directory(false) + url.fileName());
// Emit newFileSignal here
		touchTrain->line->getNextLine();
		setCaption(url.fileName(), false);
		fileOpenRecent->addURL( url );
	}

	slotStatusMsg(i18n("Ready."));
}

void KTouch::slotFileOpenRecent(const KURL& url)
{
	slotStatusMsg(i18n("Opening file..."));

	if(!url.isEmpty())
	{
		touchLecture->load(url.directory(false) + url.fileName());
// Emit newFileSignal here
		touchTrain->line->getNextLine();

		setCaption(url.fileName(), false);
		fileOpenRecent->addURL( url );
	}
	slotStatusMsg(i18n("Ready."));
}

void KTouch::slotFileSave()
{
	slotStatusMsg(i18n("saving..."));
	if(touchLecture->save()!= 0 )
	{
		QMessageBox::information( this,i18n("Saving training file"),
									i18n("Error when trying to save to file"));
	}
	slotStatusMsg(i18n("Ready."));
}

void KTouch::slotFileSaveAs()
{
	slotStatusMsg(i18n("Save file..."));
	KURL url=KFileDialog::getOpenURL(
		dirs->findResourceDir("data","ktouch/english.ktouch")+"ktouch/",
		i18n("*.ktouch|KTouch training files"), this, i18n("Save File..."));
	if(!url.isEmpty())
	{
		touchLecture->saveAs(url.directory(false) + url.fileName());
		setCaption(url.fileName(), false);
		fileOpenRecent->addURL( url );
	}

	slotStatusMsg(i18n("Ready."));
}

void KTouch::slotFileEdit()
{
	slotStatusMsg(i18n("editing..."));

}

void KTouch::slotFileQuit()
{
	slotStatusMsg(i18n("Exiting..."));
	close();
}

void KTouch::slotViewToolBar()
{
	slotStatusMsg(i18n("Toggling toolbar..."));
	///////////////////////////////////////////////////////////////////
	// turn Toolbar on or off
	if(!viewToolBar->isChecked())
	{
		toolBar("mainToolBar")->hide();
	}
	else
	{
		toolBar("mainToolBar")->show();
	}

	slotStatusMsg(i18n("Ready."));
}

void KTouch::slotViewStatusBar()
{
	slotStatusMsg(i18n("Toggle the statusbar..."));
	///////////////////////////////////////////////////////////////////
	//turn Statusbar on or off
	if(!viewStatusBar->isChecked())
	{
		statusBar()->hide();
	}
	else
	{
		statusBar()->show();
	}

	slotStatusMsg(i18n("Ready."));
}


void KTouch::slotStatusMsg(const QString &text)
{
	///////////////////////////////////////////////////////////////////
	// change status message permanently
	statusBar()->clear();
	statusBar()->changeItem(text, ID_STATUS_MSG);
}

void KTouch::slotOptionKeyboard()
{

	// Set up dialog
	OptionKeyboard *optionKeyboard=new OptionKeyboard(this,"keyboardOptionDialog",true);

	optionKeyboard->showColor->setChecked(touchTrain->keyboard->getShowColor());
	optionKeyboard->showAnimation->setChecked(touchTrain->keyboard->getShowAnimation());

	optionKeyboard->languageBox->setDisabled((touchTrain->keyboard->getLanguage()=="number"));
	optionKeyboard->showKeypad->setChecked((touchTrain->keyboard->getLanguage()=="number"));

	// need to load the keyboard files dynamically hear...

	QStringList keyboardList=QDir(dirs->findResourceDir("data","ktouch/number.keyboard") + "ktouch/","*.keyboard").entryList();

	keyboardList.sort();
	for ( QStringList::Iterator it = keyboardList.begin(); it != keyboardList.end(); ++it )
	{
		QString keyboardName=*it;
		optionKeyboard->language->insertItem(keyboardName.left(keyboardName.find(".keyboard")));
	}


	for(int i=0;optionKeyboard->language->count()>i;i++)
	{
		if(optionKeyboard->language->text(i)==touchTrain->keyboard->getLanguage())
		{
			optionKeyboard->language->setCurrentItem(i);
		}
	}

	if(optionKeyboard->exec())
	{
		// save changes made in dialog
		touchTrain->keyboard->setShowColor(optionKeyboard->showColor->isChecked());
		touchTrain->keyboard->setShowAnimation(optionKeyboard->showAnimation->isChecked());
		if(optionKeyboard->showKeypad->isChecked())
		{
			touchTrain->keyboard->loadKeyboard("number");

			//QString trainingFile=dirs->findResource("data","ktouch/number.ktouch");
			//touchLecture->loadLectureFile(trainingFile);
			KURL url(dirs->findResource("data","ktouch/number.keyboard"));
			openDocumentFile(url);
			slotFileOpenRecent(url);
		}
		else
		{
			touchTrain->keyboard->loadKeyboard(optionKeyboard->language->currentText());
			//QString trainingFile=dirs->findResource("data","ktouch/english.ktouch");
			//touchLecture->loadLectureFile(trainingFile);
			KURL url(dirs->findResource("data","ktouch/english.ktouch"));
			openDocumentFile(url);
			slotFileOpenRecent(url);
		}

	}
	delete optionKeyboard;
}

void KTouch::slotOptionTraining()
{

	// Set up dialog
	OptionTraining *optionTraining=new OptionTraining(this,"trainingOptionDialog",true);

	optionTraining->limitUp->setValue(touchTrain->status->getSpeedLimitUp());
	optionTraining->limitUp->setEnabled(touchTrain->status->autoLevel);
	optionTraining->limitDown->setValue(touchTrain->status->getSpeedLimitDown());
	optionTraining->limitDown->setEnabled(touchTrain->status->autoLevel);
//	optionTraining->remember->setChecked(remember);
	optionTraining->autoLevel->setChecked(touchTrain->status->autoLevel);

	if(optionTraining->exec())
	{
		// Save dialog changes
		touchTrain->status->setSpeedLimit(optionTraining->limitUp->value(),optionTraining->limitDown->value());
//		remember=optionTraining->remember->isChecked();
		touchTrain->status->autoLevel=optionTraining->autoLevel->isChecked();
		touchTrain->status->pushButtonLevelDown->setDisabled(touchTrain->status->autoLevel);
		touchTrain->status->pushButtonLevelUp->setDisabled(touchTrain->status->autoLevel);
	}
	delete optionTraining;

}

void KTouch::slotOptionGeneral()
{

	// Set up dialog
	OptionGeneral *optionGeneral=new OptionGeneral(this,"generalOptionDialog",true);

	optionGeneral->beepOnError->setChecked(touchTrain->status->errorSound);
	optionGeneral->soundOnLevel->setChecked(touchTrain->status->levelSound);
	optionGeneral->colorOnError->setChecked(touchTrain->line->getShowError());
	optionGeneral->showColor->setPalette(QPalette(touchTrain->line->getErrorColor()));
	optionGeneral->fontChooser->setFont(touchTrain->line->getFont());

	if(optionGeneral->exec())
	{
		// Save dialog changes
		touchTrain->status->errorSound=optionGeneral->beepOnError->isChecked();
		touchTrain->status->levelSound=optionGeneral->soundOnLevel->isChecked();
		touchTrain->line->setShowError(optionGeneral->colorOnError->isChecked());
		touchTrain->line->setErrorColor(optionGeneral->showColor->backgroundColor());
		touchTrain->line->setFont(optionGeneral->fontChooser->font());
	}
	delete optionGeneral;

}

void KTouch::keyPressEvent(QKeyEvent*e)
{
	//touchTrain->line->keyPressed(e->ascii());
}


void KTouch::showStat()
{
	touchStatWindow = new TouchStatWindow(touchStat);
	touchStatWindow->show();
}


void KTouch::hideStat()
{
	kdDebug() << "slotModeStat" << endl;
	touchStatWindow->hide();
	delete(touchStatWindow);
}


void KTouch::slotModeTrain()
{
	QWidget *oldCentralWidget=centralWidget();

	kdDebug() << "slotModeTrain" << endl;
	touchTrain = new TouchTrain(this);

	touchTrain->line->setLecture(touchLecture);
	touchTrain->line->getNextLine();

	setCentralWidget(touchTrain);
	touchTrain->show();
	touchTrain->setFocus();

	delete(oldCentralWidget);
}

void KTouch::slotModeEdit()
{
	QWidget *oldCentralWidget=centralWidget();

	kdDebug() << "slotModeEdit" << endl;
	touchEdit = new TouchEdit(this);
	setCentralWidget(touchEdit);
	touchEdit->show();
	touchEdit->setFocus();


	delete(oldCentralWidget);
}

void KTouch::slotModeTest()
{
	QWidget *oldCentralWidget=centralWidget();
	kdDebug() << "slotModeTest" << endl;
	touchTest = new TouchTest(this);
	setCentralWidget(touchTest);
	touchTest->show();
	touchTest->setFocus();

	delete(oldCentralWidget);

}


