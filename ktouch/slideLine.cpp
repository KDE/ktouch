/***************************************************************************
                          slideLine.cpp  -  description
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

#include "slideLine.h"

SlideLine::SlideLine(QWidget *parent, const char *name)
          :QWidget( parent, name )
{
  offset = 0;
  pixmap = 0;
  pos    = 0;
  cursorX= 0;
  cursorY= 0;
  cursorHight=0;
  cursorCount=0;
  font = QFont("adobe-courier", width()/4);
  needRecreate=true;
  showError=false;

  timer = new QTimer(this,"timer");
  connect( timer, SIGNAL(timeout()),SLOT(slide()));
  timer->start(100,false);
}

void SlideLine::slide()
{
  if(width()<pixmapSize)
  {
    pos+=(int)((width()-pixmapSize)*(offset) - pos)/3;
  }
  if (cursorCount>6) cursorCount=0;
  else cursorCount++;
  repaint(false);
}

void SlideLine::setTeacherText(QString t)
{
  teacherText=t;
  needRecreate=true;
  setStudentText("");
  repaint(true);
}

void SlideLine::setStudentText(QString t)
{
  studentText=t;
  textChanged=true;
  repaint(false);
}


void SlideLine::paintEvent( QPaintEvent * )
{
  if (needRecreate)
  {
    calculateWidth();
    calculateCursor();
    cursorY=height()*3/4;
    cursorHight=height()/8;

    if (pixmap!=0)
      delete pixmap;

    if (pixmapSize < width())
      pixmap = new QPixmap(width(),height());
    else
      pixmap = new QPixmap(pixmapSize,height());

    //Start with the right offset
    if(width()>=pixmapSize)
    {
      pos=(int)((width()-pixmapSize)/2);
    }
    else
    {
      pos=(int)((width()-pixmapSize)*(offset));
    }

    QPainter painter;
    painter.begin (pixmap, this);
      painter.fillRect (pixmap->rect(),QColor(255,255,255));
      painter.setFont( font );
      painter.drawText(20,0,pixmapSize,height()/2,
                            QPainter::AlignLeft | QPainter::AlignVCenter,
                            teacherText);
      //draw <-|
      //<
      int unit=cursorHight/2;

      painter.drawLine(pixmapSize-70,height()/4,
                       pixmapSize-70+unit,(height()/4)-unit);
      painter.drawLine(pixmapSize-70,height()/4,
                       pixmapSize-70+unit,(height()/4)+unit);
      // -
      painter.drawLine(pixmapSize-70,height()/4,
                       pixmapSize-70+unit*4,height()/4);
      // |
      painter.drawLine(pixmapSize-70+unit*4,height()/4,
                       pixmapSize-70+unit*4,height()/4-unit*2);

    painter.end();

    needRecreate = false;
    textChanged  = true;
  }

  // if the text has changed we will draw the new text on the back ground image.
  if (textChanged)
  {
    QPainter painter;
    painter.begin (pixmap, this);
    {
      if(showError)
      {
        painter.fillRect (0,height()/2,pixmapSize,height(),QColor(255,0,0));
      }
      else
      {
        painter.fillRect (0,height()/2,pixmapSize,height(),QColor(200,255,255));
      }

      if (pixmapSize<width())
        painter.fillRect (pixmapSize,height()/2,width(),height(),QColor(255,255,255));
      painter.setFont( font );
      painter.drawText(20,height()/2,pixmapSize,height()/2,
                            QPainter::AlignLeft | QPainter::AlignVCenter,
                            studentText);
      if(cursorCount<3)
      {
        calculateCursor();
        // draw cursor
        painter.drawLine(cursorX,cursorY+cursorHight,
                         cursorX,cursorY-cursorHight);

      }
    }
    painter.end();
  }
  bitBlt (this, pos+1, 1, pixmap);
}

void SlideLine::resizeEvent (QResizeEvent *)
{
  needRecreate=true;
}

void SlideLine::calculateWidth()
{
  font = QFont("adobe-courier", height()/4);
  QFontMetrics fontMetrics( font );
  pixmapSize=fontMetrics.width(teacherText)+100;
}

void SlideLine::calculateCursor()
{
  QFontMetrics fontMetrics( font );
  cursorX=fontMetrics.width(studentText)+22;
}

