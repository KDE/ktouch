/***************************************************************************
 *   ktouchcolorscheme.cpp                                                 *
 *   ---------------------                                                 *
 *   Copyright (C) 2003 by Andreas Nicolai                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchcolorscheme.h"

void KTouchColorScheme::createDefault() {
    m_teacherTextColor = Qt::black;
    m_teacherBackground = Qt::cyan;
    m_studentTextColor = Qt::black;
    m_studentBackground = Qt::green;
    m_errorTextColor = Qt::white;
    m_errorBackground = Qt::red;
}


// TODO : Read and write routines
