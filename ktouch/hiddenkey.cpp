/***************************************************************************
                          hiddenkey.cpp  -  description
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

#include "normalkey.h"
#include "fingerkey.h"
#include "hiddenkey.h"
#include "controlkey.h"

HiddenKey::HiddenKey(TouchKey *t, TouchKey *f, TouchKey *c)
          :TouchKey("", 0, 0)
{
	targetKey=t;
	fingerKey=f;
	controlKey=c;
}

HiddenKey::~HiddenKey()
{
}

void HiddenKey::paint(QPainter *p)
{
    if(state)
    {
	if(fingerKey)
	{
	    ((FingerKey*)fingerKey)->setActive(true);
	    fingerKey->paint(p);
	}
	if(controlKey)
	{
	    ((ControlKey*)controlKey)->state=true;
	    controlKey->paint(p);
	}
//	targetKey->state=true;
	targetKey->paint(p);
    }
    else
    {
  	if(fingerKey)
	{
	    //  ((FingerKey*)fingerKey)->setActive(false);
	    fingerKey->paint(p);
	}
	if(controlKey)
	{
	    ((ControlKey*)controlKey)->state=false;
	    controlKey->paint(p);
	}
//	targetKey->state=false;
	targetKey->paint(p);
    }
}

