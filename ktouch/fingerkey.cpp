/***************************************************************************
                          fingerkey.cpp  -  description
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

#include "fingerkey.h"
#include <iostream.h>

int FingerKey::numberOfKeys=0;

FingerKey::FingerKey(const char *keyValue, int gridX, int gridY, int w, int h)
					:TouchKey(keyValue, gridX, gridY, w, h)
{
	isActive=false;
	++numberOfKeys;
	keyColor=numberOfKeys;
}

FingerKey::~FingerKey()
{
}

void FingerKey::setActive(bool b)
{
 	isActive=b;
}

QColor FingerKey::getColor()
{
  if(TouchKey::getShowColor())
    switch(keyColor)
    {
    case 1:
      return QColor(255,238,7);break;
    case 2:
      return QColor(14,164,239);break;
    case 3:
      return QColor(158,255,155);break;
    case 4:
      return QColor(252,138,138);break;
    case 5:
      return QColor(247,138,247);break;
    case 6:
      return QColor(158,255,155);break;
    case 7:
      return QColor(14,164,239);break;
    case 8:
      return QColor(255,238,7);break;
    default:
      return QColor(155,155,155);
    }
  else return QColor(255,255,255);
}

void FingerKey::paint(QPainter *p)
{
	p->fillRect(x+1,y+1,width-2,height-2,getColor());
	paintBase(p);
	int s=getSmallest();
	int x_tmp=x;
	int y_tmp=y;
	if(s<width)
	  x_tmp=x_tmp+(width-s)/2;
	else if(s<height)
	  y_tmp=y_tmp+(height-s)/2;
	
	if(isActive==true)
	  p->drawEllipse(x_tmp,y_tmp,s,s);
	{
		p->drawEllipse(x_tmp+1,y_tmp+1,s-2,s-2);
	}
}

