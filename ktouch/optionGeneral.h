/***************************************************************************
                          optionGeneral.h  -  description
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

#ifndef OPTIONGENERAL_H
#define OPTIONGENERAL_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qcolor.h>
#include <kcolordialog.h>

#include "optionGeneralDlg.h"


class OptionGeneral : public OptionGeneralDlg  {
   Q_OBJECT
public:
         /** constructor */
   OptionGeneral(QWidget *parent=0, const char *name=0,bool modal=true);
          /** destructor */
   ~OptionGeneral();

protected slots:
   void setColor();

protected:
   virtual void keyPressEvent(QKeyEvent *);
};

#endif

