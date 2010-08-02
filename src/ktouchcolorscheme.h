/***************************************************************************
 *   ktouchcolorscheme.h                                                   *
 *   -------------------                                                   *
 *   Copyright (C) 2003 by Andreas Nicolai                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHCOLORSCHEME_H
#define KTOUCHCOLORSCHEME_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tqcolor.h>
#include <tqvaluevector.h>

#include <kurl.h>

class TQDomDocument;
class TQDomElement;
class TQDomNode;

/// Contains all colours of the colour scheme.
///
/// This is the colour scheme used for the slide line widget and the keyboard widget.  
class KTouchColorScheme {
  public:
    /// Default constructor.
    KTouchColorScheme() { clear(); }
    /// Creates the default colour scheme.
    void clear();

    /// Loads a color map (in XML format) from XML node (returns true if successful)
    bool read(const TQDomNode& node);
    /// Saves a color map to XML document (returns true if successful).
    void write(TQDomDocument& doc, TQDomElement& root) const;


    TQString m_name;          		///< The name of the colour scheme.

    TQColor 	m_teacherTextColor;    	///< The text colour for the teachers line.
    TQColor 	m_teacherBackground;   	///< The background colour for the teachers line.
    TQColor 	m_studentTextColor;    	///< The text colour for the students line.
    TQColor 	m_studentBackground;   	///< The background colour for the students line.
    TQColor 	m_errorTextColor;      	///< The text colour for wrong text (students line).
    TQColor 	m_errorBackground;     	///< The background colour for wrong text (students line).

    TQColor  m_frame;          		///< The colour for the key frames.
    TQColor  m_background[8];  		///< The background colour of the finger keys and the associated normal keys.
    TQColor  m_text;           		///< The colour for the text on the keys.
    TQColor  m_backgroundH;    		///< The background colour for highlighted (next) normal keys.
    TQColor  m_textH;          		///< The text/pen colour for highlighted (next) normal keys.
    TQColor  m_cBackground;    		///< The background colour for the control keys.
    TQColor  m_cText;          		///< The text/pen colour for the control keys.
    TQColor  m_cBackgroundH;   		///< The background colour for activated control keys.
    TQColor  m_cTextH;         		///< The text/pen colour for activated control keys.

	bool	m_default;		  		///< If true, this color map is marked as default and will not
							  		///  be saved in the XML file.

	// *** static member functions ***

    /// Loads several color maps (in XML format) from file (returns true if successful)
	/// and _adds_ them to the current color maps. This function shall be called
	/// directly after createDefaultColorSchemes().
    static bool readList(TQWidget * window, const KURL& url);
    /// Saves several color maps to file (returns true if successful).
	/// Only non-default color maps are written to file.
    static bool writeList(TQWidget * window, const KURL& url);
    /// Creates some default color schemes (erases m_colorSchemes before).
    static void createDefaults();
	/// Contains all color schemes available in the program.
	static TQValueVector<KTouchColorScheme>	m_colorSchemes;
};

#endif  // KTOUCHCOLORSCHEME_H

