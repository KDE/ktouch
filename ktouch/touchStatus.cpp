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
#include "touchStatus.moc"
#include <qpushbutton.h>
#include <qlabel.h>
#include <qapplication.h>
#include <qprogressbar.h>

TouchStatus::TouchStatus(QWidget * parent, const char * name)
           : TouchStatusLayout( parent, name )
{
	errorSound=true;
	autoLevel=true;
	reset();

	speedLimitUp = 100;
	speedLimitDown = 50;

	timer = new QTimer(this,"timer");

	pauseButton->setFocusPolicy(QWidget::NoFocus);
	connect( timer, SIGNAL(timeout()), SLOT(calculate()));
	timer->start(500,false);
}

void TouchStatus::gotError(QChar)
{
	bufferError++;
	if(errorSound) QApplication::beep();
}

void TouchStatus::gotOk(QChar c)
{
	bufferOk++;
	if(c==" " || c.digitValue()==13)
		wordCount++;
}

void TouchStatus::reset()
{
	charSpeed      = 0;
	wordSpeed      = 0;
	correct        = 1;
	wrong          = 0;
	bufferOk       = 0;
	bufferError    = 0;
	testLevelCount = 0;
	wordCount      = 0;
}

void TouchStatus::calculate()
{
	charSpeed=(charSpeed*60+bufferOk*120)/61;
	wordSpeed=(wordSpeed*60+wordCount*120)/61;
	wrong=(wrong*60+bufferError)/61;
	correct=(correct*60+bufferOk)/61;
	bufferError=0;
	bufferOk=0;
	wordCount=0;

	LCDSpeed->display((int)charSpeed);

	if ((correct+wrong)>0)
	{
		LCDCorrect->setProgress((int)(correct/(correct+wrong)*100));
	}
	else
	{
		LCDCorrect->setProgress(100);
	}

	testLevelCount++;
	if (testLevelCount>50 && autoLevel)
	{
		testLevelCount=0;
		if (charSpeed>speedLimitUp)
		{
		emit levelUp();
		}
		else
		{
		if(charSpeed<speedLimitDown){
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
	charSpeed=s;
	LCDSpeed->display(charSpeed);
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
}

/** Starts to calculate speed and correctness */
void TouchStatus::stopKTouch()
{
	timer->stop();
	emit stop();
}

void TouchStatus::runningStateChanged(int i)
{
	if(i)
		stopKTouch();
	else
		startKTouch();
}

void TouchStatus::showStatChanged(int i)
{
	if(i)
		showStat();
	else
		hideStat();
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

