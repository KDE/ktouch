/***************************************************************************
                          touchkey.h  -  description
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

#ifndef TOUCHKEY_H
#define TOUCHKEY_H

#include <qpainter.h>

class TouchKey
{
 public:
    TouchKey(const char *keyValue, int gridX, int gridY, int gridWidth=10,int gridHight=10);
    TouchKey();
    virtual ~TouchKey();
    virtual void paint(QPainter *p)=0;
    void paintBase(QPainter *p);
    void setScale(float scale);
    bool state;
    int x;
    int y;
    int width;
    int height;
    const char *keyValue;
    virtual QColor getColor();
    int getSmallest();
    
    static void setShowColor(bool show);
    static bool getShowColor();
    
    static void setShowAnimation(int show);
    static int getShowAnimation();
    
 private:
    int gridX;
    int gridY;
    int gridWidth;
    int gridHeight;
    static bool showColor;
    static int showAnimation;
};

#endif
