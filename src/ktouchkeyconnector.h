/***************************************************************************
 *   ktouchkeyconnetor.h                                                   *
 *   -------------------                                                   *
 *   Copyright (C) 2007 by Andreas Nicolai                                 *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHKEYCONNECTOR_H
#define KTOUCHKEYCONNECTOR_H

class QDomDocument;
class QDomNode;
class QDomElement;

 /// The KTouchKeyConnector class contains the information about the character that has to 
 /// be pressed and the keys on the keyboard that should be highlighted.
 /// It associates a character with a target key and optionally a finger and/or control key.
class KTouchKeyConnector {
  public:
    /// Default constructor
    KTouchKeyConnector() : m_keyUnicode(-1), m_targetKeyIndex(-1), m_modifierKeyIndex(-1)
	{}
    /// Constructor provided for convenience.
    KTouchKeyConnector(int keyUnicode, int target_key, int modifier_key)
	  : m_keyUnicode(keyUnicode), m_targetKeyIndex(target_key), m_modifierKeyIndex(modifier_key)
	{}

	/// Reads the key connector data from the DomElement
	bool read(QDomElement e);
	/// Creates a new DomElement, writes the key connector data into it and appends it to the root object.
	void write(QDomDocument& doc, QDomElement& root) const;

    int               	m_keyUnicode;			///< This is the unicode number for the character that has to be pressed to access this key.
    int					m_targetKeyIndex;		///< Index of the target key (-1 if none).
	int					m_modifierKeyIndex;		///< Index of the modifier key (-1 if none).
};

#endif  // KTOUCHKEYCONNECTOR_H
