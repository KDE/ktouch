/***************************************************************************
                          hiddenkey.h  -  description
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


#ifndef HIDDENKEY_H
#define HIDDENKEY_H

#include "touchkey.h"


class HiddenKey: public TouchKey
{
  public:
  	HiddenKey(TouchKey *targetKey, TouchKey *fingerKey, TouchKey *controlKey=0);
  	~HiddenKey();
	  void paint(QPainter *p);
  private:
		TouchKey *fingerKey;
    TouchKey *controlKey;
		TouchKey *targetKey;

};

#endif


