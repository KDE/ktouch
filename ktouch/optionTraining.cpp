/***************************************************************************
                          optionTraining.cpp  -  description
                             -------------------
    begin                : Wed Mar 14 2001
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


#include "optionTraining.h"
#include "optionTraining.moc"

OptionTraining::OptionTraining(QWidget *parent, const char *name, bool modal)
               : OptionTrainingDlg(parent, name, modal)
{
}

OptionTraining::~OptionTraining()
{
}

void OptionTraining::keyPressEvent(QKeyEvent *)
{
  //just to stop the keyPressEvent to go to the main window.
}
