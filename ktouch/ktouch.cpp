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

// include files for KDE
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <kstdaction.h>
#include <qvbox.h>

// application specific includes
#include "ktouch.h"

#define ID_STATUS_MSG 1

KTouch::KTouch(QWidget* , const char* name):KMainWindow(0, name)
{
  config=kapp->config();
  dirs = KGlobal::dirs();

	touchLecture = new TouchLecture( this );

  QVBox * view = new QVBox ( this );

	touchStatus = new TouchStatus( view );

	touchLine = new TouchLine(view,"touchLine",touchLecture);
  view->setStretchFactor(touchLine,1);

	touchKeyboard = new TouchKeyboard(view);
  touchKeyboard->setMinimumHeight(100);
	touchKeyboard->setMinimumWidth(300);
  view->setStretchFactor(touchKeyboard,3);

  setCentralWidget( view );


	QObject::connect( touchLine,    SIGNAL(isError()),
	                  touchStatus,  SLOT(gotError()));
	QObject::connect( touchLine,    SIGNAL(isOk()),
	                  touchStatus,  SLOT(gotOk()));
	QObject::connect( touchStatus,  SIGNAL(levelUp()),
	                  touchLecture, SLOT(levelUp()));
	QObject::connect( touchStatus,  SIGNAL(levelDown()),
	                  touchLecture, SLOT(levelDown()));
  QObject::connect( touchStatus,  SIGNAL(stop()),
                    touchLine,    SLOT(stop()));
  QObject::connect( touchStatus,  SIGNAL(start()),
                    touchLine,    SLOT(start()));
  QObject::connect( touchStatus,  SIGNAL(forceNextLine()),
                    touchLine,    SLOT(getNextLine()));
  QObject::connect( touchLecture, SIGNAL(levelMessage(const QString&)),
                    touchStatus,  SLOT(setLevelMessage(const QString&)));
	QObject::connect( touchLecture, SIGNAL(levelChanged(int)),
	                  touchStatus,  SLOT(setLevel(int)));
	QObject::connect( touchLine,    SIGNAL(nextKey(const QChar&)), touchKeyboard, SLOT(newKey(const QChar&)));


	///////////////////////////////////////////////////////////////////
	// call inits to invoke all other construction parts
	initStatusBar();
	initActions();
  cout << dirs->localkdedir().latin1() << endl;
  QString trainingFile=dirs->findResource("data","ktouch/english.ktouch");
  touchLecture->loadLectureFile(trainingFile);

	readOptions();
  touchLine->getNextLine();
}

void KTouch::initActions()
{
  fileOpen = KStdAction::open(this, SLOT(slotFileOpen()), actionCollection());
  fileOpenRecent = KStdAction::openRecent(this, SLOT(slotFileOpenRecent(const KURL&)), actionCollection());
  fileQuit = KStdAction::quit(this, SLOT(slotFileQuit()), actionCollection());
  viewToolBar = KStdAction::showToolbar(this, SLOT(slotViewToolBar()), actionCollection());
  viewStatusBar = KStdAction::showStatusbar(this, SLOT(slotViewStatusBar()), actionCollection());

  new KAction(i18n("Keyboard"),0,this,SLOT(slotOptionKeyboard()), actionCollection(),"optionKeyboard");
  new KAction(i18n("Training"),0,this,SLOT(slotOptionTraining()), actionCollection(),"optionTraining");
  new KAction(i18n("General"),0,this,SLOT(slotOptionGeneral()), actionCollection(),"optionGeneral");

  fileOpen->setStatusText(i18n("Opens an existing document"));
  fileOpenRecent->setStatusText(i18n("Opens a recently used file"));

  fileQuit->setStatusText(i18n("Quits the application"));
  viewToolBar->setStatusText(i18n("Enables/disables the toolbar"));
  viewStatusBar->setStatusText(i18n("Enables/disables the statusbar"));

  // use the absolute path to your ktouchui.rc file for testing purpose in createGUI();
  //createGUI("/home/haavard/ktouch/ktouch/ktouchui.rc");
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
    touchLecture->loadLectureFile(url.directory(false) + url.fileName());
    touchLine->getNextLine();
    setCaption(url.fileName(), false);
    fileOpenRecent->addURL( url );
  }
  fileOpenRecent->addURL( url );
  slotStatusMsg(i18n("Ready."));
}

void KTouch::saveOptions()
{
	//** General Options ********************************************************	
  config->setGroup("General Options");
  config->writeEntry("Geometry", size());
  config->writeEntry("Show Toolbar", viewToolBar->isChecked());
  config->writeEntry("Show Statusbar",viewStatusBar->isChecked());
  config->writeEntry("ToolBarPos", (int) toolBar("mainToolBar")->barPos());
  fileOpenRecent->saveEntries(config,"Recent Files");

  //** History ****************************************************************
  config->setGroup("History");
  config->writeEntry("Speed",touchStatus->getSpeed());
  config->writeEntry("Level",touchStatus->getLevel());
  config->writeEntry("Remember",remember);
  config->writeEntry("SpeedLimitUp",touchStatus->getSpeedLimitUp());
  config->writeEntry("SpeedLimitDown",touchStatus->getSpeedLimitDown());
  config->writeEntry("Auto Level",touchStatus->autoLevel);

  //** Keyboard ***************************************************************
  config->setGroup("Keyboard");
  config->writeEntry("Show Color",touchKeyboard->getShowColor());
  config->writeEntry("Show Animation",touchKeyboard->getShowAnimation());

  config->writeEntry("Language",touchKeyboard->getLanguage());

  //** General ****************************************************************
  config->setGroup("General");
  config->writeEntry("Beep on error",touchStatus->errorSound);
  config->writeEntry("Color on error",touchLine->showError);
}

