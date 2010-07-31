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
 
#include <tqtextstream.h>
#include <tqstringlist.h>
#include <tqmap.h>

#include <kdebug.h>

#include "ktouchutils.h"

TQString format_speed(double speed) {
    unsigned int mins = static_cast<unsigned int>(speed);
    unsigned int secs = static_cast<unsigned int>(speed*60) - mins*60;
    return TQString("%1' %2''").arg(mins).arg(secs);
}

void sort_lists(TQStringList& text, TQStringList& data) {
    if (text.count() != data.count()) return; // invalid input, no sorting possible
    TQMap<TQString, TQString> sort_map;
    TQStringList::const_iterator key_it = text.constBegin();
    TQStringList::const_iterator val_it = data.constBegin();
    while (key_it != text.constEnd()) {
//        kdDebug() << *key_it << "  " << *val_it << endl;
        sort_map[*key_it++] = *val_it++;
    }
    text.sort();
    key_it = text.constBegin();
    data.clear();
    while (key_it != text.constEnd()) {
//        kdDebug() << *key_it << "  " << sort_map[*key_it] << endl;
        data.append( sort_map[*key_it++] );
    }
}
