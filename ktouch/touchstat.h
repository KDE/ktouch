/***************************************************************************
                          touchstat.h  -  description
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

#ifndef TOUCHSTAT_H
#define TOUCHSTAT_H


/**
  *@author haavard
  */

#include <qobject.h>
#include <qdatetime.h>

class TouchStat : public QObject
{
  Q_OBJECT
public: 
	TouchStat();
	~TouchStat();
  void printStat();
  int getTotalTime();

public slots:
  void gotError(QChar);
  void gotOk(QChar);
  void start();
  void stop();

private:
  struct charStat{
     int ok;
     int error;
  };
  struct charStat arrayStat[256];
  int wordCount;
  int totalTime;
  QTime *time;
};

#endif
