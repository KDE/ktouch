/***************************************************************************
                          touchStatus.h  -  description
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

#ifndef TOUCHSTATUS_H
#define TOUCHSTATUS_H

#include <qtimer.h>
#include <qlcdnumber.h>
#include <qprogressbar.h>
#include <kstddirs.h>

#include "touchStatusLayout.h"

class TouchStatus: public TouchStatusLayout
{
	Q_OBJECT

	public:
		TouchStatus( QWidget *parent, const char *name=0);

		/** Sets the speed limit for going to next or previous level.
			Up should always bee higher then down.  */
		void setSpeedLimit(int up, int down);
		/** Returns the speed limit to go down one level */
		int getSpeedLimitDown();
		/** Returns the limit to go to next level */
		int getSpeedLimitUp();
		bool errorSound;
		bool levelSound;
		bool autoLevel;

	public slots:
		void gotError(QChar);
		void gotOk(QChar);
		void reset();
		void setLevel(int);
		void levelForcedUp();
		void levelForcedDown();
		unsigned int getLevel();
		void setSpeed(int);
		unsigned int getSpeed();
		/** Starts to calculate speed and correctness */
		void startKTouch();
		/** Stops calculating speed and correctness. */
		void stopKTouch();
		void runningStateChanged(int);
		void showStatChanged(int);
		void setLevelMessage(const QString&);

	private:
		QTimer *timer;
		double charSpeed;
		double wordSpeed;
		int speedLimitUp;
		int speedLimitDown;
		double correct;
		double wrong;
		double bufferError;
		double bufferOk;
		int wordCount;

		int testLevelCount;
		KStandardDirs *dirs;


	private slots:
		void calculate();

	signals:
		void levelUp();
		void levelDown();
		void stop();
		void start();
		void forceNextLine();
		void showStat();
		void hideStat();
};

#endif
