/***************************************************************************
                          optionGeneral.cpp  -  description
                             -------------------
    begin                : Thu Apr 5 2001
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

#include "optionGeneral.h"

#include <kcolordialog.h>
#include "optionGeneral.moc"


OptionGeneral::OptionGeneral(QWidget *parent, const char *name, bool modal)
              : OptionGeneralDlg(parent, name, modal)
{
  // 4 is size list
  fontChooser->enableColumn(4 , false );
}

OptionGeneral::~OptionGeneral()
{
}

void OptionGeneral::setColor()
{
  QColor newColor;
  newColor=showColor->backgroundColor();
  int result = KColorDialog::getColor( newColor );
  if ( result == KColorDialog::Accepted )
  {
    showColor->setBackgroundColor( newColor );
  }
}

void OptionGeneral::keyPressEvent(QKeyEvent *)
{
  //just to stop the keyPressEvent to go to the main window.
}

