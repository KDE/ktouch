/***************************************************************************
                          controlkey.cpp  -  description
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


#include"controlkey.h"

ControlKey::ControlKey(const char *keyValue, int gridX, int gridY, int w, int h)
					:TouchKey(keyValue, gridX, gridY, w, h)
{}

ControlKey::~ControlKey()
{}

void ControlKey::paint(QPainter *p)
{
  if (state)
  {
    p->fillRect(x+width/4,y+2,width-width/2,height-height*2/3,QBrush( QColor(50,50,50)));
  }
	else
	{
   	p->fillRect(x,y,width,height,QColor(200,200,200));
	}
 	p->drawRect(x,y,width,height);
 	p->drawRect(x+1,y+1,width-2,height-2);

  //draw the text on the key
  p->setFont(QFont("courier", getSmallest()/5));
  p->drawText(x+4,y+height-6,keyValue);
}

