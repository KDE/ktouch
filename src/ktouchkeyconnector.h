/***************************************************************************
 *   ktouchkeyconnetor.h                                                   *
 *   -------------------                                                   *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHKEYCONNECTOR_H
#define KTOUCHKEYCONNECTOR_H


#include "ktouchkeys.h"

 /// This is the key connector which represents a key or key combination that can be pressed.
 /// It associates a character with a target key and optionally a finger and/or control key.
class KTouchKeyConnector {
  public:
    /// Default constructor
    KTouchKeyConnector() : m_keyChar(0), m_targetKeyChar(0), m_fingerKeyChar(0), m_controlKeyChar(0) {};
    /// Constructor provided for convenience.
    KTouchKeyConnector(const QChar& keyChar, const QChar& targetKeyChar, const QChar& fingerKeyChar,
                       const QChar& controlKeyChar)
      : m_keyChar(keyChar), m_targetKeyChar(targetKeyChar), m_fingerKeyChar(fingerKeyChar), m_controlKeyChar(controlKeyChar) {};

    /// This is the character that has to be pressed to access this key.
    QChar               m_keyChar;

    QChar               m_targetKeyChar;    ///< The target character (the key the user has to press).
    QChar               m_fingerKeyChar;    ///< The finger key (where the used finger normally rests).
    QChar               m_controlKeyChar;   ///< The modifier key the user has to press also (for example LeftShift).
};

#endif  // KTOUCHKEYCONNECTOR_H
