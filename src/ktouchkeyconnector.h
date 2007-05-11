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

#include <QList>

class QDomDocument;
class QDomNode;
class QDomElement;

class KTouchKey;

/// @brief The KTouchKeyConnector class contains the information about the character that has to 
///        be pressed and the keys on the keyboard that should be highlighted.
///
/// It associates a character with a target key and optionally a control/modifier key. 
/// For reading/writing the pointers to the target and modifier key are converted into 
/// the index of the corresponding key in the key list (which is therefore passed as 
/// an extra argument to the read() and write() member functions).
class KTouchKeyConnector {
  public:
    /// Default constructor
    KTouchKeyConnector() : m_keyUnicode(-1), m_targetKey(NULL), m_modifierKey(NULL)
	{}
    /// Constructor provided for convenience.
    KTouchKeyConnector(int keyUnicode, KTouchKey * target_key, KTouchKey * modifier_key)
	  : m_keyUnicode(keyUnicode), m_targetKey(target_key), m_modifierKey(modifier_key)
	{}

	/// Reads the key connector data from the DomElement
	bool read(const QDomElement &e, const QList<KTouchKey*>& keylist);
	/// Creates a new DomElement, writes the key connector data into it and appends it to the root object.
	void write(QDomDocument& doc, QDomElement& root, const QList<KTouchKey*>& keylist) const;

    int               	m_keyUnicode;		///< This is the unicode number for the character that has to be pressed to access this key.
    KTouchKey			*m_targetKey;		///< Target key pointer (NULL if none).
	KTouchKey			*m_modifierKey;		///< Modifier key pointer (NULL if none).
};

#endif  // KTOUCHKEYCONNECTOR_H
