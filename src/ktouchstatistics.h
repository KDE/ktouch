/***************************************************************************
 *   ktouchstatistics.h                                                    *
 *   ------------------                                                    *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHSTATISTICS_H
#define KTOUCHSTATISTICS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "ktouchstatistics_dlg.h"
#include "ktouchstatisticsdata.h"

class QShowEvent;
class KTouch;
class KURL;

/// Implementation of the statistics widget
class KTouchStatistics : public KTouchStatisticsDlg {
    Q_OBJECT
  public:
    /// Constructor, takes a pointer to the trainer object.
    KTouchStatistics(QWidget* parent);

	/// Fills the statistics dialog with data and executes it.
	void run(const KURL& currentLecture, const KTouchStatisticsData& stats,
		const KTouchLevelStats& currLevelStats,
		const KTouchSessionStats& currSessionStats);

  public slots:
	/// Called whenever the lecture is changed.
    void lectureActivated(int index);
	/// Clears the statistics data.
	void clearHistory();
	/// Updates the chart with current lecture and settings
	void updateChartTab();

  private:
	/// Updates the data in the current tab, using the data stored in m_currSessionStats.
	void updateCurrentSessionTab();
	/// Updates the data in the current tab, using the data stored in m_currLevelStats.
	void updateCurrentLevelTab();

	unsigned int			m_currentIndex;		///< Index in lecture combo of the current lecture.
	unsigned int			m_lectureIndex;		///< Index of currently selected lecture.
	KTouchStatisticsData 	m_allStats;			///< Contains all lecture statistics.
	KTouchLevelStats 		m_currLevelStats;	///< Contains current level stats (not yet stored in allStats).
	KTouchSessionStats		m_currSessionStats;	///< Contains current session stats (not yet stored in allStats).
};

#endif  // KTOUCHSTATISTICS_H
