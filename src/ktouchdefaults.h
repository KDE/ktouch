/***************************************************************************
 *   ktouchdefaults.h                                                      *
 *   -------------------                                                   *
 *   Copyright (C) 2004 by Andreas Nicolai                                 *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHDEFAULTS_H
#define KTOUCHDEFAULTS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

extern const unsigned int BEGINNER_SPEED;		///< Speed for rating in characters per minute.
extern const unsigned int ADVANCED_SPEED;		///< Speed for rating in characters per minute.
extern const unsigned int PROFESSIONAL_SPEED;	///< Speed for rating in characters per minute.

extern const int KEYBOARD_MARGIN;				///< The margin around the keybaord layout.

extern const int LCD_UPDATE_INTERVAL;    		///< Update interval for LCD display and statistics.

#endif  // KTOUCHDEFAULTS_H
