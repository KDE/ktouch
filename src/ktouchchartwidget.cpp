/***************************************************************************
 *   ktouchchartwidget.cpp                                                 *
 *   ---------------------                                                 *
 *   Copyright (C) 2005 by Andreas Nicolai                                 *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchchartwidget.h"
#include "ktouchchartwidget.moc"

#include <libkdeedu/kdeeduplot/kplotaxis.h>

#include <QPainter>

#include <klocale.h>

#include <numeric>  // for mathematical functions
#include <cmath>

KTouchChartWidget::KTouchChartWidget(QWidget* parent, const char *name)
  : KPlotWidget(0.0, 120.0, 0.0, 240.0, parent)
{
	// temporary
	setObjectName( name );
	setUpdatesEnabled( false );
	setShowGrid( false ); // no grid please
	setBackgroundColor( Qt::white );
	setForegroundColor( Qt::black );
	axis( BottomAxis )->setLabel( i18n( "Progress" ) );
	axis( LeftAxis )->setLabel( i18n( "Words per second" ) );
	setUpdatesEnabled( true );
	update();
}

KTouchChartWidget::~KTouchChartWidget()
{
}
