/***************************************************************************
 *   ktouchkeyconnetor.cpp                                                 *
 *   ---------------------                                                 *
 *   Copyright (C) 2004 by Andreas Nicolai                                 *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchkeyconnector.h"

// Reads the key connector data from the DomElement
bool KTouchKeyConnector::read(QDomNode node) {
/*	QDomNode newChars = in.namedItem("NewCharacters");
	if (newChars.isNull())  m_newChars = i18n("basically all characters on the keyboard","abcdefghijklmnopqrstuvwxyz");
	else					m_newChars = newChars.firstChild().nodeValue();
	QDomNode levelComment = in.namedItem("LevelComment");
	if (!levelComment.isNull())  m_comment = levelComment.firstChild().nodeValue();
    m_lines.clear();
	QDomNode line = in.namedItem("Line");
	while ( !line.isNull() ) {
		m_lines.push_back( line.firstChild().nodeValue() );
        line = line.nextSibling();
    }
    if (m_lines.empty()) {
        createDefault();
        return false;
    }
*/
    return true;
}

// Creates a new DomElement, writes the key connector data into it and appends it to the root object.
void KTouchKeyConnector::write(QDomDocument& doc, QDomElement& root) const {
/*	QDomElement element = doc.createElement("KeyConnector");
	QDomText textnode = doc.createTextNode(QString(m_keyChar));
	element.appendChild(textnode);
	element.setAttribute("TargetKeyChar", QString(m_targetKeyChar));
	if (m_fingerKeyChar != QChar(0))
		element.setAttribute("FingerKeyChar", QString(m_fingerKeyChar));
	if (!m_controlKeyId.isEmpty())
		element.setAttribute("ControlKeyId", m_controlKeyId);
	root.appendChild(element);
*/
}

