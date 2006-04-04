/***************************************************************************
 *   ktouchleveldata.cpp                                                   *
 *   -----------------                                                     *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchleveldata.h"
#include <qiodevice.h>
#include <qdom.h>
#include <klocale.h>
#include <kdebug.h>
#include <qstringlist.h>

const QString& KTouchLevelData::line(unsigned int lineNumber) const {
    if (lineNumber>=m_lines.size())
        lineNumber=0;
    return m_lines[lineNumber];
}

void KTouchLevelData::createDefault() {
    m_newChars = i18n("Quite a lot");
    m_lines.clear();
    QString text = i18n("This is a small default text. If you want\n"
                        "to start practicing touch typing, open\n"
                        "a lecture/training file from the main menu.\n"
                        "If you press enter after the next line you have\n"
                        "successfully completed the default lecture. Hurray!!!");
    QStringList l( QStringList::split(QChar('\n'), text) );
    for (QStringList::const_iterator it = l.constBegin(); it != l.constEnd(); ++it)
        m_lines.push_back( *it );
}

bool KTouchLevelData::readLevel(QTextStream& in) {
    QString line;
    m_comment = m_newChars = QString::null;
    m_lines.clear();
    // now read all the lines of the level, until EOF or an empty line
    line = in.readLine();
    while (!line.isNull()) {
        if (!line.isEmpty()) {
            // Do we have a comment here? And if yes, is a keyword in the line?
            if (line.find("# Comment:")==0)
                m_comment = line.right(line.length() - 10).stripWhiteSpace();
            else if (line[0]!='#') {
                // the first line is the new characters line
                if (m_newChars.isNull())  m_newChars = line;
                else                              m_lines.push_back(line);
            }
        }
        line = in.readLine();
    };
    if (m_lines.empty()) {
        createDefault();
        return false;
    }
    return true;
}

bool KTouchLevelData::readLevel(QDomNode in) {
	QDomNode newChars = in.namedItem("NewCharacters");
	if (newChars.isNull())  m_newChars = i18n("basically all characters on the keyboard","abcdefghijklmnopqrstuvwxyz");
	else					m_newChars = newChars.firstChild().nodeValue();
	QDomNode levelComment = in.namedItem("LevelComment");
	if (!levelComment.isNull())  m_comment = levelComment.firstChild().nodeValue();
    m_lines.clear();
	QDomNode line = in.namedItem("Line");
	while ( !line.isNull() ) {
        QString l = line.firstChild().nodeValue();
		m_lines.push_back( l.stripWhiteSpace() );
        line = line.nextSibling();
    }
    if (m_lines.empty()) {
        createDefault();
        return false;
    }
    return true;
}

void KTouchLevelData::writeLevel(QTextStream& out) const {
    out << "# Comment: " << m_comment << endl;   // comment line
    out << m_newChars << endl;                   // new characters line
    for (QValueVector<QString>::const_iterator it=m_lines.begin(); it!=m_lines.end(); ++it)
        out << *it << endl;
    out << endl;
}

void KTouchLevelData::writeLevel(QDomDocument& doc, QDomElement& root) const {
	QDomElement level = doc.createElement("Level");
	root.appendChild(level);
	// comment (optional)
	if (!m_comment.isEmpty()) {
		QDomElement comment = doc.createElement("LevelComment");
		QDomText commentText = doc.createTextNode(m_comment);
		comment.appendChild(commentText);
		level.appendChild(comment);
	}
	// new characters (must have that)
	QDomElement newchars = doc.createElement("NewCharacters");
	QDomText newcharsText;
	if (m_newChars.isEmpty())
		newcharsText = doc.createTextNode(i18n("basically all characters on the keyboard","abcdefghijklmnopqrstuvwxyz"));
	else
		newcharsText = doc.createTextNode(m_newChars);
	newchars.appendChild(newcharsText);
	level.appendChild(newchars);
	// the lines
    for (QValueVector<QString>::const_iterator it=m_lines.begin(); it!=m_lines.end(); ++it) {
		QDomElement line = doc.createElement("Line");
		QDomText lineText = doc.createTextNode(*it);
		line.appendChild(lineText);
		level.appendChild(line);
	}
}

