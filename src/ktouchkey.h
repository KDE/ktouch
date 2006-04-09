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

/// This class contains information about one character on a key.
class KTouchKeyChar {
  public:
	/// Position of the character on the key.
	enum position_t {
      TOP_LEFT,
      TOP_RIGHT,
      BOTTOM_LEFT,
      BOTTOM_RIGHT
    };

	/// Constructor.
	KTouchKeyChar() {}
	/// Constructor.
	KTouchKeyChar(QChar ch, position_t p, bool bold = false) :
		m_ch(ch), m_pos(p), m_bold(bold) {}

    QChar 		m_ch;		///< The character to draw.
	position_t 	m_pos;		///< The position of the character.
    bool  		m_bold;		///< Whether this is a bold character.

	QString		m_text;		///< The text to draw of m_ch == 0.
};

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
	KTouchKey() : m_type(NORMAL), m_x(0), m_y(0), m_w(0), m_h(0) {}
	/// Convenience constructor for a key with a single character (like before).
	KTouchKey(keytype_t type, int x, int y, int w, int h, QChar ch);
	/// Convenience constructor for a key with a text on it (type will be OTHER).
	KTouchKey(int x, int y, int w, int h, QString text);

	/// Resizes the key (this function will be obsolete soon)
	void resize(double scale);

	/// Reads the key data from the DomElement
	bool read(QDomNode node);
	/// Creates a new DomElement, writes the key data into it and appends it to the root object.
	void write(QDomDocument& doc, QDomElement& root) const;

	unsigned int	m_number;		///< The number of the key.
	keytype_t		m_type;			///< The type of the key.
	KTouchKeyChar	m_chars[4];		///< The key character information.
	int				m_x;			///< The x-coordinate of the top-left corner of the key.
	int				m_y;			///< The y-coordinate of the top-left corner of the key.
	int				m_w;			///< The width.
	int				m_h;			///< The height.

	int				m_xS;			///< The scaled x-coordinate of the top-left corner of the key.
	int				m_yS;			///< The scaled y-coordinate of the top-left corner of the key.
	int				m_wS;			///< The scaled width.
	int				m_hS;			///< The scaled height.

};
// ---------------------------------------------------------------------------------------

#endif  // KTOUCHKEYS_H
