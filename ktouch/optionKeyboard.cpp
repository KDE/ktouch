/***************************************************************************
                          optionKeyboard.cpp  -  description
                             -------------------
    begin                : Tue Mar 13 2001
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

#include "optionKeyboard.h"
#include "optionKeyboard.moc"

OptionKeyboard::OptionKeyboard(QWidget *parent, const char *name, bool modal)
               : OptionKeyboardDlg(parent, name, modal)
{
}

OptionKeyboard::~OptionKeyboard()
{
}

void OptionKeyboard::keyPressEvent(QKeyEvent *)
{
  //just to stop the keyPressEvent to go to the main window.
}
