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
    
    
    ///////////////////////////////////////////////////////////////////
    // call inits to invoke all other construction parts
    initStatusBar();
    initActions();
    setAutoSaveSettings();

    kdDebug() << "read option" << endl;
    readOptions();
    
    slotModeTrain();
}

KTouch::~KTouch()
{
    delete touchLecture;
    delete touchStat;
}

void KTouch::initActions()
{
    fileOpen = KStdAction::open(this, SLOT(slotFileOpen()), actionCollection());
//	fileSave = KStdAction::save(this, SLOT(slotFileSave()), actionCollection());
//	fileSaveAs = KStdAction::saveAs(this, SLOT(slotFileSaveAs()), actionCollection());
    fileOpenRecent = KStdAction::openRecent(this, SLOT(slotFileOpenRecent(const KURL&)), actionCollection());
    fileQuit = KStdAction::quit(this, SLOT(close()), actionCollection());
    createStandardStatusBarAction();
    setStandardToolBarMenuEnabled(true);

    new KAction(i18n("Keyboard..."),0,this,SLOT(slotOptionKeyboard()), actionCollection(),"optionKeyboard");
    new KAction(i18n("Training..."),0,this,SLOT(slotOptionTraining()), actionCollection(),"optionTraining");
    new KAction(i18n("General..."),0,this,SLOT(slotOptionGeneral()), actionCollection(),"optionGeneral"); 
    
/*
  new KAction(i18n("Train"),1,this,SLOT(slotModeTrain()), actionCollection(),"train");
  new KAction(i18n("Edit"),2,this,SLOT(slotModeEdit()), actionCollection(),"edit");
  new KAction(i18n("Test"),3,this,SLOT(slotModeTest()), actionCollection(),"test");
*/
    
    statShow = new KToggleAction(i18n("Stat"),0,this,SLOT(slotViewStat()), actionCollection(),"stat");
    
    fileOpen->setStatusText(i18n("Opens an existing document"));
    fileOpenRecent->setStatusText(i18n("Opens a recently used file"));
    
    fileQuit->setStatusText(i18n("Quits the application"));
    
    // use the absolute path to your ktouchui.rc file for testing purpose in createGUI();
    // createGUI("/home/haavard/cvs/kde/kdeedu/ktouch/ktouch/ktouchui.rc");
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
    fileOpenRecent->saveEntries(config,"Recent Files");
}

void KTouch::readOptions()
{
    //** General Options ***************************************************
    // initialize the recent file list
    fileOpenRecent->loadEntries(config,"Recent Files");
    
    // ** Recent files *******************************************************
    config->setGroup("Recent Files");
    KURL url=config->readEntry("File1",dirs->findResource("data","ktouch/english.ktouch"));
    
    touchLecture->load(url.directory(false,true)+url.fileName());
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
	i18n("*.ktouch|KTouch training files"), this, i18n("Open File"));
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
    slotStatusMsg(i18n("Saving..."));
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
	i18n("*.ktouch|KTouch training files"), this, i18n("Save File"));
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
    slotStatusMsg(i18n("Editing..."));
    
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
    optionKeyboard->showAnimation->setCurrentItem(touchTrain->keyboard->getShowAnimation());
    
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
	touchTrain->keyboard->setShowAnimation(optionKeyboard->showAnimation->currentItem());
	if(optionKeyboard->showKeypad->isChecked())
	{
	    touchTrain->keyboard->loadKeyboard("number");
	    KURL url(dirs->findResource("data","ktouch/number.ktouch"));
	    openDocumentFile(url);
	    slotFileOpenRecent(url);
	}
	else
	{
	    touchTrain->keyboard->loadKeyboard(optionKeyboard->language->currentText());
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
    optionTraining->remember->setChecked(touchTrain->remember);
    optionTraining->autoLevel->setChecked(touchTrain->status->autoLevel);
    
    if(optionTraining->exec())
    {
	// Save dialog changes
	touchTrain->status->setSpeedLimit(optionTraining->limitUp->value(),optionTraining->limitDown->value());
	touchTrain->remember=optionTraining->remember->isChecked();
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


void KTouch::slotViewStat()
{
    if(statShow->isChecked())
    {
	touchStatWindow = new TouchStatWindow(touchStat,this);
	touchStatWindow->show();
	connect(touchStatWindow, SIGNAL(statShowSetChecked(bool)), statShow, SLOT(setChecked(bool)));
    }
    else
    {
	touchStatWindow->hide();
	delete(touchStatWindow);
    }
}


void KTouch::slotModeTrain()
{
    QWidget *oldCentralWidget=centralWidget();
    
    kdDebug() << "slotModeTrain" << endl;
    touchTrain = new TouchTrain(this,touchLecture, touchStat);
    touchTrain->line->getNextLine();
    
    setCentralWidget(touchTrain);
    touchTrain->show();
    //touchTrain->setFocus();
    
    delete(oldCentralWidget);
}

void KTouch::slotModeEdit()
{
    QWidget *oldCentralWidget=centralWidget();
    
    kdDebug() << "slotModeEdit" << endl;
    touchEdit = new TouchEdit(this, touchLecture);

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


