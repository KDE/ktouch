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

OptionGeneral::OptionGeneral(QWidget *parent, const char *name, bool modal)
              : OptionGeneralDlg(parent, name, modal)
{
}

OptionGeneral::~OptionGeneral()
{
}

void OptionGeneral::keyPressEvent(QKeyEvent *)
{
  //just to stop the keyPressEvent to go to the main window.
}
