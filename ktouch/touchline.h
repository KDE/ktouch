/***************************************************************************
                          touchline.h  -  description
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

#ifndef TOUCHLINE_H
#define TOUCHLINE_H

#include <qcolor.h>

#include "touchlecture.h"
#include "touchLineLayout.h"
#include "slideLine.h"

class TouchLine: public TouchLineLayout
{
  Q_OBJECT
public:
  TouchLine(QWidget *parent=0, const char *name=0,TouchLecture *myLecture=0);
	~TouchLine();
	bool getShowError();
	void setShowError(bool s);
	
	QColor getErrorColor();
	void setErrorColor(QColor);

public slots:
  void setTeacherLine(const QString&);
  void getNextLine();
  void start();
  void stop();
  void keyPressed(QChar);

signals:
  void isError();
  void isOk();
  void nextKey(const QChar&);
  void stopKTouch();
  void startKTouch();
  void endOfLine();
  void teacherLineChanged(const QString&);

private:
  unsigned int pos;
  QString teacherLine;
  QString studentLine;
  TouchLecture *lecture;
  bool stopped;
  bool showError;


};

#endif




