/***************************************************************************
 *   ktouchtrainingsession.cpp                                             *
 *   -------------------------                                             *
 *   Copyright (C) 2003 by Andreas Nicolai                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "ktouchtrainingsession.h"
#include <qtextstream.h>
#include <iterator>
#include <algorithm>
using std::set;
using std::list;
using std::copy;
using std::back_inserter;

KTouchTrainingSession::KTouchTrainingSession(QString str) {
    // first let's extract the state variables
    QTextStream textstrm(str, IO_ReadOnly );
    textstrm >> m_elapsedTime >> m_totalChars >> m_correctChars >> m_words;
    // and now let's extract the wrong typed chars...
    int charcode, correctCount, wrongCount;
    textstrm >> charcode >> correctCount >> wrongCount;
    while (charcode!=0) {
        m_charStats.insert( KTouchCharStats( QChar(charcode), correctCount, wrongCount) );
        textstrm >> charcode >> correctCount >> wrongCount;
    };
};

void KTouchTrainingSession::reset() {
    m_totalChars=m_correctChars=m_words=m_elapsedTime=0;
    m_charStats.clear();
    m_wordBuffer="";
};

QString KTouchTrainingSession::asString() const {
    // We store the session info as follows: elapsed time, total chars, correct chars, words,
    QString tmp = QString("%1 %2 %3 %4")
        .arg(m_elapsedTime).arg(m_totalChars).arg(m_correctChars).arg(m_words);
    // and then we append for each missed char the char-unicode and the counters
    for (set<KTouchCharStats>::const_iterator it=m_charStats.begin(); it!=m_charStats.end(); ++it)
        tmp += QString(" %1 %2 %3").arg(it->m_char.unicode()).arg(it->m_correctCount).arg(it->m_wrongCount);
    return tmp;
};

void KTouchTrainingSession::addCorrectChar(QChar ch) {
    ++m_correctChars;
    ++m_totalChars;
    // we only count non-space characters
    if (ch!=QChar(' ')) {
        set<KTouchCharStats>::iterator it = m_charStats.find( KTouchCharStats(ch,0,0) );
        if (it==m_charStats.end())
            m_charStats.insert( KTouchCharStats(ch,1,0) );
        else
            ++(const_cast<KTouchCharStats&>(*it).m_correctCount);
        // NOTE: this const_cast is necessary because the current gcc 3.2 has a small bug in the
        //       implementation of set::find(). The non-const overload is missing and find() always
        //       returns a const object. Maybe in the next release this will be fixed. Until then
        //       the const_cast helps.
    };
    if (ch==QChar(' ') && !m_wordBuffer.isEmpty() ) {
        m_wordBuffer="";
        ++m_words;
    }
    else
        m_wordBuffer+=ch;
};

void KTouchTrainingSession::addWrongChar(QChar missedChar) {
    ++m_totalChars;
    if (missedChar==QChar(8) || missedChar==QChar(' ')) return; // ignore backspaces and spaces
    set<KTouchCharStats>::iterator it = m_charStats.find( KTouchCharStats(missedChar,0,0) );
    if (it==m_charStats.end())
        m_charStats.insert( KTouchCharStats(missedChar,0,1) );
    else
        ++(const_cast<KTouchCharStats&>(*it).m_wrongCount);
};

list<KTouchCharStats> KTouchTrainingSession::sortedCharStats() const {
    list<KTouchCharStats> tmpList;
    std::copy(m_charStats.begin(), m_charStats.end(), back_inserter(tmpList) );
    tmpList.sort(greaterHitMissRatio);
    return tmpList;
};
