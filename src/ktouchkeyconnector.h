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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qdom.h>
#include <qvaluevector.h>
#include "ktouchkey.h"

 /// The KTouchKeyConnector class contains the information about the character that has to 
 /// be pressed and the keys on the keyboard that should be highlighted.
 /// It associates a character with a target key and optionally a finger and/or control key.
class KTouchKeyConnector {
  public:
    /// Default constructor
    KTouchKeyConnector() : m_keyChar(0), m_targetKeyChar(0), m_fingerKeyChar(0), 
	    m_controlKeyId(""), m_targetKeyIndex(-1), m_fingerKeyIndex(-1), m_controlKeyIndex(-1)
	 {};
    /// Constructor provided for convenience.
    KTouchKeyConnector(const QChar& keyChar, const QChar& targetKeyChar, const QChar& fingerKeyChar,
                       const QString& controlKeyId)
    	: m_keyChar(keyChar), m_targetKeyChar(targetKeyChar), m_fingerKeyChar(fingerKeyChar), 
	    m_controlKeyId(controlKeyId), m_targetKeyIndex(-1), m_fingerKeyIndex(-1), m_controlKeyIndex(-1)
	{};

	/// Reads the key connector data from the DomElement
	bool read(QDomNode node);
	/// Creates a new DomElement, writes the key connector data into it and appends it to the root object.
	void write(QDomDocument& doc, QDomElement& root) const;
	/// Updates the indices for the keys
	void updateConnections(const QValueVector<KTouchKey>& keyvector);
	
    QChar               m_keyChar;			///< This is the character that has to be pressed to access this key.
    QChar               m_targetKeyChar;	///< The target key (the key that will be highlighted).
    QChar               m_fingerKeyChar;	///< The finger key (where the used finger normally rests).
    QString             m_controlKeyId;		///< The id string for the associated control key that should be highlighted.

	// these variables are set after the full keyboard layout was read
    int					m_targetKeyIndex;	///< Index of the target key (-1 if none).
    int					m_fingerKeyIndex;	///< Index of the finger key (-1 if none).
	int					m_controlKeyIndex;	///< Index of the control key (-1 if none).
};

#endif  // KTOUCHKEYCONNECTOR_H
