
/***************************************************************************
                          touchTrain.h  -  description
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

#ifndef TOUCHTRAIN_H
#define TOUCHTRAIN_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kconfig.h>
#include <kapplication.h>

#include <qpushbutton.h>

#include "touchTrainLayout.h"
#include "touchkeyboard.h"
#include "touchStatus.h"
#include "touchstat.h"
#include "touchlecture.h"

class TouchTrain : public TouchTrainLayout {
   Q_OBJECT

public:
	/** constructor */
	TouchTrain(QWidget* parent, TouchLecture* lecture, TouchStat* status);

	/** destructor */
	~TouchTrain();

	void saveOptions();
	void readOptions();

	bool remember;

private:
	KConfig *config;
	TouchLecture* lecture;
};

#endif


