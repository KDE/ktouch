/***************************************************************************
 *   ktouchkeyboardwidget.h                                                *
 *   ----------------------                                                *
 *   Copyright (C) 2000 by Håvard Frøiland, 2004 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHKEYBOARDWIDGET_H
#define KTOUCHKEYBOARDWIDGET_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qwidget.h>
#include <qvaluevector.h>
#include <qmap.h>
#include "ktouchkeys.h"

#include "ktouchkey.h"  // new class
#include "ktouchkeyconnector.h" // new class

class KURL;

/** This is the keyboard widget at the bottom of the training screen.
 *  In addition to the painting functions this class has the member
 *  functions loadKeyboard() and saveKeyboard() which can read and write
 *  a keyboard layout from/into a file.<p>
 *  The keyboard is drawn in the paintEvent() member function. The
 *  resizing, that means the recalculation of m_shift is
 *  done in the resizeEvent() member function.<p>
 *  The state of keyboard and keys does not change while it is
 *  repainted or resized. Only when a new character has been typed
 *  and the newKey() slot is called the state changes (and thus the
 *  activated keys and finger key animations etc.)
 */
class KTouchKeyboardWidget : public QWidget {
    Q_OBJECT
  public:
    /// Constructor
    KTouchKeyboardWidget(QWidget *parent);
    /// Reads a keyboard layout from the given URL.
    /// The function returns 'true' when the reading was successful or 'false' if not. In this
    /// case the optional parameter errorMsg contains the error message.
    bool loadKeyboard(QWidget * window, const KURL& url, QString* errorMsg=NULL);
    /// Saves the keyboard layout to the URL.
    void saveKeyboard(QWidget * window, const KURL& url);
    /// Applies preferences to the keyboard layout and the keys.
    /// This means that the layout is basically recreated and if the layout type/language
    /// changed it will be reloaded.
    void applyPreferences(QWidget * window, bool silent);

  public slots:
    /// This function displays the next key (or key combination) the user has to press.
    void newKey(const QChar& nextChar);

  protected:
    /// Draws the keyboard.
    void paintEvent(QPaintEvent *);
    /// Resizes (recalculates m_shift and m_scale) and redraws the keyboard.
    void resizeEvent(QResizeEvent *);

  private:
    /// Does what the name says (create a default keyboard which is a number keypad).
    /// This function is needed in case there no keyboard file could be found.
    void createDefaultKeyboard();
    /// Does the actual reading.
    bool readKeyboard(const QString& fileName, QString& errorMsg);
    /// Assigns the background colours to the normal keys, which have a finger key assigned.
    void updateColours();

	// *** new data storage classes ***
    QValueVector<KTouchKey>          	m_keys;        		///< The geometrical key data.
    QValueVector<KTouchKeyConnector>	m_keyConnections;	///< Contains the character - key associations.
	QMap<unsigned int, int> 			m_keyMap;			///< Links target keys with finger keys: m_keyMap[target_key] = finger_key
	QMap<unsigned int, unsigned int>	m_colorMap;			///< Links finger keys with color indices: m_colorMap[finger_key] = color_index in color scheme

	// *** old data storage classes ***
    QPtrList<KTouchBaseKey>         	m_keyList;     		///< The pointer list with base class pointers to the keys.
    QValueList<KTouchKeyConnection> 	m_connectorList;	///< Contains the character - key associations.

	static QMap<QChar, int>	 			m_keyCharMap;		///< Links target keys with finger keys: m_keyCharMap[target_key_char] = color_index

    int                 m_keyboardWidth;    ///< The width of the keyboard (maximum of the sums of all keywidths in each line).
    int                 m_keyboardHeight;   ///< The height of the keyboard (sum of all key row heights).
    double              m_shift;            ///< The horizontal shift for the keyboard.
    QString             m_currentLayout;    ///< The name of the currently used layout.
    QChar               m_nextKey;          ///< The next to be pressed character.

	bool				m_hideKeyboard;		///< If true, the keyboard won't be shown.
};

#endif  // KTOUCHKEYBOARDWIDGET_H
