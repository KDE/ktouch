/***************************************************************************
                          touchline.cpp  -  description
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

#include <qapplication.h>
#include "touchline.h"


TouchLine::TouchLine(QWidget * parent, const char * name, TouchLecture *l)
         : TouchLineLayout( parent, name )
{
	teacherLine = "";
	studentLine = "";
	lecture = l;
	pos=0;
  stopped=false;
}

TouchLine::~TouchLine()
{

}

void TouchLine::getNextLine()
{
  setTeacherLine(lecture->getNextLine());
}

void TouchLine::keyPressed(QChar e)
{
  if(!stopped)
  {
    //The character is a normal character
    //if ((e.unicode()>31 && e.unicode()<126) || (e.unicode()>127 && e.unicode()<256))
    if(e.isPrint())
    {
      studentLine+=(char)e.unicode();
      pos++;

     	if (teacherLine.left(pos)==studentLine)
  	  {
        emit isOk(e);
  			if (pos==teacherLine.length())
      	{
        	emit nextKey((char)13);
  			}
  			else
  			{
  				emit nextKey(teacherLine.at(pos));
  			}
      }
  	  else
      {
        emit isError(e);
        if(showError)
          line->error=true;
        // Backspace
        emit nextKey((char)8);
      }
    }

    // The character is a back space
  	else if(e.unicode()==8)
    {
      if (pos>0)
      {
        pos--;
        studentLine=studentLine.left(pos);
        if (teacherLine.left(pos)==studentLine)
  	    {
          emit isOk(e);
          line->error=false;
          emit nextKey(teacherLine[pos]);
        }
  			else
  			{
  				emit nextKey((char)8);
  			}
      }
    }
  	else if(e.unicode()==13)
    {
      if ((pos==teacherLine.length()) && (teacherLine.left(pos)==studentLine))
      {
        emit endOfLine();
        setTeacherLine(lecture->getNextLine());
        emit isOk(e);
        return;
      }
  	}
    line->setStudentText(studentLine);
    line->offset=pos/(float)teacherLine.length();
  }
  else QApplication::beep();
}

void TouchLine::setTeacherLine(const QString& t)
{
	teacherLine=t;
  line->offset=0;
  line->setTeacherText(teacherLine);

	studentLine = "";
	pos=0;

	emit nextKey(teacherLine[pos]);
}

void TouchLine::stop()
{
  stopped=true;
}

void TouchLine::start()
{
  stopped=false;
}

bool TouchLine::getShowError()
{
  return showError;
}

void TouchLine::setShowError(bool s)
{
  showError=s;
}

QColor TouchLine::getErrorColor()
{
  return line->errorColor;
}

void TouchLine::setErrorColor(QColor c)
{
  line->errorColor=c;
}

QFont TouchLine::getFont()
{
  return line->getFont();
}

void TouchLine::setFont(QFont f)
{
  line->setFont(f);
}


