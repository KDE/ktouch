/***************************************************************************
                          touchkey.cpp  -  description
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

#include "touchkey.h"
#include <iostream.h>

bool TouchKey::showColor=true;
int TouchKey::showAnimation = 0;

TouchKey::TouchKey(const char *k, int x, int y, int w, int h)
{
  keyValue=k;
  gridX=x;
  gridY=y;
  gridWidth=w;
  gridHeight=h;
  state=false;
  x=y=0;
  width=height=0;
}

TouchKey::~TouchKey()
{};

void TouchKey::setShowColor(bool show)
{
  showColor=show;
}

bool TouchKey::getShowColor()
{
  return showColor;
}

void TouchKey::setShowAnimation(int show)
{
  showAnimation=show;
}

int TouchKey::getShowAnimation()
{
  return showAnimation;
}


void TouchKey::setScale(float s)
{
    x=(int)((gridX+1)*s);
    y=(int)((gridY+1)*s);
    width=(int)((gridWidth-2)*s);
    height=(int)((gridHeight-2)*s);
}

void TouchKey::paintBase(QPainter *p)
{
    if (state)
    {
	switch(getShowAnimation()){
	    case 0:
		break;
		
	    case 1:
		p->fillRect(x+width/6,y+height/6,2*width/3,2*height/3, QColor(255,0,0));
		break;
		
	    case 2:
		p->fillRect(x,y,width,height, QBrush( QColor(150,150,150),QBrush::Dense2Pattern) );
		break;
		
	    case 3:
		p->setPen( QColor(255,255,255) );
		p->drawLine(x,y,x+width,y+height);
		p->drawLine(x,y+height,x+width,y);
		break;
	}
    }
    p->drawRect(x,y,width,height);
    
    //Draw text on keys
    p->setFont(QFont("courier", getSmallest()/2));
    p->drawText(x,y,width,height,QPainter::AlignCenter,keyValue);
}

QColor TouchKey::getColor()
{
   return QColor();
}

int TouchKey::getSmallest()
{
  if(width>height)
    return height;
  return width;
}

