/***************************************************************************
 *   ktouchkey.h                                                           *
 *   -----------                                                           *
 *   Copyright (C) 2000 by Håvard Frøiland, 2004 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHKEY_H
#define KTOUCHKEY_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qpainter.h>
#include <qdom.h>

/// This class represents a key on the keyboard.
/// The primary character is the identification character for the key and will
/// be printed top left of the key (like normal character keys). If a secondary 
/// character is given (as for the keys containing numbers), the primary key will
/// be printed bottom left and the secondary key will be printed top left.
/// If m_secondaryChar is 0, no secondary character is given.
class KTouchKey {
  public:
    enum keytype_t {
	  NORMAL,
	  FINGER,
	  ENTER,
	  BACKSPACE,
	  SHIFT,
	  SPACE,
	  OTHER
	};
 
	/// Default constructor
	KTouchKey() : m_type(NORMAL), m_primaryChar(0), m_secondaryChar(0), m_x(0), m_y(0), m_w(0), m_h(0) {}
	/// Initialisation Constructor
	KTouchKey(keytype_t type, const QChar& primaryChar, const QChar& secondaryChar,
		int x, int y, int w, int h)
		: m_type(type), m_primaryChar(primaryChar), m_secondaryChar(secondaryChar), 
		  m_x(x), m_y(y), m_w(w), m_h(h) 
	{};
	
	/// Reads the key data from the DomElement
	bool read(QDomNode node);
	/// Creates a new DomElement, writes the key data into it and appends it to the root object.
	void write(QDomDocument& doc, QDomElement& root) const;
	
	keytype_t	m_type;
	QChar		m_primaryChar;		///< The primary character printed on the key.
	QChar		m_secondaryChar;	///< The (optional) secondary character printed on the key.
	QString		m_otherKeyText;		///< The text for 'decorative' keys.
	int			m_x;
	int			m_y;
	int			m_w;
	int			m_h;
};
// ---------------------------------------------------------------------------------------

#endif  // KTOUCHKEYS_H
