/***************************************************************************
 *   ktouchkeyconnector.cpp                                                *
 *   ---------------------                                                 *
 *   Copyright (C) 2007 by Andreas Nicolai                                 *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchkeyconnector.h"

#include <QtXml>

// Reads the key connector data from the DomElement
bool KTouchKeyConnector::read(QDomElement e, const QList<KTouchKey*>& keylist) {
	if (e.hasAttribute("KeyUnicode"))
		m_keyUnicode = e.attribute("KeyUnicode").toInt();
	else
		return false;
	if (e.hasAttribute("TargetKeyIndex")) {
		int index = e.attribute("TargetKeyIndex").toInt();
		if (index >= 0 && index < keylist.count())
			m_targetKey = keylist[index];
	}
	if (e.hasAttribute("ModifierIndex")) {
		int index = e.attribute("ModifierIndex").toInt();
		if (index >= 0 && index < keylist.count())
			m_modifierKey = keylist[index];
	}
    return true;
}

// Creates a new DomElement, writes the key connector data into it and appends it to the root object.
void KTouchKeyConnector::write(QDomDocument& doc, QDomElement& root, 
	const QList<KTouchKey*>& keylist) const 
{
	QDomElement element = doc.createElement("KeyConnector");
	element.setAttribute("KeyUnicode", m_keyUnicode);
	int index = keylist.indexOf(m_targetKey);
	if (index != -1)
		element.setAttribute("TargetKeyIndex", index);
	index = keylist.indexOf(m_modifierKey);
	if (index != -1)
		element.setAttribute("ModifierIndex", index);
	root.appendChild(element);
}

