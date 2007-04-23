/***************************************************************************
 *   ktouchkeyboard.cpp                                                    *
 *   ------------------                                                    *
 *   Copyright (C) 2004 by Andreas Nicolai                                 *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchkeyboard.h"

#include <QFile>
#include <QTextStream>
#include <QtXml>
#include <QMap>

#include <kdebug.h>
#include <ktemporaryfile.h>
#include <kio/netaccess.h>
#include <klocale.h>

#include "ktouchkey.h"

// --------------------------------------------------------------------------

// Clears the keyboard data
void KTouchKeyboard::clear() {
	qDeleteAll(m_keys);
    m_keys.clear();
    m_connectors.clear();
	m_title.clear();
	m_comment.clear();
	m_language.clear();
	m_fontSuggestions.clear();
}
// ----------------------------------------------------------------------------

// Loads a keyboard layout (old format) from file (returns true if successful).
bool KTouchKeyboard::load(QWidget * window, const KUrl& url) {
    // Ok, first download the contents as usual using the KIO lib
    // File is only downloaded if not local, otherwise it's just opened
    QString target;
    bool result = false;
    //kDebug() << "[KTouchKeyboard::load]  " << url << endl;
    if (KIO::NetAccess::download(url, target, window)) {
        // Ok, that was successful, store the lectureURL and read the file
        QFile infile(target);
        if ( !infile.open( QIODevice::ReadOnly ) )
            return false;   // Bugger it... couldn't open it...
        QTextStream in( &infile );
        result = read(in);
    };
    KIO::NetAccess::removeTempFile(target);
    return result;
}
// ----------------------------------------------------------------------------

// Loads a lecture (in XML format) from file (returns true if successful).
bool KTouchKeyboard::loadXML(QWidget * window, const KUrl& url) {
    // Ok, first download the contents as usual using the KIO lib
    // File is only downloaded if not local, otherwise it's just opened
    QString target;
    bool result = false;
    if (KIO::NetAccess::download(url, target, window)) {
        // Ok, that was successful, store the lectureURL and read the file
        QFile infile(target);
        if ( !infile.open( QIODevice::ReadOnly ) )
            return false;   // Bugger it... couldn't open it...
		QDomDocument doc;
		doc.setContent( &infile );
        result = read(doc);
    }
    KIO::NetAccess::removeTempFile(target);
    return result;
}
// ----------------------------------------------------------------------------

// Saves the lecture data to file (returns true if successful).
bool KTouchKeyboard::saveXML(QWidget * window, const KUrl& url) const {
	// create the XML document
	QDomDocument doc;
	write(doc);

	// and save it
    QString tmpFile;
    KTemporaryFile *temp=0;
    if (url.isLocalFile())
        tmpFile=url.path();         // for local files the path is sufficient
    else {
        temp=new KTemporaryFile;         // for remote files create a temporary file first
        temp->open();
        tmpFile=temp->fileName();
    }

    QFile outfile(tmpFile);
    if ( !outfile.open( QIODevice::WriteOnly ) ) {
        if (temp)  delete temp;
        // kDebug() << "Error creating lecture file!" << endl;
        return false;
    }

    QTextStream out( &outfile );
    out << doc.toString();
    outfile.close();
    // if we have a temporary file, we still need to upload it
    if (temp) {
        KIO::NetAccess::upload(tmpFile, url, window);
        delete temp;
    }
    return true;
}
// ----------------------------------------------------------------------------

// Loads keyboard data from file, preserved for compatibility
bool KTouchKeyboard::read(QTextStream& in) {
    QString line;
    clear();          // empty the keyboard

    // now loop until end of file is reached
    do {
        // skip all empty lines or lines containing a comment (starting with '#')
        do {  line = in.readLine().trimmed();  }
        while (!line.isNull() && (line.isEmpty() || line[0]=='#'));
        // Check if end of file encountered and if that is the case -> bail out at next while
        if (line.isNull())  continue;

        // 'line' should now contain a key specification
        QTextStream lineStream(&line, QIODevice::ReadOnly);
        QString keyType;
        int keyAscII;
        QString keyText;
        int x(0), y(0), w(0), h(0);
        lineStream >> keyType >> keyAscII;
        if (keyType=="FingerKey") {
            lineStream >> keyText >> x >> y >> w >> h;
			if (keyText.isEmpty()) continue;
            w=h=8; // set default values for finger keys
			KTouchKey * key = new KTouchKey(KTouchKey::FINGER, x+1, y+1, w, h, keyText[0]);
            m_keys.push_back(key);
        }
        else if (keyType=="ControlKey") {
            lineStream >> keyText >> x >> y >> w >> h;
			if (keyText.isEmpty()) continue;
			KTouchKey * key = new KTouchKey(x+1, y+1, w-2, h-2, keyText);
            m_keys.push_back(key);
        }
        else if (keyType=="NormalKey") {
            int fingerCharCode;
            lineStream >> keyText >> x >> y >> fingerCharCode;
			if (keyText.isEmpty()) continue;
            w=h=8; // set default values for normal keys
			KTouchKey * key = new KTouchKey(KTouchKey::NORMAL, x+1, y+1, w, h, keyText[0]);
            m_keys.push_back(key);
			//character_map[keyText[0]] = fingerCharCode;
        } else if (keyType=="HiddenKey") {
            int targetChar, fingerChar, controlChar;
            lineStream >> targetChar >> fingerChar >> controlChar;
			//KTouchKeyConnector keycon(targetChar, targetChar, fingerChar, 0);
            //m_connectors.push_back(keycon);
        }
        else {
            //qdebug() << i18n("Missing key type in line '%1'.",line);
            return false;
        }
        // TODO : calculate the maximum extent of the keyboard on the fly...
    } while (!in.atEnd() && !line.isNull());

    return (!m_keys.isEmpty());  // empty file means error

    return false;
}
// ----------------------------------------------------------------------------

// Loads keyboard data from file into an XML document
bool KTouchKeyboard::read(const QDomDocument& doc) {
/*
	// clean current data
	kDebug() << "Reading new keyboard layout" << endl;
    m_keys.clear();
	m_connectors.clear();
	m_title.clear();
	// retrieve the title
	QDomNodeList entries = doc.elementsByTagName("Title");
	if (entries.count() >= 1)	m_title = entries.item(0).firstChild().nodeValue();
	else						m_title = i18n("untitled keyboard layout");
	kDebug() << "Title: " << m_title << endl;
	// retrieve the comment
	entries = doc.elementsByTagName("Comment");
	if (entries.count() >= 1)
		m_comment = entries.item(0).firstChild().nodeValue();
	// retrieve the font suggestion
	entries = doc.elementsByTagName("FontSuggestions");
	if (entries.count() >= 1)
		m_fontSuggestions = entries.item(0).firstChild().nodeValue();
	// retrieve the language id
	entries = doc.elementsByTagName("Language");
	if (entries.count() >= 1)
		m_language = entries.item(0).firstChild().nodeValue();
	// retrieve the key definitions
	entries = doc.elementsByTagName("KeyDefinitions");
	QDomNode node = entries.item(0).firstChild();
	while (!node.isNull()) {
        KTouchKey key;
		if (key.read(node))
        	m_keys.push_back(key);
		// TODO : Error message if reading failed
		node = node.nextSibling();
	}
	// retrieve the key definitions
	entries = doc.elementsByTagName("KeyConnections");
	node = entries.item(0).firstChild();
	while (!node.isNull()) {
        KTouchKeyConnector keycon;
		if (keycon.read(node))
        	m_connectors.push_back(keycon);
		// TODO : Error message if reading failed
		node = node.nextSibling();
	}
    if (!m_title.isEmpty())
        return true;  // if we could read a title, let's assume all is ok
    else {
        // Hmm, no levels in the file. So we create our default mini level and report an error.
        createDefault();
7        return false;
    };
*/
	return false;
}
// ----------------------------------------------------------------------------

