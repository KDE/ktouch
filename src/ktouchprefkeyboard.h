/***************************************************************************
 *   ktouchprefkeyboard.h                                                  *
 *   --------------------                                                  *
 *   Copyright (C) 2003 by Andreas Nicolai                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHPREFKEYBOARD_H
#define KTOUCHPREFKEYBOARD_H

#include "ktouchprefkeyboardlayout.h"

/** This is the implementation of the "Keyboard settings" page of the preferences dialog.
 *  This class only contains the functionality which is used "inside" this page (e.g.
 *  connections between check boxes and enable/disable events). The main functionality
 *  of the preferences dialog is implemented in the class KTouchPref.
 */
class KTouchPrefKeyboard : public KTouchPrefKeyboardLayout {
  Q_OBJECT
  public:
    /// Default constructor.
    KTouchPrefKeyboard(QWidget *parent, const char *name=0, WFlags f=0);

	  QFont m_font;   // font used for keybaord
		void updateFont(const QFont& font);
	  
  public slots:
    /// Will be called when the choose font button has been pressed.
    void fontChooseBtnClicked();

  signals:
    /// Will be emitted when the user changed the fond settings in the font dialog.
    void fontChanged();

};
#endif  // KTOUCHPREFKEYBOARD_H
