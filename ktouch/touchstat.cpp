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

#include <qfile.h>
#include <qtextstream.h>
#include <qmessagebox.h>
#include <qdatetime.h>

#include <kdebug.h>

#include "touchstat.h"
#include "touchstat.moc"

TouchStat::TouchStat()
{
    dirs = KGlobal::dirs();
    for(int i=0;i<512;i++)
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
    delete time;
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
    time=NULL;
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


// add todays stat's to file
void TouchStat::saveStat()
{

    QFile f(dirs->saveLocation("appdata")+"stat.log");
    if ( f.open(IO_Append | IO_WriteOnly ) )
    {
	QTextStream t( &f );
	
	t << "Date:            " << QDateTime::currentDateTime().toString() << "\n";
	t << "Time spent:      " << (getTotalTime()/1000) << "\n";
	t << "Word per minute: " << getWordPerMin()       << "\n";
	t << "Char per minute: " << getCharPerMin()       << "\n";
	t << "Correctness:     " << getRatio()            << "\n";
	
	t << "Char stats:\n";
	for(int i=0;i<512;i++)
	{
	    if(arrayStat[i].ok!=0 || arrayStat[i].error!=0)
		t << i << " " << arrayStat[i].ok << " " << arrayStat[i].error << "\n";
	}
	t << "\n";
	f.close();
    }
}