// Saves keyboard data in the XML document
void KTouchKeyboard::write(QDomDocument& doc) const {
    QDomElement root = doc.createElement( "KTouchKeyboard" );
    doc.appendChild(root);
	// Store title and ensure that the file contains a title!
	QDomElement title = doc.createElement("Title");
	QDomText titleText;
	if (m_title.isEmpty())	titleText = doc.createTextNode( i18n("untitled keyboard layout") );
	else					titleText = doc.createTextNode(m_title);
	title.appendChild(titleText);
	root.appendChild(title);
	// Store comment if given
	if (!m_comment.isEmpty()) {
		QDomElement e = doc.createElement("Comment");
		QDomText t = doc.createTextNode(m_comment);
		e.appendChild(t);
		root.appendChild(e);
	}
	// Store font suggestion if given
	if (!m_fontSuggestions.isEmpty()) {
		QDomElement e = doc.createElement("FontSuggestions");
		QDomText t = doc.createTextNode(m_fontSuggestions);
		e.appendChild(t);
		root.appendChild(e);
	}
	// Store language idif given
	if (!m_language.isEmpty()) {
		QDomElement e = doc.createElement("Language");
		QDomText t = doc.createTextNode(m_language);
		e.appendChild(t);
		root.appendChild(e);
	}
	// Store keys
	QDomElement keys = doc.createElement("Keys");
	root.appendChild(keys);
	for (QList<KTouchKey*>::const_iterator it=m_keys.begin(); it!=m_keys.end(); ++it)
		(*it)->write(doc, keys);
	// Store connectors
	QDomElement conns = doc.createElement("Connections");
	root.appendChild(conns);
    for (QMap<QChar, KTouchKeyConnector>::const_iterator it=m_connectors.begin(); it!=m_connectors.end(); ++it)
		it->write(doc, conns);
}
// ----------------------------------------------------------------------------

