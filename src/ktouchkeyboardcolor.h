/***************************************************************************
 *   ktouchkeyboardcolor.h                                                 *
 *   ---------------------                                                 *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHKEYBOARDCOLOR_H
#define KTOUCHKEYBOARDCOLOR_H


/// This structure contains the data for a keyboard colour scheme.
struct KTouchKeyboardColor {
    QString     m_name;                 ///< The name of the colour scheme.
    QColor      m_frame;                ///< The colour for the key frames.
    QColor      m_background[8];        ///< The background colour of the finger keys and the associated normal keys.
    QColor      m_text;                 ///< The colour for the text on the keys.
    QColor      m_backgroundH;          ///< The background colour for highlighted (next) normal keys.
    QColor      m_textH;                ///< The text/pen colour for highlighted (next) normal keys.
    QColor      m_cBackground;          ///< The background colour for the control keys.
    QColor      m_cText;                ///< The text/pen colour for the control keys.
    QColor      m_cBackgroundH;         ///< The background colour for activated control keys.
    QColor      m_cTextH;               ///< The text/pen colour for activated control keys.
};

#endif  // KTOUCHKEYBOARDCOLOR_H
