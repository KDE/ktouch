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
  wordCount = 0;
  totalTime = 0;
  start();
}

TouchStat::~TouchStat(){
  stop();
}

void TouchStat::gotError(QChar c)
{
  arrayStat[c].error++;
}

void TouchStat::gotOk(QChar c)
{
  arrayStat[c].ok++;
  if(c==" " || c==13)
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
    return (totalTime+time->elapsed());
  }
  return totalTime;
}

void TouchStat::printStat()
{
  int totalError = 0;
  int totalOk = 0;
  kdDebug() << "------------------------------------" << endl;
  for(int i=0;i<256;i++)
  {
    if((arrayStat[i].ok + arrayStat[i].error) != 0)
    {
       kdDebug() << i << " -> " << arrayStat[i].ok << "|" << arrayStat[i].error << endl;
       totalError+=arrayStat[i].error;
       totalOk+=arrayStat[i].ok;
    }
  }
  kdDebug() << endl;
  kdDebug() << "Number of words: " << wordCount << endl;
  kdDebug() << endl;
  kdDebug() << "Number of words pr. minute: " << (float)wordCount*60000/getTotalTime() << endl;
  kdDebug() << endl;
  kdDebug() << "Total time: " << getTotalTime()/1000 << "s" << endl;
  kdDebug() << endl;
  kdDebug() << "Ratio : " << (float)totalError/(totalOk+totalError) << endl;
  kdDebug() << endl;
}
