/***************************************************************************
 *   ktouchstatus.h                                                        *
 *   --------------                                                        *
 *   Copyright (C) 2000 by H�ard Friland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHSTATUS_H
#define KTOUCHSTATUS_H

#include "ktouchstatuslayout.h"

/** This is the implementation of the status widget (at the top of the main view).
 *  It simply displays the status but does not have a functionality of its own.
 *  Everythings is calculated in the trainer (see KTouchTrainer)and then displayed
 *  using the updateStatus() member function.
 *  The "new characters" string is set using the member function setNewChars().
 */
class KTouchStatus : public KTouchStatusLayout {
    Q_OBJECT
  public:
    /// Constructor
    KTouchStatus(QWidget *parent);
    /// Sets the new characters text.
    void setNewChars(const QString& newChars);
    /// Updates the level LCD and the correctness progress bar
    void updateStatus(unsigned int level, double correctness);
};

#endif  // KTOUCHSTATUS_H
