
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
#include <kapp.h>

#include <qpushbutton.h>

#include "touchTrainLayout.h"
#include "touchkeyboard.h"
#include "touchStatus.h"

class TouchTrain : public TouchTrainLayout {
   Q_OBJECT

public:
	/** constructor */
	TouchTrain(QWidget* parent=0, const char* name=0);

	/** destructor */
	~TouchTrain();

	void saveOptions();
	void readOptions();
	
private:
	KConfig *config;
	bool remember;

};

#endif


