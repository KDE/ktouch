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
    updateFiveTimes();
    updateTwentyTimes();
    updateAllTimes();
    
    timer = new QTimer(this,"timer");
    
    connect( timer, SIGNAL(timeout()), SLOT(updateToday()));
    timer->start(1000,false);
}

TouchStatWindow::~TouchStatWindow()
{

}

void TouchStatWindow::reject()
{
    emit statShowSetChecked(false);
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
    setFocuseChar();
    
}

void TouchStatWindow::updateFiveTimes()
{

}

void TouchStatWindow::updateTwentyTimes()
{

}

void TouchStatWindow::updateAllTimes()
{

}

void TouchStatWindow::autoUpdate(int i)
{

    if(i)
	timer->start(1000,false);
    else
	timer->stop();
}

// This part should be changed, It's crap!!
void TouchStatWindow::setFocuseChar()
{
	QString s;
	float ratio;

	int bad[]={0,0,0,0,0};
	float badValue[]={0,0,0,0,0};

	for(int i=0;i<512;i++)
	{
		if((stat->arrayStat[i].error+stat->arrayStat[i].ok)>0)
			ratio=((float)stat->arrayStat[i].error/(float)(stat->arrayStat[i].ok+stat->arrayStat[i].error))*100;
		else
			ratio=0;

		float min=100;
		int minPos=0;
		for(int j=0;j<5;j++)
		{
			if(badValue[j]<min)
			{
				min=badValue[j];
				minPos=j;
			}
		}

		if(min<ratio)
		{
			bad[minPos]=i;
			badValue[minPos]=ratio;
		}
	}
	problemChar1->setText(QChar(bad[0]));
	problemProg1->setProgress((int)badValue[0]);

	problemChar2->setText(QChar(bad[1]));
	problemProg2->setProgress((int)badValue[1]);

	problemChar3->setText(QChar(bad[2]));
	problemProg3->setProgress((int)badValue[2]);

	problemChar4->setText(QChar(bad[3]));
	problemProg4->setProgress((int)badValue[3]);

	problemChar5->setText(QChar(bad[4]));
	problemProg5->setProgress((int)badValue[4]);
}
