/***************************************************************************
 *   ktouchlecture.h                                                       *
 *   ---------------                                                       *
 *   Copyright (C) 2003 by Håvard Frøiland                                 *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHMACROS_H
#define KTOUCHMACROS_H

template <typename T>
T min(T x, T y) {
    return (x < y) ? x : y;
}

template <typename T>
T max(T x, T y) {
    return (x > y) ? x : y;
}

#endif

