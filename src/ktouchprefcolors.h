/***************************************************************************
 *   ktouchprefcolors.h                                                    *
 *   ------------------                                                    *
 *   Copyright (C) 2003 by Andreas Nicolai                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHPREFCOLORS_H
#define KTOUCHPREFCOLORS_H

#include <qwidget.h>
#include "ktouchprefcolorslayout.h"

/// Implementation of the "color settings" preferences page
class KTouchPrefColors : public KTouchPrefColorsLayout {
    Q_OBJECT
  public:
    /// Constructor
    KTouchPrefColors(QWidget *parent, const char *name=0, WFlags f=0);
    /// Destructor
    ~KTouchPrefColors();

  public slots:
    /// Enables/Disables the error color settings.
    void errorColorCheckToggled(bool on);
};

#endif  // KTOUCHPREFCOLORS_H
