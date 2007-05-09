/***************************************************************************
 *   ktouchkeyboard.h                                                      *
 *   ----------------                                                      *
 *   Copyright (C) 2004 by Andreas Nicolai                                 *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHKEYBOARD_H
#define KTOUCHKEYBOARD_H

#include <QTextStream>
#include <QList>
#include <QMap>
#include <QObject>
#include <QFont>

#include "ktouchkeyconnector.h"
class KTouchKey;

class KUrl;

/// @brief This class stores the keyboard layout and the connectivity between 
///        characters and the actual keys.
///
/// For any given character the mapping m_connectors can be used to obtain
/// the indices for the keys to highlight on the keyboard. This information
/// can be used by the keyboard widget to change the state of the appropriate
/// keys.
/// @code
/// QChar keyToPress = 'H';
/// KTouchKeyConnector & c = m_connectors[keyToPress.unicode()];
/// KTouchKey * targetKey = c.m_targetKey;
/// KTouchKey * modifierKey = c.m_modifierKey;
/// if (targetKey != NULL) {
/// 	KTouchKey * fingerKey = targetKey->m_fingerKey;
/// 	// ...
/// }
/// @endcode
class KTouchKeyboard : public QObject {
  Q_OBJECT
public:
	/// Default constructor, sets up the standard number keyboard.
	KTouchKeyboard(QObject * parent=0) : QObject(parent) { createDefault(); }
	/// Clears the keyboard (resets all data)
	void clear();
    /// Loads a keyboard layout (old format) from file (returns true if successful).
    bool load(QWidget * window, const KUrl& url);
    /// Loads a lecture (in XML format) from file (returns true if successful).
    bool loadXML(QWidget * window, const KUrl& url);
    /// Saves the lecture data to file (returns true if successful).
    bool saveXML(QWidget * window, const KUrl& url) const;
	/// Creates the default number keyboard.
	void createDefault();
	/// Updates the keys to finger key pointers and also updates the color indices in the finger keys.
	void updateKeyPointers();
	/// Sets the font in all keys of the keyboard.
	void setFont(const QFont& f);
	/// Returns the current keyboard font.
	const QFont& font() const { return m_font; }
	/// Returns whether key click events on keys are allowed or not.
	bool allowKeyClicks() const;
	/// Deletes a key and updates all pointers.
	void deleteKey(KTouchKey * k);

    QList<KTouchKey*>         			m_keys;      	///< List with key definitions.
    QMap<int, KTouchKeyConnector>		m_connectors;	///< Mapping with character connectivity data.
	
	QString		m_title;			///< Title of the keyboard (to appear in the menu).
	QString		m_comment;			///< Comments about the creator of the keyboard layout.
	QString		m_language;			///< Language ID of keyboard
	QString		m_fontSuggestions;	///< Suggestions of fonts to be used on the keys.

  private:
	/// Tests, whether a key with the given unicode number already exists in the keylist and
	/// appends a warning message if it does.
	bool keyAlreadyExists(int keyUnicode, QString type, QString& warnings);
    /// Loads keyboard data from file
    bool read(QTextStream& in, QString& warnings);
    /// Loads keyboard data from file into an XML document
    bool read(const QDomDocument& doc, QString& warnings);
    /// Saves keyboard data in the XML document
    void write(QDomDocument& doc) const;

    /// The font that is used to draw the keyboard.
    ///
    /// The keys access this member variable when they draw themselves.
	QFont		m_font;	
};

#endif  // KTOUCHKEYBOARD_H
