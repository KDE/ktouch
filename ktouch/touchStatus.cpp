/***************************************************************************
                          touchStatus.cpp  -  description
                             -------------------
    begin                : Sun Dec 10 2000
    copyright            : (C) 2000 by Haavard Froeiland
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

#include "touchStatus.h"
#include <iostream.h>
#include <qlabel.h>
#include <qapplication.h>
#include <qprogressbar.h>
#include <kaudioplayer.h>

TouchStatus::TouchStatus(QWidget * parent, const char * name)
           : TouchStatusLayout( parent, name )
{
  errorSound=true;
  autoLevel=true;
  reset();

  speedLimitUp = 100;
	speedLimitDown = 50;

  timer = new QTimer(this,"timer");

  connect( timer, SIGNAL(timeout()), SLOT(calculate()));
  timer->start(500,false);
}

void TouchStatus::gotError()
{
  bufferError++;
  if(errorSound) QApplication::beep();
}

void TouchStatus::gotOk()
{
  bufferOk++;
}

void TouchStatus::reset()
{
  speed          = 0;
  correct        = 1;
  wrong          = 0;
  bufferOk       = 0;
  bufferError    = 0;
  testLevelCount = 0;
}

void TouchStatus::calculate()
{
  speed=(speed*30+bufferOk*120)/31;
  wrong=(wrong*30+bufferError)/31;
  correct=(correct*30+bufferOk)/31;
  bufferError=0;
  bufferOk=0;

  LCDSpeed->display((int)speed);
  if ((correct+wrong)>0)
  {
    LCDCorrect->setProgress((int)(correct/(correct+wrong)*100));
  }
  else
  {
    LCDCorrect->setProgress(100);
  }

  testLevelCount++;
  if (testLevelCount>20 && autoLevel)
  {
    testLevelCount=0;
    if (speed>speedLimitUp)
    {
      emit levelUp();
    }
    else
    {
      if(speed<speedLimitDown){
          emit levelDown();
			}
    }
  }
}

void TouchStatus::setLevel(int level)
{
  LCDLevel->display(level+1);
}

void TouchStatus::levelForcedUp()
{
  if(!autoLevel)
  {
    emit levelUp();
    emit forceNextLine();
  }
}
void TouchStatus::levelForcedDown()
{
  if(!autoLevel)
  {
    emit levelDown();
    emit forceNextLine();
  }
}

unsigned int TouchStatus::getLevel()
{
 	return (LCDLevel->intValue()-1);
}

void TouchStatus::setSpeed(int s)
{
  speed=s;
	LCDSpeed->display(speed);
}

unsigned int TouchStatus::getSpeed()
{
 	return LCDSpeed->intValue();
}

/** Stops calculating speed and correctness. */
void TouchStatus::startKTouch()
{
	timer->start(500);
  emit start();
//  cout << "start" << endl;
}

/** Starts to calculate speed and correctness */
void TouchStatus::stopKTouch()
{
	timer->stop();
  emit stop();
//  cout << "stop" << endl;
}

void TouchStatus::runningStateChanged(int i)
{
  if(i)
    stopKTouch();
  else
    startKTouch();
}

/** Sets the speed limit for going to next or previous level.	Up should always bee higher then down.  */
void TouchStatus::setSpeedLimit(int up, int down){
	if(down>up)
	{
		down=up;
	}
  else if(up==0 || down==0)
  {
    down=50;
    up=100;
  }
	speedLimitDown=down;
	speedLimitUp=up;

}

/** Returns the limit to go to next level */
int TouchStatus::getSpeedLimitUp(){
	return speedLimitUp;
}

/** Returns the speed limit to go down one level */
int TouchStatus::getSpeedLimitDown(){
	return speedLimitDown;
}

void TouchStatus::setLevelMessage(const QString& s)
{
  message->setText(s);
}
