/***************************************************************************
 *   ktouchprefgeneral.h                                                   *
 *   -------------------                                                   *
 *   Copyright (C) 2003 by Andreas Nicolai                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHPREFGENERAL_H
#define KTOUCHPREFGENERAL_H

#include <qfont.h>
#include "ktouchprefgenerallayout.h"

/** This is the implementation of the "General options" page of the preferences dialog.
 *  This class only contains the functionality which is used "inside" this page (e.g.
 *  connections between check boxes and enable/disable events). The main functionality
 *  of the preferences dialog is implemented in the class KTouchPref.
 */
class KTouchPrefGeneral : public KTouchPrefGeneralLayout {
  Q_OBJECT
  public:
    /// Default constructor.
    KTouchPrefGeneral(QWidget *parent, const char *name=0, WFlags f=0);
    /// Updates the font m_font and the font display label.
    void updateFont(const QFont& font);

    QFont m_font;   ///< The font that can be chosen for the sliding lines.

  public slots:
    /// Will be called when the choose font button has been pressed.
    void fontChooseBtnClicked();

  signals:
    /// Will be emitted when the user changed the fond settings in the font dialog.
    void fontChanged();
};

#endif  // KTOUCHPREFGENERAL_H
