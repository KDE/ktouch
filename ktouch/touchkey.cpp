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

void TouchKey::setScale(float s)
{
  x=(int)((gridX+1)*s);
  y=(int)((gridY+1)*s);
	width=(int)((gridWidth-2)*s);
  height=(int)((gridHeight-2)*s);
}

void TouchKey::paintBase(QPainter *p)
{
  if (state==true)
  {
    p->fillRect(x,y,width,height, QBrush( QColor(50,50,50),QBrush::Dense4Pattern) );
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

