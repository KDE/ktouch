/***************************************************************************
                          touchStatWindow.cpp  -  description
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

#include "touchStatWindow.h"
#include "touchStatWindow.moc"


TouchStatWindow::TouchStatWindow(TouchStat *s, QWidget *parent, const char *name, bool modal)
               : TouchStatWindowLayout(parent, name, modal)
{
	stat=s;
	updateToday();

	timer = new QTimer(this,"timer");

	connect( timer, SIGNAL(timeout()), SLOT(updateToday()));
	timer->start(1000,false);

}

TouchStatWindow::~TouchStatWindow()
{
}

void TouchStatWindow::updateToday()
{
	// total
	totalWords ->display(stat->wordCount);
	totalOk    ->display(stat->okCount);
	totalError ->display(stat->errorCount);
	totalTime  ->display(stat->getTotalTime()/1000);

	// typing rate
	wordMin    ->display(stat->getWordPerMin());
	charMin    ->display(stat->getCharPerMin());

	// accurancy
	acc        ->setProgress(stat->getRatio());

	// Proggress
	progress   ->setText(stat->getProgText());

}

void TouchStatWindow::autoUpdate(int i)
{
	if(i)
		timer->start(1000,false);
	else
		timer->stop();
}
