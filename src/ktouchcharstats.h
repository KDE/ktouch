/***************************************************************************
 *   ktouchcharstats.h                                                     *
 *   -----------------                                                     *
 *   Copyright (C) 2003 by Andreas Nicolai                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHCHARSTATS_H
#define KTOUCHCHARSTATS_H

#include <qstring.h>    // for QChar

class QTextStream;

/** Contains the stats for a character.
 *  Basically in this class the number of times the character has been pressed is counted. Both
 *  the number of times it was correctly pressed and the number of times it was missed are stored
 *  which allows to calculate a relative "hit-miss-ratio".
 */
class KTouchCharStats {
  public:
    /// Default constructor.
    KTouchCharStats() {};
    /// Constructor with parameters.
    KTouchCharStats(QChar ch, unsigned int correct, unsigned int wrong)
      : m_char(ch), m_correctCount(correct), m_wrongCount(wrong) {};
    /// Returns the hit-miss ratio (a value between 0-all correct and 100-all wrong).
    int hitMissRatio() const;

    QChar           m_char;         ///< The character that has been missed.
    unsigned int    m_correctCount; ///< How often the character has been typed correctly.
    unsigned int    m_wrongCount;   ///< How often the character has been missed (not typed when it ought to be typed).
};

/// Comparison operator < : returns 'true' when the char-code of 'lhs' is less then the one of 'rhs'
inline bool operator<(const KTouchCharStats &lhs, const KTouchCharStats &rhs) { return lhs.m_char<rhs.m_char; }
/// Comparison operator > : returns 'true' when the char-code of 'lhs' is greater then the one of 'rhs'
inline bool operator>(const KTouchCharStats &lhs, const KTouchCharStats &rhs) { return lhs.m_char>rhs.m_char; }
/// Comparison operator == : returns 'true' when the char-code of 'lhs' is equal to then the one of 'rhs'
inline bool operator==(const KTouchCharStats &lhs, const KTouchCharStats &rhs) { return lhs.m_char==rhs.m_char; }

/// Writes the content of a KTouchCharStats object into the text stream.
QTextStream& operator<<(QTextStream &out, const KTouchCharStats &ch);

/// Sort criterion: Returns 'true' when the hit-miss ratio of 'lhs' is worse then the one of 'rhs'.
bool greaterHitMissRatio(const KTouchCharStats &lhs, const KTouchCharStats &rhs);

#endif  // KTOUCHCHARSTATS_H
