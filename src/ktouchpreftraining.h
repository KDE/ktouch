/***************************************************************************
 *   ktouchpreftraining.h                                                  *
 *   --------------------                                                  *
 *   Copyright (C) 2003 by Andreas Nicolai                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHPREFTRAINING_H
#define KTOUCHPREFTRAINING_H

#include <ktouchpreftraininglayout.h>

/** This is the implementation of the "Training options" page of the preferences dialog.
 *  This class only contains the functionality which is used "inside" this page (e.g.
 *  connections between check boxes and enable/disable events). The main functionality
 *  of the preferences dialog is implemented in the class KTouchPref.
 */
class KTouchPrefTraining : public KTouchPrefTrainingLayout {
    Q_OBJECT
  public:
    /// Default constructor.
    KTouchPrefTraining(QWidget *parent, const char *name=0, WFlags f=0);

  public slots:
    /// Enables/Disables the automatic level changing options.
    void levelChangeToggled(bool on);
};

#endif  // KTOUCHPREFTRAINING_H
