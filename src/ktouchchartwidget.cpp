/***************************************************************************
 *   ktouchstatistics.cpp                                                  *
 *   --------------------                                                  *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchchartwidget.h"
#include "ktouchchartwidget.moc"

#include <qpainter.h>

#include <klocale.h>

#include <numeric>  // for mathematical functions
#include <cmath>

#include "ktouchtrainer.h"

KTouchChartWidget::KTouchChartWidget(QWidget* parent, const char *name)
  : QFrame(parent, name), m_trainer(NULL)
{
    setFrameShape( QFrame::StyledPanel );
    setFrameShadow( QFrame::Sunken );
    setLineWidth( 2 );
    setEraseColor( Qt::white );
}

KTouchChartWidget::~KTouchChartWidget()
{
}

void KTouchChartWidget::setChartType(chart_t type) {
    m_chartType = type;
    update();
}

void KTouchChartWidget::paintEvent(QPaintEvent *pe) {
    QFrame::paintEvent(pe); // let the base class draw the frame
    if (m_trainer==NULL || m_trainer->m_sessionHistory.size()<2)
        return;
    QPainter p(this);
    QRect chartFrame(50,10,rect().width()-60, rect().height()-50);
    unsigned int chartXCenter = (chartFrame.left()+chartFrame.right())/2;
    p.setPen( Qt::black );
    // Draw coordinate axes
    p.drawLine( chartFrame.left(), chartFrame.bottom(), chartFrame.right(), chartFrame.bottom());
    p.drawLine( chartFrame.left(), chartFrame.top(), chartFrame.left(), chartFrame.bottom());
    // Draw x-achses text
    QFont font = p.font();
    font.setPointSize(10);
    p.setFont( font );
    p.drawText( chartFrame.left(), chartFrame.bottom() + 3,
               chartFrame.width()/2, rect().bottom()-5, Qt::AlignLeft | Qt::AlignTop, i18n("first") );
    p.drawText( chartXCenter, chartFrame.bottom() + 3,
               chartFrame.width()/2, rect().bottom()-5, Qt::AlignRight | Qt::AlignTop, i18n("most recent") );
    font.setPointSize(15);
    font.setWeight( QFont::DemiBold );
    p.setFont( font );
    p.drawText( chartFrame.left(), chartFrame.bottom()+13,
               chartFrame.width(), rect().bottom()-chartFrame.bottom()-18,
               Qt::AlignHCenter | Qt::AlignBottom , i18n("Training sessions") );
    // Draw y-achses text
    QString yAxisText;
    switch (m_chartType) {
      case CharsPerMinute   : yAxisText=i18n("Characters per minute"); break;
      case WordsPerMinute   : yAxisText=i18n("Words per minute"); break;
      case Accuracy         : yAxisText=i18n("Accuracy"); break;
      case ElapsedTime      : yAxisText=i18n("Elapsed training time"); break;
    };
    p.rotate( 270 );
    p.drawText( -chartFrame.bottom(), 5, chartFrame.bottom(), 40, Qt::AlignHCenter | Qt::AlignTop, yAxisText );
    // reset coordinate system and font
    p.rotate(-270);
    font.setPointSize(10);
    font.setWeight( QFont::Normal );
    p.setFont( font );
    // calculate x-distances between points
    double xstep = static_cast<double>(chartFrame.width())/(m_trainer->m_sessionHistory.size()-1);
    switch (m_chartType) {
        case CharsPerMinute :
        {
            // first find the maximum speed
            double maxCharSpeed=0;
            for (QValueList<KTouchTrainingSession>::const_iterator it=m_trainer->m_sessionHistory.begin();
                it!=m_trainer->m_sessionHistory.end();++it)
            {
                if ((*it).charSpeed()>maxCharSpeed)
                    maxCharSpeed=(*it).charSpeed();
            };
            // round to a nice value
            maxCharSpeed = std::ceil(maxCharSpeed/25) * 25;
            // draw vertical axis marks
            p.drawText(0, chartFrame.bottom()-5, chartFrame.left()-5, 10, Qt::AlignRight | Qt::AlignVCenter, "0");
            p.drawText(0, chartFrame.top()-5, chartFrame.left()-5, 10, Qt::AlignRight | Qt::AlignVCenter,
                QString("").setNum(static_cast<int>(maxCharSpeed)) );
            // calculate y-scaling factor
            double yScale = chartFrame.height()/maxCharSpeed;
            // loop over all training sessions and chart
            double x = chartFrame.left();
            double lastX=x;
            int lastY=0;
            for (QValueList<KTouchTrainingSession>::const_iterator it=m_trainer->m_sessionHistory.begin();
                it!=m_trainer->m_sessionHistory.end(); ++it, x+=xstep)
            {
                int y = chartFrame.bottom() - static_cast<int>(yScale*(*it).charSpeed());
                p.setPen( QPen(Qt::black, 4) );
                p.drawPoint(static_cast<int>(x), y);
                p.setPen( QPen(Qt::black, 0) );
                if (it!=m_trainer->m_sessionHistory.begin())
                    p.drawLine(static_cast<int>(lastX), lastY, static_cast<int>(x), y);
                lastX=x;
                lastY=y;
            };
        };
        break;

        case WordsPerMinute :
        {
            // first find the maximum speed
            double maxWordSpeed=0;
            for (QValueList<KTouchTrainingSession>::const_iterator it=m_trainer->m_sessionHistory.begin();
                it!=m_trainer->m_sessionHistory.end();++it)
            {
                if ((*it).wordSpeed()>maxWordSpeed)
                    maxWordSpeed=(*it).wordSpeed();
            };
            // round to a nice value
            maxWordSpeed = std::ceil(maxWordSpeed/25) * 25;
            // draw vertical axis marks
            p.drawText(0, chartFrame.bottom()-5, chartFrame.left()-5, 10, Qt::AlignRight | Qt::AlignVCenter, "0");
            p.drawText(0, chartFrame.top()-5, chartFrame.left()-5, 10, Qt::AlignRight | Qt::AlignVCenter,
                QString("").setNum(static_cast<int>(maxWordSpeed)) );
            // calculate y-scaling factor
            double yScale = chartFrame.height()/maxWordSpeed;
            // loop over all training sessions and draw chart
            double x = chartFrame.left();
            double lastX=x;
            int lastY=0;
            for (QValueList<KTouchTrainingSession>::const_iterator it=m_trainer->m_sessionHistory.begin();
                it!=m_trainer->m_sessionHistory.end(); ++it, x+=xstep)
            {
                int y = chartFrame.bottom() - static_cast<int>(yScale*(*it).wordSpeed());
                p.setPen( QPen(Qt::black, 4) );
                p.drawPoint(static_cast<int>(x), y);
                p.setPen( QPen(Qt::black, 0) );
                if (it!=m_trainer->m_sessionHistory.begin())
                    p.drawLine(static_cast<int>(lastX), lastY, static_cast<int>(x), y);
                lastX=x;
                lastY=y;
            };
        };
        break;

        case Accuracy :
        {
            // draw vertical axis marks
            p.drawText(0, chartFrame.bottom()-5, chartFrame.left()-5, 10, Qt::AlignRight | Qt::AlignVCenter, "0 %");
            p.drawText(0, chartFrame.top()-5, chartFrame.left()-5, 10, Qt::AlignRight | Qt::AlignVCenter, "100 %");
            // y-scaling factor == available chart height
            double yScale = chartFrame.height();
            // loop over all training sessions and draw chart
            double x = chartFrame.left();
            double lastX=x;
            int lastY=0;
            for (QValueList<KTouchTrainingSession>::const_iterator it=m_trainer->m_sessionHistory.begin();
                it!=m_trainer->m_sessionHistory.end(); ++it, x+=xstep)
            {
                int y = chartFrame.bottom() - static_cast<int>(yScale*(*it).correctness());
                p.setPen( QPen(Qt::black, 4) );
                p.drawPoint(static_cast<int>(x), y);
                p.setPen( QPen(Qt::black, 0) );
                if (it!=m_trainer->m_sessionHistory.begin())
                    p.drawLine(static_cast<int>(lastX), lastY, static_cast<int>(x), y);
                lastX=x;
                lastY=y;
            };
        };
        break;

        case ElapsedTime :
        {
            // first find the maximum speed
            double time=0;
            for (QValueList<KTouchTrainingSession>::const_iterator it=m_trainer->m_sessionHistory.begin();
                it!=m_trainer->m_sessionHistory.end();++it)
            {
                if ((*it).m_elapsedTime>time)
                    time=(*it).m_elapsedTime;
            };
            // round time to a nice number
            time = std::ceil(time/6000) * 6000;
            // calculate axis string
            QString yAxisText;
            if (time<60000)
                yAxisText = QString("%1 s").arg(static_cast<int>(time/1000));
            else
                yAxisText = QString("%1 min").arg(static_cast<int>(time/60000));

            // draw vertical axis marks
            p.drawText(0, chartFrame.bottom()-5, chartFrame.left()-5, 10, Qt::AlignRight | Qt::AlignVCenter, "0 s");
            p.drawText(0, chartFrame.top()-5, chartFrame.left()-5, 10, Qt::AlignRight | Qt::AlignVCenter, yAxisText);
            // calculate y-scaling factor
            double yScale = chartFrame.height()/time;
            // loop over all training sessions and draw typing speed chart
            double x = chartFrame.left();
            double lastX=x;
            int lastY=0;
            for (QValueList<KTouchTrainingSession>::const_iterator it=m_trainer->m_sessionHistory.begin();
                it!=m_trainer->m_sessionHistory.end(); ++it, x+=xstep)
            {
                int y = chartFrame.bottom() - static_cast<int>(yScale*(*it).m_elapsedTime);
                p.setPen( QPen(Qt::black, 4) );
                p.drawPoint(static_cast<int>(x), y);
                p.setPen( QPen(Qt::black, 0) );
                if (it!=m_trainer->m_sessionHistory.begin())
                    p.drawLine(static_cast<int>(lastX), lastY, static_cast<int>(x), y);
                lastX=x;
                lastY=y;
            };
        };
        break;
    };
}
