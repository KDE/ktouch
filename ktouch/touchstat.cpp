/***************************************************************************
                          touchstat.cpp  -  description
                             -------------------
    begin                : Sat Aug 18 2001
    copyright            : (C) 2001 by haavard
    email                : haavard@localhost.localdomain
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "touchstat.h"
#include "touchstat.moc"
#include <kdebug.h>

TouchStat::TouchStat(){
	for(int i=0;i<256;i++)
	{
		arrayStat[i].ok=0;
		arrayStat[i].error=0;
	}
	wordCount  = 0;
	errorCount = 0;
	okCount    = 0;
	totalTime  = 1;

	start();
}

TouchStat::~TouchStat(){
	stop();
}

void TouchStat::gotError(QChar c)
{
	arrayStat[c].error++;
	errorCount++;
}

void TouchStat::gotOk(QChar c)
{
	arrayStat[c].ok++;
	okCount++;
	if(c==" " || c.digitValue()==13)
		wordCount++;
}

void TouchStat::start()
{
	time = new QTime();
	time->start();
}

void TouchStat::stop()
{
	totalTime+=time->elapsed();
	delete(time);
}


int TouchStat::getTotalTime()
{
	if(time!=NULL)
	{
		return totalTime+time->elapsed();
	}
	return totalTime;
}

int TouchStat::getRatio()
{
	if(okCount+errorCount>0)
	{
		float f=((float)okCount/(okCount+errorCount))*100;
		return (int)f;
	}
	else
		return 100;
}

int TouchStat::getCharPerMin()
{
	float f=okCount*60000/getTotalTime();
	return (int)f;
}

int TouchStat::getWordPerMin()
{
	float f=wordCount*60000/getTotalTime();
	return (int)f;
}

QString TouchStat::getProgText()
{
	QString s;
	float max=0;
	int bad=0;
	for(int i=0;i<256;i++)
	{
		if((arrayStat[i].error+arrayStat[i].ok)>0)
			if(((float)arrayStat[i].error/(arrayStat[i].ok))>max)
			{
				max=(float)arrayStat[i].error/arrayStat[i].ok;
				bad=i;
			}
	}
	s = "You should train on: ";
	s += QChar(bad);

	return s;
}
