/***************************************************************************
 *   ktouchutils.cpp                                                       *
 *   ---------------                                                       *
 *   Copyright (C) 2000 by Håvard Frøiland, 2004 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
 
#include <qtextstream.h>
 
#include "ktouchutils.h"

QString format_speed(double speed) {
    unsigned int mins = static_cast<unsigned int>(speed);
    unsigned int secs = static_cast<unsigned int>(speed*60) - mins*60;
    return QString("%1' %2''").arg(mins).arg(secs);
};
