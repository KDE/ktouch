/***************************************************************************
                          normalkey.h  -  description
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

#ifndef NORMALKEY_H
#define NORMALKEY_H

#include "touchkey.h"


class NormalKey: public TouchKey
{
  public:
  	NormalKey(const char *keyValue, int gridX, int gridY, TouchKey *fingerKey, int width=10);
  	~NormalKey();
	  void paint(QPainter *p);
  private:
		TouchKey *fingerKey;
static bool showColor;
};

#endif


