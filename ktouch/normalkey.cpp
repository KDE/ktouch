/***************************************************************************
                          normalkey.cpp  -  description
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

#include"normalkey.h"
#include"fingerkey.h"


NormalKey::NormalKey(const char *keyValue, int gridX, int gridY, TouchKey *f, int w )
					:TouchKey(keyValue, gridX, gridY, w)
{
  fingerKey=f;
}

NormalKey::~NormalKey()
{
}

void NormalKey::paint(QPainter *p)
{
  p->eraseRect(x,y,width,height);
  p->fillRect(x,y,width,height,QBrush(QColor(fingerKey->getColor())));
	paintBase(p);
	if(state==true)
	{
		if(fingerKey)
		{
			((FingerKey*)fingerKey)->setActive(true);
			fingerKey->paint(p);
		}
	
	}
	else
	{
		if(fingerKey)
		{
			((FingerKey*)fingerKey)->setActive(false);
     	fingerKey->paint(p);
		}

	}

}

