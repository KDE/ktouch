/***************************************************************************
                          touchkeyboard.h  -  description
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

#ifndef TOUCHKEYBOARD_H
#define TOUCHKEYBOARD_H

#include <qwidget.h>
#include <qpainter.h>
#include <vector>
#include <qpixmap.h>
#include <kstddirs.h>

#include "fingerkey.h"
#include "normalkey.h"
#include "controlkey.h"
#include "hiddenkey.h"


class TouchKeyboard: public QWidget
{
  Q_OBJECT
public:
	TouchKeyboard(QWidget *parent=0, const char *name=0);
	void loadKeyboard(QString language);
	QString getLanguage();
  void setShowColor(bool show);
  void setShowAnimation(bool show);
  bool getShowColor();
  bool getShowAnimation();

public slots:
  void newKey(const QChar&);
			
protected:
	void paintEvent( QPaintEvent * );
	virtual void resizeEvent ( QResizeEvent * );

private:
	QChar lastKey;

	TouchKey *keyArray[512];
	void clearKeyboard();
	void calculateSize();
	QString language;
	bool showAnimation;
  int trans;
  KStandardDirs *dirs;
  void setIfMax(int w, int h);
  int maxWidth;
  int maxHight;
};


#endif

