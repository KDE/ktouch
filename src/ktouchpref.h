/***************************************************************************
 *   ktouchpref.h                                                          *
 *   ------------                                                          *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHPREF_H
#define KTOUCHPREF_H

#include <kdialogbase.h>

class KTouchPrefGeneral;
class KTouchPrefTraining;
class KTouchPrefKeyboard;
class KTouchPrefColors;

/** The preferences dialog of KTouch.
 *  This is a typical preferences dialog and behaves as such :-).
 *  The most important function in this dialog is the update() member function. It transfers -
 *  depending on the parameter - the values of the global preferences data object to the dialog UI
 *  widgets or vice versa. So it will be called as update(true) just before showing the dialog
 *  and as update(false) whenever the Apply or the OK button is pressed. Note that pressing
 *  the apply button will trigger the applyPreferences() signal, which causes an update of all
 *  the widgets of the KTouch program (the corresponding slot is in the class KTouch). The
 *  changes will saved via the KTouchSettings::saveSettings() member function. This saves
 *  our poor preferences dialog the work of knowing and handling configuration data :-)<p>
 *  Note: when adding a new option remember to add a slot connection to setModified (contructor),
 *  add the values in update() and setDefaults() and of course change the KTouchSettings object.
 */
class KTouchPref : public KDialogBase {
    Q_OBJECT
  public:
    /// Constructor
    KTouchPref();
    /// Sets defaults in all pages of the preferences dialog (but pops up a confirmation dialog first).
    void setDefaults();
    /// Transfers data from the settings object towards the dialog (toDialog==true) or vice versa.
    void update(bool toDialog);

  public slots:
    /// Will be called when the "Default" button has been pressed
    void slotDefault();
    /// Will be called when the "Apply" button has been pressed.
    void slotApply();
    /// Will be called whenever an option has been changed.
    void setModified();

  signals:
    /// This signal is emitted when the Apply button has been clicked.
    void applyPreferences();

  private:
    KTouchPrefGeneral   *m_pageGeneral;   ///< the "General options" page
    KTouchPrefTraining  *m_pageTraining;  ///< the "Training options" page
    KTouchPrefKeyboard  *m_pageKeyboard;  ///< the "Keyboard settings" page
    KTouchPrefColors    *m_pageColors;    ///< the "Color settings" page
};

#endif  // KTOUCHPREF_H
