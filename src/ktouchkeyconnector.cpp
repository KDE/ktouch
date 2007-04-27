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

#include "ktouchkey.h"

// Reads the key connector data from the DomElement
bool KTouchKeyConnector::read(QDomElement e) {
	if (e.hasAttribute("KeyUnicode"))
		m_keyUnicode = e.attribute("KeyUnicode").toInt();
	else
		return false;
	if (e.hasAttribute("TargetKeyIndex"))
		m_targetKeyIndex = e.attribute("TargetKeyIndex").toInt();
	if (e.hasAttribute("FingerKeyIndex"))
		m_fingerKeyIndex = e.attribute("FingerKeyIndex").toInt();
	if (e.hasAttribute("ModifierIndex"))
		m_modifierKeyIndex = e.attribute("ModifierIndex").toInt();
    return true;
}

// Creates a new DomElement, writes the key connector data into it and appends it to the root object.
void KTouchKeyConnector::write(QDomDocument& doc, QDomElement& root) const {
	QDomElement element = doc.createElement("KeyConnector");
	element.setAttribute("KeyUnicode", m_keyUnicode);
	if (m_targetKeyIndex != -1)
		element.setAttribute("TargetKeyIndex", m_targetKeyIndex);
	if (m_fingerKeyIndex != -1)
		element.setAttribute("FingerKeyIndex", m_fingerKeyIndex);
	if (m_modifierKeyIndex != -1)
		element.setAttribute("ModifierIndex", m_modifierKeyIndex);
	root.appendChild(element);
}

