/***************************************************************************
                          optionKeyboard.h  -  description
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

#ifndef OPTIONKEYBOARD_H
#define OPTIONKEYBOARD_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "optionKeyboardDlg.h"

class OptionKeyboard : public OptionKeyboardDlg  {
   Q_OBJECT
public:
         /** constructor */
   OptionKeyboard(QWidget *parent=0, const char *name=0,bool modal=true);
          /** destructor */
   ~OptionKeyboard();

protected:
   virtual void keyPressEvent(QKeyEvent *);
};

#endif

