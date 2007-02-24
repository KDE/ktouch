/***************************************************************************
 *   Copyright (C) 2007 by Håvard Frøiland                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHTEXTLINEWIDGET_H
#define KTOUCHTEXTLINEWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsLineItem>
#include <QGraphicsPathItem>
#include <QTimer>
#include <Qt>

class KTouchTextLineWidget : public QWidget {
    Q_OBJECT

  public:
    KTouchTextLineWidget(QWidget *parent);
    ~KTouchTextLineWidget();

  public slots:
    void applyPreferences();
    void setTeacherText(const QString& text);
    void setStudentText(const QString& text);
    void setFont(const QFont& font);
    void setActive(bool active);
    void toggleCursor();
    void recalculatePosition();

  protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *){
        recalculateSize();
    }
    void recalculateSize();
    QPainterPath roundedRectPathFactory(double x, double y, double width, double height, double rounded);

  private:
    QGraphicsScene *scene;
    QGraphicsSimpleTextItem *teacher;
    QGraphicsPathItem *teacherRect;
    QGraphicsSimpleTextItem *student;
    QGraphicsPathItem *studentRect;
    QGraphicsLineItem *cursor;

    QFont font;

    QTimer *animationTimer;
    QTimer *cursorTimer;

    double pos;

    Qt::LayoutDirection direction;
};

#endif
