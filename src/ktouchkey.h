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

#include <QPainter>

class QDomElement;
class QDomNode;
class QDomDocument;

/// This class represents a key on the keyboard.
class KTouchKey {
  public:
	/// Position of a character on the key.
	enum position_t {
      TOP_LEFT,
      TOP_RIGHT,
      BOTTOM_LEFT,
      BOTTOM_RIGHT
    };

	/// Type of a key
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
	KTouchKey() : m_type(NORMAL), m_x(0), m_y(0), m_w(0), m_h(0) {}
	/// Convenience constructor for a key with a single character.
	KTouchKey(keytype_t type, int x, int y, int w, int h, QChar ch);
	/// Convenience constructor for a key with a text on it (type will be OTHER).
	KTouchKey(int x, int y, int w, int h, const QString &text);

	/// Reads the key data from the DomElement
	bool read(QDomNode node);
	/// Creates a new DomElement, writes the key data into it and appends it to the root object.
	void write(QDomDocument& doc, QDomElement& root) const;

	unsigned int	m_number;		///< The number of the key.
	keytype_t		m_type;			///< The type of the key.
	QChar			m_keyChar[4];	///< The characters on each key, one for each possible position.
	QString			m_keyText;		///< The alternative text for decorative keys.
	int				m_x;			///< The x-coordinate of the top-left corner of the key.
	int				m_y;			///< The y-coordinate of the top-left corner of the key.
	int				m_w;			///< The width.
	int				m_h;			///< The height.
};
// ---------------------------------------------------------------------------------------

#endif  // KTOUCHKEYS_H
