/***************************************************************************
                          slideLine.h  -  description
                             -------------------
    begin                : Thu Mar 15 2001
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

#ifndef SLIDELINE_H
#define SLIDELINE_H

#include <qwidget.h>
#include <qpainter.h>
#include <qstring.h>
#include <qpixmap.h>
#include <qtimer.h>
#include <qfontmetrics.h>

class SlideLine: public QWidget
{
  Q_OBJECT

public:
 	SlideLine(QWidget *parent=0, const char *name=0);

  void setTeacherText(QString text);
  void setStudentText(QString text);
  float offset;
  bool error;
  QColor errorColor;

  void setFont(QFont f);
  QFont getFont();

protected:
 	void paintEvent  ( QPaintEvent * );
	void resizeEvent ( QResizeEvent * );


private:
  bool needRecreate;
  QString teacherText;
  QString studentText;
  QPixmap *pixmap;
  int pixmapSize;
  QTimer *timer;
  int pos;
  bool textChanged;
  int cursorCount;
  int cursorX;
  int cursorY;
  int cursorHight;
  void calculateWidth();
  void calculateCursor();
  QFont font;

private slots:
  void slide();
};

#endif
