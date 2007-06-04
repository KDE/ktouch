/***************************************************************************
 *   ktouchkey.h                                                           *
 *   -----------                                                           *
 *   Copyright (C) 2000-2007 by Håvard Frøiland and Andreas Nicolai        *
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
#include <QObject>
#include <QGraphicsItem>

class QDomElement;
class QDomDocument;

class QGraphicsSceneMouseEvent;

/// This class represents a key on the keyboard.
///
/// This class contains all the geometrical information for a key to be drawn
/// on the keyboard. A KTouchKey is a QGraphicsItem and draws itself on
/// the graphics scene containing the keyboard.
///
/// Each key has an associated finger key, which is also used to determine the
/// color of the key. The index of the finger key is stored in the keyboard file.
/// After reading all keys, the KTouchKeyboard class will set the finger key
/// pointers for faster access in all keys. When writing the keys to the 
/// keyboard file, the list of keys is passed and the index is obtained.
class KTouchKey : public QObject, public QGraphicsItem {
  Q_OBJECT
public:

	/// Position of a character on the key.
	enum position_t {
      TopLeft,
      TopRight,
      BottomLeft,
      BottomRight
    };

	/// Type of a key
    enum keytype_t {
	  Normal,
	  Finger,
	  Enter,
	  Backspace,
	  Shift,
	  Space,
	  CapsLock,
	  Tab,
	  Other
	};
 
	/// The (highlighting, marking) state of a key
	enum state_t {
		NormalState,			// normal key appearance on the keyboard
		HighlightedState,		// when indicating the next target key
		FingerKeyState,			// when indicating the origin key of the finger to be used
		ModifierState			// when indicating a certain modifier key
	};

	/// Default constructor.
	KTouchKey(QObject * parent);
	/// Convenience constructor for a key with a single character.
	KTouchKey(QObject * parent, keytype_t type, int x, int y, int w, int h, QChar ch);
	/// Convenience constructor for a key with a text on it (type will be OTHER).
	KTouchKey(QObject * parent, int x, int y, int w, int h, const QString &text);
	/// Default destructor.
	virtual ~KTouchKey() {}

	/// Reads the key data from the DomElement
	bool read(const QDomElement &e);
	/// Creates a new DomElement, writes the key data into it and appends it to the root object.
	void write(QDomDocument& doc, QDomElement& root, const QList<KTouchKey*>& keys) const;

	/// Changes the state of the key.
	///
	/// This function will be called from the keyboard widget whenever the key appearance should change.
	void setState(state_t state);

	// *** re-implementations of QGraphicsItem functions ***

	/// Returns the bounding rectangle of this key.
	virtual QRectF boundingRect() const;
	/// Draws the key onto the graphics scene using the provided painter.
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	// *** member variables ***

	state_t			m_state;		///< The current state of the key (normal, highlighted etc.)
	unsigned int	m_colorIndex;	///< The color index for a finger key.

	keytype_t		m_type;			///< The type of the key.
	QChar			m_keyChar[4];	///< The characters on each key, one for each possible position.
	QString			m_keyText;		///< The alternative text for decorative keys.
	int				m_x;			///< The x-coordinate of the top-left corner of the key.
	int				m_y;			///< The y-coordinate of the top-left corner of the key.
	int				m_w;			///< The width.
	int				m_h;			///< The height.

	int				m_fingerKeyIndex;	///< The index for a finger key (only used for reading).
	KTouchKey		*m_fingerKey;		///< The finger key.

	static QString keyTypeString(keytype_t t);
	static keytype_t keyType(const QString& str);

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

signals:
	/// Emitted when the user left-clicks a key.
	void clicked(KTouchKey * me);
	/// Emitted when the user moved the graphics view item around.
	void positionChanged(KTouchKey * me);

private:
	double m_dragXOffset;
	double m_dragYOffset;
};
// ---------------------------------------------------------------------------------------

#endif  // KTOUCHKEYS_H
