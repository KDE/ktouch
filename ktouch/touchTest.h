
/***************************************************************************
                          touchTest.h  -  description
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

#ifndef TOUCHTEST_H
#define TOUCHTEST_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "touchTestLayout.h"

class TouchTest : public TouchTestLayout {
   Q_OBJECT

public:
	/** constructor */
	TouchTest(QWidget* parent=0, const char* name=0);

	/** destructor */
	~TouchTest();



};

#endif

