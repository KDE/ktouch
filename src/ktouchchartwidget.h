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

#ifndef KTOUCHCHARTWIDGET
#define KTOUCHCHARTWIDGET

#include <qframe.h>

class QResizeEvent;
class QPaintEvent;

class KTouchTrainer;

/// This is the chart widget and contains all chart drawing code.
class KTouchChartWidget : public QFrame {
    Q_OBJECT
  public:
    enum chart_t {
        CharsPerMinute,
        WordsPerMinute,
        Accuracy,
        ElapsedTime
    };
    /// Constructor.
    KTouchChartWidget(QWidget* parent, const char *name=0);
    /// Destructor.
    ~KTouchChartWidget();
    /// Sets the chart type and triggers an update.
    void setChartType(chart_t type);

    KTouchTrainer  *m_trainer;      ///< Pointer to the trainer object

  protected:
    void paintEvent(QPaintEvent *pe);

  private:
    chart_t         m_chartType;    ///< Type of chart to draw
};

#endif  // KTOUCHCHARTWIDGET
