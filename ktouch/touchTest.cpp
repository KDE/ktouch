/***************************************************************************
                          touchTest.cpp  -  description
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

#include "touchTest.h"
#include "touchTest.moc"

#include <kdebug.h>

TouchTest::TouchTest( QWidget* parent, const char* name)
          : TouchTestLayout(parent,name)
{
}

TouchTest::~TouchTest()
{

}