void KTouch::readOptions()
{
	//** General Options ********************************************************
  config->setGroup("General Options");

  // bar status settings
  bool bViewToolbar = config->readBoolEntry("Show Toolbar", true);
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

  QSize size=config->readSizeEntry("Geometry");
  if(!size.isEmpty())
  {
    resize(size);
  }

  //** History ****************************************************************
  config->setGroup("History");
  remember=config->readBoolEntry("Remember");
  if(remember)
  {
     touchLecture->setLevel(config->readNumEntry("Level"));
     touchStatus->setSpeed(config->readNumEntry("Speed"));
  }
  touchStatus->setSpeedLimit(config->readNumEntry("SpeedLimitUp"),config->readNumEntry("SpeedLimitDown"));

  touchStatus->autoLevel=config->readBoolEntry("Auto Level");
  touchStatus->pushButtonLevelDown->setDisabled(touchStatus->autoLevel);
  touchStatus->pushButtonLevelUp->setDisabled(touchStatus->autoLevel);


  //** Keyboard ***************************************************************
  config->setGroup("Keyboard");
  touchKeyboard->setShowColor(config->readBoolEntry("Show Color"));
  touchKeyboard->setShowAnimation(config->readBoolEntry("Show Animation"));
  QString lang=config->readEntry("Language");
  if(lang.isEmpty()) lang="en";
  touchKeyboard->loadKeyboard(config->readEntry("Language"));

  //** General ***************************************************************
  config->setGroup("General");
  touchStatus->errorSound=config->readBoolEntry("Beep on error");
  touchLine->showError=config->readBoolEntry("Color on error");
}

bool KTouch::queryExit()
{
  printf("queryExit");
  saveOptions();
  return true;
}

/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////

void KTouch::slotFileOpen()
{
  slotStatusMsg(i18n("Opening file..."));
	
  KURL url=KFileDialog::getOpenURL(QString::null,
      i18n("*|All files"), this, i18n("Open File..."));
  if(!url.isEmpty())
  {
    touchLecture->loadLectureFile(url.directory(false) + url.fileName());
    touchLine->getNextLine();
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
    touchLecture->loadLectureFile(url.directory(false) + url.fileName());
    touchLine->getNextLine();
    setCaption(url.fileName(), false);
  }
  slotStatusMsg(i18n("Ready."));

}

void KTouch::slotFileQuit()
{
  cout << "exit" << endl;
  slotStatusMsg(i18n("Exiting..."));
  exit(0);
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
  OptionKeyboard *optionKeyboard=new OptionKeyboard(this,"keyboardOptionDialog",true);
  optionKeyboard->grabKeyboard();
  optionKeyboard->showColor->setChecked(touchKeyboard->getShowColor());
  optionKeyboard->showAnimation->setChecked(touchKeyboard->getShowAnimation());
  for(int i=0;optionKeyboard->language->count()>i;i++)
  {
    if(optionKeyboard->language->text(i)==touchKeyboard->getLanguage())
    {
       optionKeyboard->language->setCurrentItem(i);
    }
  }

  int result = optionKeyboard->exec();
  if(result==1)
  {
    touchKeyboard->setShowColor(optionKeyboard->showColor->isChecked());
    touchKeyboard->setShowAnimation(optionKeyboard->showAnimation->isChecked());
    touchKeyboard->loadKeyboard(optionKeyboard->language->currentText());
  }
  delete optionKeyboard;
  touchLine->grabKeyboard ();
}

void KTouch::slotOptionTraining()
{
  OptionTraining *optionTraining=new OptionTraining(this,"trainingOptionDialog",true);
  optionTraining->limitUp->setValue(touchStatus->getSpeedLimitUp());
  optionTraining->limitDown->setValue(touchStatus->getSpeedLimitDown());
  optionTraining->remember->setChecked(remember);
  optionTraining->autoLevel->setChecked(touchStatus->autoLevel);
	
  int result = optionTraining->exec();
  if(result==1)
  {
    touchStatus->setSpeedLimit(optionTraining->limitUp->value(),optionTraining->limitDown->value());
    remember=optionTraining->remember->isChecked();

    touchStatus->autoLevel=optionTraining->autoLevel->isChecked();
    touchStatus->pushButtonLevelDown->setDisabled(touchStatus->autoLevel);
    touchStatus->pushButtonLevelUp->setDisabled(touchStatus->autoLevel);
  }
  delete optionTraining;
}

void KTouch::slotOptionGeneral()
{
  OptionGeneral *optionGeneral=new OptionGeneral(this,"generalOptionDialog",true);
  optionGeneral->beepOnError->setChecked(touchStatus->errorSound);
  optionGeneral->colorOnError->setChecked(touchLine->showError);
  int result = optionGeneral->exec();
  if(result==1)
  {
     touchStatus->errorSound=(optionGeneral->beepOnError->isChecked());
     touchLine->showError=(optionGeneral->colorOnError->isChecked());
  }
  delete optionGeneral;
}

void KTouch::keyPressEvent(QKeyEvent*e)
{
  touchLine->keyPressed(e->ascii());
}


