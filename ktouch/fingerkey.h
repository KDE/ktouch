/***************************************************************************
                          fingerkey.h  -  description
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

#ifndef FINGERKEY_H
#define FINGERKEY_H


#include "touchkey.h"

class FingerKey: public TouchKey
{
public:
	FingerKey(const char *keyValue, int gridX, int gridY, int w=10, int h=10);
	~FingerKey();
	void paint(QPainter *p);
	void setActive(bool b);
	QColor getColor();
	static int numberOfKeys;

private:
	bool isActive;
	int keyColor;
};

#endif

