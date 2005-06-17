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

#include <qcolor.h>

/// Contains all colours of the colour scheme.
///
/// This is the colour scheme used for the slide line widget and the keyboard widget.  
class KTouchColorScheme {
  public:
    /// Default constructor.
    KTouchColorScheme() { createDefault(); };
    /// Creates the default colour scheme.
    void createDefault();

    QColor m_teacherTextColor;    ///< The text colour for the teachers line.
    QColor m_teacherBackground;   ///< The background colour for the teachers line.
    QColor m_studentTextColor;    ///< The text colour for the students line.
    QColor m_studentBackground;   ///< The background colour for the students line.
    QColor m_errorTextColor;      ///< The text colour for wrong text (students line).
    QColor m_errorBackground;     ///< The background colour for wrong text (students line).

    QString     m_name;           ///< The name of the colour scheme.
    QColor      m_frame;          ///< The colour for the key frames.
    QColor      m_background[8];  ///< The background colour of the finger keys and the associated normal keys.
    QColor      m_text;           ///< The colour for the text on the keys.
    QColor      m_backgroundH;    ///< The background colour for highlighted (next) normal keys.
    QColor      m_textH;          ///< The text/pen colour for highlighted (next) normal keys.
    QColor      m_cBackground;    ///< The background colour for the control keys.
    QColor      m_cText;          ///< The text/pen colour for the control keys.
    QColor      m_cBackgroundH;   ///< The background colour for activated control keys.
    QColor      m_cTextH;         ///< The text/pen colour for activated control keys.
};

#endif  // KTOUCHCOLORSCHEME_H

