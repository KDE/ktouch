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

#include <iostream.h>

#include <qobject.h>
#include <qdatetime.h>

#include <kstddirs.h>

class TouchStat : public QObject
{
	Q_OBJECT

	public:
		TouchStat();
		~TouchStat();

		int getTotalTime();
		int wordCount;
		int errorCount;
		int okCount;
		int getRatio();
		int getWordPerMin();
		int getCharPerMin();

		struct charStat{
			int ok;
			int error;
		};

		struct charStat arrayStat[512];


	public slots:
		void gotError(QChar);
		void gotOk(QChar);
		void start();
		void stop();
		void saveStat();

	private:
		KStandardDirs *dirs;
		QTime *time;
		int totalTime;

};

#endif
