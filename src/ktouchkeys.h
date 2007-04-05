/***************************************************************************
 *   ktouchkeys.h                                                          *
 *   ------------                                                          *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHKEYS_H
#define KTOUCHKEYS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qpainter.h>

class KTouchFingerKey;
class KTouchNormalKey;
class KTouchControlKey;

/** This is the base class for all keys (KTouchFingerKey, KTouchNormalKey and KTouchControlKey).
 *  Do not create instances of KTouchKey itself or you will get only blank keys. Note that the
 *  KTouchKey class hierarchy contains only the information for painting the keys. The connectivity
 *  is handled using the KTouchKeyConnector.
 */
class KTouchBaseKey {
  public:
    /// The possible types of the keys
    enum KeyType { FINGER_KEY, NORMAL_KEY, CONTROL_KEY };

    /// Constructor
    KTouchBaseKey(const QChar& keyChar, const QString& keyText, int x, int y, int w, int h)
      : m_keyChar(keyChar), m_keyText(keyText), m_isActive(false), m_isNextKey(false),
        m_x(x), m_y(y), m_w(w), m_h(h), m_type(NORMAL_KEY) {}
    /// Destructor.
    virtual ~KTouchBaseKey() {}
    /// Paints the basic key shape using the painter p.
    virtual void paint(QPainter& p) const;
    /// Recalculates the scaled position and size properties of the key.
    void resize(double scale);
    /// Returns the boundary frame of the key.
    QRect frame()  const { return QRect(m_x, m_y, m_w, m_h); }
    /// Returns the key type.
    KeyType type() const { return m_type; }

    QChar   m_keyChar;      ///< The character that needs to be pressed to access this char.
    QString m_keyText;      ///< The text on the key (may be a single char only).
    bool    m_isActive;     ///< Indicates whether the key is active (finger and control keys).
    bool    m_isNextKey;    ///< Indicates whether this is the next to be pressed key (normal and finger keys).
	QFont   m_font;
	float   m_font_scale;
    
  protected:
    int     m_x;        ///< The x position of the key.
    int     m_y;        ///< The y position of the key.
    int     m_w;        ///< The width of the key.
    int     m_h;        ///< The height of the key.

    int     m_xS;       ///< The scaled x position of the key.
    int     m_yS;       ///< The scaled y position of the key.
    int     m_wS;       ///< The scaled width of the key.
    int     m_hS;       ///< The scaled height of the key.

    KeyType m_type;     ///< Stores the type of the key (convenience for saving of the keyboard layout).
};
// ---------------------------------------------------------------------------------------



/** This is a normal keyboard key with text.
 *  The background colour of the key will be taken from the current colour scheme using the
 *  colour index (this should be the same colour as used for the corresponding finger key).
 */
class KTouchNormalKey : public KTouchBaseKey {
  public:
    /// Constructor
    KTouchNormalKey(const QChar& keyChar, const QString& keyText, int x, int y, int w, int h);
    /// Destructor
    virtual ~KTouchNormalKey() {}
    /// Extends the painting routine of KTouchKey (adds the text).
    void paint(QPainter& p) const;
    /// Indicates the colour index in the colour scheme, that has to be used for this key
    /// and will be set in KTouchKeyboard::updateColors().
    unsigned int m_colorIndex;
};
// ------------------------------------------------------------------------------------



/** This is a key where a finger rests while not typing.
 *  The name is taken from the fact, that there is normally for each finger (not including the
 *  thumbs) a key on a keyboard where the finger rests while it is not "used". A finger key
 *  is basically a normal key with additional stuff in the painting routine to mark it as a
 *  finger key.
 */
class KTouchFingerKey : public KTouchNormalKey {
  public:
    /// Constructor
    KTouchFingerKey(const QChar& keyChar, const QString& keyText, int x, int y, int w, int h);
    /// Destructor
    ~KTouchFingerKey() { --m_fingerKeyCount; }
    /// Extends the painting algoritm of KTouchNormalKey when marked.
    void paint(QPainter& p) const;

  private:
    static int m_fingerKeyCount;    ///< Contains the number of created finger keys (for colour generation).
};
// ------------------------------------------------------------------------------------



/** This is a special or control key.
 *  This key acts as a modifier key to a normal key (for instance a shift key) and has a
 *  different shape and painting routine then the normal keys. Therefore it is directly
 *  derived from KTouchKey.
 */
class KTouchControlKey : public KTouchBaseKey {
  public:
    /// Constructor
    KTouchControlKey(const QChar& keyChar, const QString& keyText, int x, int y, int w, int h);
    /// Extends the parents paint routine (draws the text or other fancy stuff).
    void paint(QPainter& p) const;
};
// ------------------------------------------------------------------------------------


/// This is the key connector which represents a key or key combination that can be pressed.
 /// It associates a character with a target key and optionally a finger and/or control key.
class KTouchKeyConnection {
  public:
    /// Default constructor
    KTouchKeyConnection() : m_keyChar(0), m_targetKeyChar(0), m_fingerKeyChar(0), m_controlKeyChar(0) {}
    /// Constructor provided for convenience.
    KTouchKeyConnection(const QChar& keyChar, const QChar& targetKeyChar, const QChar& fingerKeyChar,
                       const QChar& controlKeyChar)
      : m_keyChar(keyChar), m_targetKeyChar(targetKeyChar), m_fingerKeyChar(fingerKeyChar), m_controlKeyChar(controlKeyChar) {}

    /// This is the character that has to be pressed to access this key.
    QChar               m_keyChar;

    QChar               m_targetKeyChar;    ///< The target character (the key the user has to press).
    QChar               m_fingerKeyChar;    ///< The finger key (where the used finger normally rests).
    QChar               m_controlKeyChar;   ///< The modifier key the user has to press also (for example LeftShift).
};

#endif  // KTOUCHKEYS_H
