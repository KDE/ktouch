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

#ifndef KTOUCHSTATISTICS_H
#define KTOUCHSTATISTICS_H

#include <qwidget.h>
#include "ktouchstatisticslayout.h"

class QShowEvent;
class QResizeEvent;
class QPixmap;
class KTouchTrainer;

/// Implementation of the statistics widget
class KTouchStatistics : public KTouchStatisticsLayout {
    Q_OBJECT
  public:
    /// Constructor, takes a pointer to the trainer object.
    KTouchStatistics(QWidget *parent, KTouchTrainer* trainer);
    /// Destructor
    ~KTouchStatistics();

  protected:
    /// Updates the tab pages.
    void showEvent(QShowEvent*);

  private slots:
    /// Asks for confirmation and clears the history.
    void clearHistory();
    /// Updates the "averaged session statistics" (when session cound slider changes).
    void updateAverageTab();
    /// Updates content of "charts" page (actually sets the chart type so that the widget can draw itself again).
    void updateChartTab();

  private:
    /// Updates content of "current session statistics" page.
    void updateCurrentTab();

    KTouchTrainer  *m_trainer;      ///< Pointer to the trainer object of KTouch.
};

#endif  // KTOUCHSTATISTICS_H
