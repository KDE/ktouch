/***************************************************************************
 *   ktouchutils.h                                                         *
 *   -------------                                                         *
 *   Copyright (C) 2000 by Håvard Frøiland, 2004 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHUTILS_H
#define KTOUCHUTILS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qstring.h>

/// Formats the speed (given in chars/min) and creates a nice looking text.
QString format_speed(double speed);

/// Sorts both lists but uses only the values in "text" for sorting.
void sort_lists(QStringList& text, QStringList& data);

#endif  // KTOUCHUTILS_H
