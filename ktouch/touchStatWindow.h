/***************************************************************************
                          touchStatWindow.h  -  description
                             -------------------
    begin                : Thu Sep 5 2001
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

#ifndef TOUCHSTATWINDOW_H
#define TOUCHSTATWINDOW_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qtimer.h>
#include <qlcdnumber.h>
#include <qprogressbar.h>
#include <qtextview.h>
#include <qlabel.h>

#include "touchStatWindowLayout.h"
#include "touchstat.h"

class TouchStatWindow : public TouchStatWindowLayout
{
    Q_OBJECT
	public:
    /** constructor */
    TouchStatWindow(TouchStat *stat, QWidget *parent=0, const char *name=0,bool modal=false);
    /** destructor */
    ~TouchStatWindow();
    
 private:
    // timer used to call updateToday()
    QTimer *timer;
    
    // pointer to the real statistic data
    TouchStat *stat;
    
    private slots:
	
	
	void updateToday();
    void autoUpdate(int);
    void updateFiveTimes();
    void updateTwentyTimes();
    void updateAllTimes();
    
    void setFocuseChar();
    
};

#endif
