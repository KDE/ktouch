/***************************************************************************
 *   ktouchleveldata.cpp                                                   *
 *   -------------------                                                   *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchleveldata.h"
#include <klocale.h>
#include <qiodevice.h>
#include <kdebug.h>

KTouchLevelData::KTouchLevelData() {
    createDefault();
};

KTouchLevelData::KTouchLevelData(const QString& comment, const QString& newChars) : m_comment(comment), m_newChars(newChars) {
    createDefault();
};

const QString& KTouchLevelData::line(unsigned int lineNumber) const {
    if (lineNumber>=m_lines.count())
        lineNumber=0;
    return m_lines[lineNumber];
};

void KTouchLevelData::createDefault() {
    m_comment = i18n("This is a default mini level!");
    m_newChars = i18n("only a few");
    m_lines.clear();
    m_lines.append( i18n("This is the first line in the mini level!") );
};

bool KTouchLevelData::readLevel(QTextStream& in, const QString& newChars, const QString& comment) {
    QString line;
    m_newChars = newChars;
    m_comment = comment;
    m_comment.remove(0,2);
    m_lines.clear();
    // now read all the lines of the level, until EOF or an empty line
    line = in.readLine();
    while (!line.isNull() && !line.isEmpty() && line[0]!='#') {
        m_lines.push_back(line);
        line = in.readLine();
    };
    if (m_lines.isEmpty()) {
        createDefault();
        return false;
    }
    return true;
}

void KTouchLevelData::writeLevel(QTextStream& out) const {
    out << "# " << m_comment << endl;   // comment line
    out << m_newChars << endl;          // new characters line
    for (QValueVector<QString>::const_iterator it=m_lines.begin(); it!=m_lines.end(); ++it)
        out << *it << endl;
    out << endl;
}
