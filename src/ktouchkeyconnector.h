/***************************************************************************
 *   ktouchkeyconnetor.h                                                   *
 *   -------------------                                                   *
 *   Copyright (C) 2004 by Andreas Nicolai                                 *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHKEYCONNECTOR_H
#define KTOUCHKEYCONNECTOR_H

#include <QVector>

#include "ktouchkey.h"

 /// The KTouchKeyConnector class contains the information about the character that has to 
 /// be pressed and the keys on the keyboard that should be highlighted.
 /// It associates a character with a target key and optionally a finger and/or control key.
class KTouchKeyConnector {
  public:
    /// Default constructor
    KTouchKeyConnector() : m_keyChar(-1), m_targetKeyIndex(-1), m_fingerKeyIndex(-1), m_modifierKeyIndex(-1)
	{}
    /// Constructor provided for convenience.
    KTouchKeyConnector(int keyChar, int target_key, int finger_key, int modifier_key)
	  : m_keyChar(keyChar), m_targetKeyIndex(target_key), m_fingerKeyIndex(finger_key),
		m_modifierKeyIndex(modifier_key)
	{}

	/// Reads the key connector data from the DomElement
	bool read(QDomNode& node);
	/// Creates a new DomElement, writes the key connector data into it and appends it to the root object.
	void write(QDomDocument& doc, QDomElement& root) const;

    int               	m_keyChar;				///< This is the unicode number for the character that has to be pressed to access this key.
    int					m_targetKeyIndex;		///< Index of the target key (-1 if none).
    int					m_fingerKeyIndex;		///< Index of the finger key (-1 if none).
	int					m_modifierKeyIndex;		///< Index of the modifier key (-1 if none).
};

#endif  // KTOUCHKEYCONNECTOR_H
