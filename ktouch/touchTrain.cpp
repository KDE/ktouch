/***************************************************************************
                          touchTrain.cpp  -  description
                             -------------------
    begin                : Tue Nov 27 2001
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

#include "touchTrain.h"
#include "touchTrain.moc"

#include "touchline.h"
#include <kdebug.h>

TouchTrain::TouchTrain( QWidget* parent, TouchLecture* l)
          : TouchTrainLayout(parent)
{
	lecture=l;

	//FIXME this should be rewritten
	line->setLecture(lecture);

	connect(status,  SIGNAL(levelUp()),         lecture,    SLOT(levelUp()));
	connect(status,  SIGNAL(levelDown()),       lecture,    SLOT(levelDown()));
	connect(lecture, SIGNAL(levelChanged(int)), status,     SLOT(setLevel(int)));
	connect(status,  SIGNAL(forceNextLine()),   line,       SLOT(getNextLine()));
	connect(lecture, SIGNAL(levelMessage(const QString&)), status, SLOT(setLevelMessage(const QString&)));

//	connect(status,  SIGNAL(showStat()),        this,       SLOT(showStat()));
//	connect(status,  SIGNAL(hideStat()),        this,       SLOT(hideStat()));

//	connect(line,    SIGNAL(isError(QChar)),    touchStat,  SLOT(gotError(QChar)));
//	connect(line,    SIGNAL(isOk(QChar)),       touchStat,  SLOT(gotOk(QChar)));


	config=kapp->config();
	readOptions();

	status->pushButtonLevelDown->setDisabled(status->autoLevel);
	status->pushButtonLevelUp->setDisabled(status->autoLevel);
}

TouchTrain::~TouchTrain()
{
	saveOptions();
}

void TouchTrain::saveOptions()
{
	keyboard->saveOptions();

	config->setGroup("General");
	config->writeEntry("Beep on error",status->errorSound);
	config->writeEntry("Sound on level",status->levelSound);
	config->writeEntry("Color on error",line->getShowError());
	config->writeEntry("ErrorColor",(QColor)line->getErrorColor());
	config->writeEntry("Font", (QFont)line->getFont());

	//** History ****************************************************************
	config->setGroup("History");
	config->writeEntry("Speed",status->getSpeed());
	config->writeEntry("Level",status->getLevel());
	config->writeEntry("Remember",remember);
	config->writeEntry("SpeedLimitUp",status->getSpeedLimitUp());
	config->writeEntry("SpeedLimitDown",status->getSpeedLimitDown());
	config->writeEntry("Auto Level",status->autoLevel);
}

void TouchTrain::readOptions()
{
	keyboard->readOptions();

	//** General ***************************************************************
	config->setGroup("General");

	status->errorSound=config->readBoolEntry("Beep on error",true);
	status->levelSound=config->readBoolEntry("Sound on level",true);
	line->setShowError(config->readBoolEntry("Color on error",true));

	QColor color(255,150,150);
	line->setErrorColor(config->readColorEntry("ErrorColor",&color));

	QFont font("adobe-courier");
	line->setFont(config->readFontEntry("Font",&font));


	//** History ****************************************************************
	config->setGroup("History");

	remember=config->readBoolEntry("Remember",true);
	if(remember)
	{
		lecture->setLevel(config->readNumEntry("Level",1));
		status->setSpeed(config->readNumEntry("Speed",0));
	}
	status->setSpeedLimit(config->readNumEntry("SpeedLimitUp"),config->readNumEntry("SpeedLimitDown"));

	// move this into touchTrain->status   setAutoLevel(bool b);
	status->autoLevel=config->readBoolEntry("Auto Level",true);

}


