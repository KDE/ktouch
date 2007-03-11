/***************************************************************************
 *   Copyright (C) 2007 by Håvard Frøiland                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchtextlinewidget.h"
#include "ktouchtextlinewidget.moc"

#include "prefs.h"

#include <QPainter>

KTouchTextLineWidget::KTouchTextLineWidget(QWidget *parent)
  : QGraphicsView( parent )
{
    scene = new QGraphicsScene(parent);

    teacher = new QGraphicsSimpleTextItem();
    teacherRect = new QGraphicsPathItem();
    teacherRect->setZValue(-1);

    scene->addItem(teacher);
    scene->addItem(teacherRect);

    student = new QGraphicsSimpleTextItem();
    studentRect = new QGraphicsPathItem();
    studentRect->setZValue(-1);

    scene->addItem(student);
    scene->addItem(studentRect);

    cursor = new QGraphicsLineItem();
    cursor->setZValue(1);
    cursor->setPen(QPen(QBrush(Qt::black), 2));
    scene->addItem(cursor);

    animationTimer = new QTimer(this);

    cursorTimer = new QTimer(this);
    cursorTimer->setInterval(500);

    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing);

    setBackgroundBrush(palette().brush(QPalette::Window));
    setFrameStyle(QFrame::NoFrame);

    connect(animationTimer, SIGNAL(timeout()), this, SLOT(recalculatePosition()));
    connect(cursorTimer, SIGNAL(timeout()), this, SLOT(toggleCursor()));

    direction = parent->layoutDirection();

    applyPreferences();
}

KTouchTextLineWidget::~KTouchTextLineWidget() {
}

void KTouchTextLineWidget::applyPreferences() {
    setMaximumHeight(Prefs::maxSlidingWidgetHeight());

    if (Prefs::overrideLectureFont())
        font = Prefs::font();

    student->setBrush(QBrush(Prefs::studentTextColor()));
    teacher->setBrush(QBrush(Prefs::teacherTextColor()));
    teacherRect->setBrush(QBrush(Prefs::teacherBackgroundColor()));
    studentRect->setBrush(QBrush(Prefs::studentBackgroundColor()));

    animationTimer->setInterval(100 - Prefs::slidingSpeed());

    recalculateSize();
}


void KTouchTextLineWidget::setTeacherText(const QString& text) {
    teacher->setText(text);
    recalculateSize();
    setStudentText("");
}


void KTouchTextLineWidget::setStudentText(const QString& text) {
    student->setText(text);
    double studentTextWidth = student->boundingRect().width();
    double teacherTextWidth = teacher->boundingRect().width();

    if(direction == Qt::LeftToRight)
        cursor->setPos(studentTextWidth, 0);
    else
    {
        cursor->setPos(teacherTextWidth - studentTextWidth, 0);
        student->setPos(teacherTextWidth - studentTextWidth, teacher->boundingRect().height() + 5);
    }

    if(teacher->text().startsWith(text)){
        studentRect->setBrush(QBrush(Prefs::studentBackgroundColor()));
        student->setBrush(QBrush(Prefs::studentTextColor()));
    }
    else{
        studentRect->setBrush(QBrush(Prefs::errorBackgroundColor()));
        student->setBrush(QBrush(Prefs::errorTextColor()));
   }
    animationTimer->start();
}

void KTouchTextLineWidget::setFont(const QFont& f) {
    if (Prefs::overrideLectureFont())
        font = Prefs::font();
    else
        font= f;
}

void KTouchTextLineWidget::recalculateSize(){
    font.setPointSize(height()/4);
    teacher->setFont(font);

    QRectF rect = teacher->boundingRect();

    student->setFont(font);

    studentRect->setPath(roundedRectPathFactory(rect.x()-5, rect.y(), rect.width()+10, rect.height(), qMin(rect.height(), rect.width())/2));
    teacherRect->setPath(roundedRectPathFactory(rect.x()-5, rect.y(), rect.width()+10, rect.height(), qMin(rect.height(), rect.width())/2));

    if(direction == Qt::LeftToRight){
        cursor->setLine(2,rect.height() + 5  + 5, 2,rect.height() * 2 + 5 - 10);
        cursor->setPos(student->boundingRect().width(), 0);
        student->setPos(0, rect.height() + 5);
    }
    else{
        cursor->setLine(-2,rect.height() + 5  + 5, -2,rect.height() * 2 + 5 - 10);
        cursor->setPos(teacher->boundingRect().width() - student->boundingRect().width(), 0);
        student->setPos(teacher->boundingRect().width() - student->boundingRect().width(), rect.height() + 5);
    }

    studentRect->setPos(0, rect.height() + 5);

    animationTimer->start();
}

void KTouchTextLineWidget::recalculatePosition() {
    double padding = 40;
    double maxPos = teacher->boundingRect().width();

    double studentWidth = student->boundingRect().width();

    double ration = studentWidth/maxPos;
    double diff = maxPos - width();

    double wantedPos;
    if(diff < -padding){
        wantedPos = diff/2;
    }
    else{
        if(direction == Qt::LeftToRight)
            wantedPos = diff * ration - (padding * (1 - (2*ration)));
        else
            wantedPos = diff - (diff * ration - (padding * (1 - (2*ration))));
    }
    pos = pos + (wantedPos - pos) / 5;

    setSceneRect(pos, 0 , width(), height());

    if(qAbs(wantedPos - pos)<1)
        animationTimer->stop();
}

void KTouchTextLineWidget::setActive(bool active){
    if(active)
        cursorTimer->start();
    else
    {
        cursorTimer->stop();
        cursor->setVisible(true);
    }
}

void KTouchTextLineWidget::toggleCursor(){
    cursor->setVisible(!cursor->isVisible());
}

QPainterPath KTouchTextLineWidget::roundedRectPathFactory(double x, double y, double width, double height, double rounded){
    QPainterPath roundRectPath;
    roundRectPath.moveTo(width+x, y+rounded/2);
    roundRectPath.arcTo(width+x-rounded, y, rounded, rounded, 0.0, 90.0);
    roundRectPath.lineTo(x+rounded/2, y);
    roundRectPath.arcTo(x, y, rounded, rounded, 90.0, 90.0);
    roundRectPath.lineTo(x, height+y-rounded/2);
    roundRectPath.arcTo(x, height+y-rounded, rounded, rounded, 180.0, 90.0);
    roundRectPath.lineTo(width+x-rounded/2, height+y);
    roundRectPath.arcTo(width+x-rounded, height+y-rounded, rounded, rounded, 270.0, 90.0);
    roundRectPath.closeSubpath();
    return roundRectPath;
}
