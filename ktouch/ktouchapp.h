/***************************************************************************
                          ktouch.h  -  description
                             -------------------
    begin                : Thu Aug 30 15:33:49 EST 2001
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


#ifndef KTOUCHAPP_H
#define KTOUCHAPP_H

#include <kuniqueapp.h>

class KTouch;

class KTouchApp : public KUniqueApplication
{
	Q_OBJECT

public:
	KTouchApp();
	~KTouchApp();

	int newInstance();
	KTouch* getInstance();

private:
	KTouch *top;

};

#endif