// Creates the default number keyboard.
void KTouchKeyboard::createDefault() {

    // let's create a default keyboard
    const int keySpacing = 4;
    const int keyHeight = 20;
    const int keyWidth = 20;
    const int col = keyWidth+keySpacing;
    const int row = keyHeight+keySpacing;
    // First let's create the visible layout.
	// This means we have to create all keys that will be displayed.
    // Note: purely decorative keys get a key character code of 0!
	qDeleteAll(m_keys);
    m_keys.clear();
	m_keys.push_back( new KTouchKey(2*col+      0,     0, keyWidth, keyHeight, i18nc("Num-lock", "Num")) );
	m_keys.push_back( new KTouchKey(KTouchKey::NORMAL, 2*col+    col,     0, keyWidth, keyHeight, QChar('/') ) );
	m_keys.push_back( new KTouchKey(KTouchKey::NORMAL, 2*col+  2*col,     0, keyWidth, keyHeight, QChar('*') ) );
	m_keys.push_back( new KTouchKey(KTouchKey::NORMAL, 2*col+  3*col,     0, keyWidth, keyHeight, QChar('-') ) );
	m_keys.push_back( new KTouchKey(KTouchKey::NORMAL, 2*col+      0,   row, keyWidth, keyHeight, QChar('7') ) );
	m_keys.push_back( new KTouchKey(KTouchKey::NORMAL, 2*col+  1*col,   row, keyWidth, keyHeight, QChar('8') ) );
	m_keys.push_back( new KTouchKey(KTouchKey::NORMAL, 2*col+  2*col,   row, keyWidth, keyHeight, QChar('9') ) );
	m_keys.push_back( new KTouchKey(KTouchKey::FINGER, 2*col+  	 0, 2*row, keyWidth, keyHeight, QChar('4') ) );
	m_keys.push_back( new KTouchKey(KTouchKey::FINGER, 2*col+  1*col, 2*row, keyWidth, keyHeight, QChar('5') ) );
	m_keys.push_back( new KTouchKey(KTouchKey::FINGER, 2*col+  2*col, 2*row, keyWidth, keyHeight, QChar('6') ) );
	m_keys.push_back( new KTouchKey(KTouchKey::NORMAL, 2*col+  	 0, 3*row, keyWidth, keyHeight, QChar('1') ) );
	m_keys.push_back( new KTouchKey(KTouchKey::NORMAL, 2*col+  1*col, 3*row, keyWidth, keyHeight, QChar('2') ) );
	m_keys.push_back( new KTouchKey(KTouchKey::NORMAL, 2*col+  2*col, 3*row, keyWidth, keyHeight, QChar('3') ) );
	m_keys.push_back( new KTouchKey(KTouchKey::NORMAL, 2*col+  	 0, 4*row, 2*keyWidth+keySpacing, keyHeight, QChar('0') ) );
	m_keys.push_back( new KTouchKey(KTouchKey::NORMAL, 2*col+  2*col, 4*row, keyWidth, keyHeight, QChar('.') ) );
	m_keys.push_back( new KTouchKey(KTouchKey::FINGER, 2*col+  3*col,   row, keyWidth, 2*keyHeight+keySpacing, QChar('+') ) );
	m_keys.push_back( new KTouchKey(KTouchKey::ENTER,     2*col+  3*col, 3*row, keyWidth, 2*keyHeight+keySpacing, QChar() ) );
	m_keys.push_back( new KTouchKey(KTouchKey::BACKSPACE, 2*col+  5*col,     0, keyWidth, keyHeight, QChar() ) );
/*
    // now we need to create the connections between the characters thaTt can be typed and the
    // keys that need to be displayed on the keyboard
    // The arguments to the constructor are: keychar, targetkey, fingerkey, controlkeyid

	m_connectors.clear();
	m_connectors.push_back( KTouchKeyConnector('/', '/','5', 0) );
	m_connectors.push_back( KTouchKeyConnector('*', '*','6', 0) );
	m_connectors.push_back( KTouchKeyConnector('-', '-','+', 0) );
	m_connectors.push_back( KTouchKeyConnector('+', '+',  0, 0) );
	m_connectors.push_back( KTouchKeyConnector('0', '0',  0, 0) );
	m_connectors.push_back( KTouchKeyConnector('1', '1','4', 0) );
	m_connectors.push_back( KTouchKeyConnector('2', '2','5', 0) );
	m_connectors.push_back( KTouchKeyConnector('3', '3','6', 0) );
	m_connectors.push_back( KTouchKeyConnector('4', '4',  0, 0) );
	m_connectors.push_back( KTouchKeyConnector('5', '5',  0, 0) );
	m_connectors.push_back( KTouchKeyConnector('6', '6',  0, 0) );
	m_connectors.push_back( KTouchKeyConnector('7', '7','4', 0) );
	m_connectors.push_back( KTouchKeyConnector('8', '8','5', 0) );
	m_connectors.push_back( KTouchKeyConnector('9', '9','6', 0) );
    m_connectors.push_back( KTouchKeyConnector('.', '.', '6', 0) );
*/
	m_title = "Number keypad";
	m_comment = "Predefined keyboard layout";
	m_language.clear();
	// language does not apply to numbers... that's one of the nice things with math :-)
	m_fontSuggestions = "Monospace";
    m_width = 8*col;
    m_height = 5*row;
}
// ----------------------------------------------------------------------------

void KTouchKeyboard::updateConnections() {
//	for (QList<KTouchKeyConnector>::iterator it = m_connectors.begin(); it != m_connectors.end(); ++it)
//		(*it).updateConnections(m_keys);
}
// ----------------------------------------------------------------------------

