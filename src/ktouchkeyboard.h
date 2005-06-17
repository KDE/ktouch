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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qvaluevector.h>

#include "ktouchkeyconnector.h"
#include "ktouchkey.h"

class KURL;

/// This class stores the keyboard layout and the connectivity between characters 
/// and the actual keys.
class KTouchKeyboard  {
  public:
	/// Default constructor, sets up the standard number keyboard.
	KTouchKeyboard() { createDefault(); }
	/// Clears the keyboard (resets all data)
	void clear();
    /// Loads a keyboard layout (old format) from file (returns true if successful).
    bool load(QWidget * window, const KURL& url);
    /// Loads a lecture (in XML format) from file (returns true if successful).
    bool loadXML(QWidget * window, const KURL& url);
    /// Saves the lecture data to file (returns true if successful).
    bool saveXML(QWidget * window, const KURL& url) const;
	/// Creates the default number keyboard.
	void createDefault();
	/// Updates the indices in the KTouchKeyConnector objects for faster access.
	void updateConnections();
	
    QValueVector<KTouchKey>         	m_keys;      	///< Vector with key definitions.
    QValueVector<KTouchKeyConnector>	m_connectors;	///< Vector with connectivity data.
	
	QString		m_title;			///< Title of the keyboard (to appear in the menu).
	QString		m_comment;			///< Comments about the creator of the keyboard layout.
	QString		m_language;			///< Language ID of keyboard
	QString		m_fontSuggestions;	///< Suggestions of fonts to be used on the keys.

	// These variables are recalculated after the keyboard was loaded.
	int			m_width;		///< The width of the keyboard (maximum of the sums of all keywidths in each line).
	int			m_height;		///< The height of the keyboard (sum of all key row heights).
	
  private:
    /// Loads keyboard data from file
    bool read(QTextStream& in);
    /// Loads keyboard data from file into an XML document
    bool read(const QDomDocument& doc);
    /// Saves keyboard data in the XML document
    void write(QDomDocument& doc) const;
};

#endif  // KTOUCHKEYBOARD_H
