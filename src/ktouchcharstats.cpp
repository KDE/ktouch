/***************************************************************************
 *   ktouchcharstats.cpp                                                   *
 *   -------------------                                                   *
 *   Copyright (C) 2003 by Andreas Nicolai                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchcharstats.h"
#include <qtextstream.h>

int KTouchCharStats::hitMissRatio() const {
    if (m_correctCount+m_wrongCount==0) return 0;
    return static_cast<int>(100.0*m_wrongCount/(m_correctCount+m_wrongCount));
};

QTextStream& operator<<(QTextStream &out, const KTouchCharStats &ch) {
    return (out << ch.m_char.unicode() << " " << ch.m_correctCount << " " << ch.m_wrongCount);
};

bool greaterHitMissRatio(const KTouchCharStats &lhs, const KTouchCharStats &rhs) {
    return (lhs.hitMissRatio() > rhs.hitMissRatio());
};
