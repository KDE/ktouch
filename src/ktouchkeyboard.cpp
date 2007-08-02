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
#include "ktouchkeyboard.moc"

#include <QFile>
#include <QTextStream>
#include <QtXml/QDomDocument>
#include <QMap>

#include <kdebug.h>
#include <ktemporaryfile.h>
#include <kio/netaccess.h>
#include <klocale.h>
#include <kmessagebox.h>

#include "ktouchkey.h"
#include "ktouchkeyboardeditordialog.h"

// --------------------------------------------------------------------------

// Clears the keyboard data
void KTouchKeyboard::clear() {
	// disconnect all key events
	for (QList<KTouchKey*>::iterator it = m_keys.begin(); it != m_keys.end(); ++it) {
		(*it)->disconnect();
	}
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
bool KTouchKeyboard::load(QWidget * window, const KUrl& url, QString& msg) {
    // Ok, first download the contents as usual using the KIO lib
    // File is only downloaded if not local, otherwise it's just opened
    QString target;
    bool result = false;
    //kDebug() << "[KTouchKeyboard::load]  " << url;
    if (KIO::NetAccess::download(url, target, window)) {
        // Ok, that was successful, store the lectureURL and read the file
        QFile infile(target);
        if ( !infile.open( QIODevice::ReadOnly ) )
            return false;   // Bugger it... couldn't open it...
        QTextStream in( &infile );
        result = read(in, msg);
    }
	else {
		msg = i18n("Could not open/download keyboard file '%1'").arg(url.url());
	}
    KIO::NetAccess::removeTempFile(target);
    return result;
}
// ----------------------------------------------------------------------------

// Loads a lecture (in XML format) from file (returns true if successful).
bool KTouchKeyboard::loadXML(QWidget * window, const KUrl& url, QString& msg) {
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
		QString warnings;
        result = read(doc, warnings);
		if (!warnings.isEmpty()) {
			// TODO : show message box with warnings
		}
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
        // kDebug() << "Error creating lecture file!";
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

bool KTouchKeyboard::keyAlreadyExists(int keyUnicode, const QString &type, QString& warnings) {
	if (m_connectors.find(keyUnicode) != m_connectors.end()) {
		warnings += i18n("%1 with display character '%2' and unicode '%3' "
			"has been already defined and is skipped.\n", type, QChar(keyUnicode), keyUnicode);
		return true;
	}
	else return false;
}
// ----------------------------------------------------------------------------

// Loads keyboard data from file, preserved for compatibility
bool KTouchKeyboard::read(QTextStream& in, QString& warnings) {
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
        int keyUnicode;
        QString keyText;
        int x(0), y(0), w(8), h(8);
        lineStream >> keyType >> keyUnicode;
        if (keyType=="FingerKey") {
			if (keyAlreadyExists(keyUnicode, i18n("Finger key"), warnings))   continue;
            lineStream >> keyText >> x >> y >> w >> h;
            x *= 10; y *= 10; w *= 10; h *= 10;
            if (w==0)	w=80;
            if (h==0)	h=80;
			if (keyText.isEmpty()) continue;
			// add the key - in uppercase
			KTouchKey * key = new KTouchKey(this, KTouchKey::Finger, x, y, w, h, QChar(keyUnicode).toUpper());
            m_keys.push_back(key);
			// add the corresponding key connector
    		// KTouchKeyConnector(int keyUnicode, KTouchKey * target_key, KTouchKey * modifier_key)
			m_connectors[keyUnicode] = KTouchKeyConnector(keyUnicode, key, NULL);
        }
        else if (keyType=="ControlKey") {
			if (keyAlreadyExists(keyUnicode, i18n("Control key"), warnings))   continue;
            lineStream >> keyText >> x >> y >> w >> h;
            x *= 10; y *= 10; w = w*10 - 2*h; h = h*8;   
            // Note: 20% of height less in both directions for compatibility with old files 
			if (keyText.isEmpty()) continue;
			KTouchKey::keytype_t type = KTouchKey::Other;
			if (keyText.compare("Enter",Qt::CaseInsensitive)==0)			type = KTouchKey::Enter;
			else if (keyText.compare("Space",Qt::CaseInsensitive)==0)		type = KTouchKey::Space;
			else if (keyText.compare("BackSpace",Qt::CaseInsensitive)==0)	type = KTouchKey::Backspace;
			else if (keyText.compare("Shift",Qt::CaseInsensitive)==0)		type = KTouchKey::Shift;
			else if (keyText.compare("CapsLock",Qt::CaseInsensitive)==0)	type = KTouchKey::CapsLock;
			else if (keyText.compare("Tab",Qt::CaseInsensitive)==0)			type = KTouchKey::Tab;
			KTouchKey * key = new KTouchKey(this, x, y, w, h, keyText);
			key->m_type = type;
            m_keys.push_back(key);
			// add a key connector
    		// KTouchKeyConnector(int keyUnicode, KTouchKey * target_key, KTouchKey * modifier_key)
			m_connectors[keyUnicode] = KTouchKeyConnector(keyUnicode, key, NULL);
        }
        else if (keyType=="NormalKey") {
			if (keyAlreadyExists(keyUnicode, i18n("Normal key"), warnings))   continue;
            int fingerUnicode;
            lineStream >> keyText >> x >> y >> fingerUnicode;
            x *= 10; y *= 10; w *= 10; h *= 10;
			if (keyText.isEmpty()) continue;
			// lookup the finger key index
			KTouchKeyConnector & fingerKeyConn = m_connectors[ fingerUnicode ];
			if (fingerKeyConn.m_targetKey == NULL) {
				warnings += i18n("Unknown finger key with unicode '%1'. Normal key with "
					"display character '%2' and unicode '%3' skipped.\n", fingerUnicode, QChar(keyUnicode), keyUnicode);
				continue;
			}
			// add the key - uppercase display character
			KTouchKey * key = new KTouchKey(this, KTouchKey::Normal, x, y, w, h, QChar(keyUnicode).toUpper());
			// identify the finger key index (the pointer is set below)
			key->m_fingerKeyIndex = m_keys.indexOf(fingerKeyConn.m_targetKey);
            m_keys.push_back(key);
    		// KTouchKeyConnector(int keyUnicode, KTouchKey * target_key, KTouchKey * modifier_key)
			m_connectors[keyUnicode] = KTouchKeyConnector(keyUnicode, key, NULL);

        } 
        else if (keyType=="HiddenKey") {
			if (keyAlreadyExists(keyUnicode, i18n("Hidden key"), warnings))   continue;
            int targetUnicode, fingerUnicode, modifierUnicode;
            lineStream >> targetUnicode >> fingerUnicode >> modifierUnicode;
			// lookup the associated keys
			KTouchKeyConnector & targetKeyConn 		= m_connectors[ targetUnicode ];
			KTouchKeyConnector & fingerKeyConn 		= m_connectors[ fingerUnicode ];
			KTouchKeyConnector & modifierKeyConn 	= m_connectors[ modifierUnicode ];

			if (targetKeyConn.m_targetKey == NULL) {
				warnings += i18n("Unknown target key with unicode '%1'. Hidden key with "
					"display character '%2' and unicode '%3' skipped.\n", targetUnicode, QChar(keyUnicode), keyUnicode);
				continue;
			}
			if (fingerKeyConn.m_targetKey == NULL) {
				warnings += i18n("Unknown finger key with unicode '%1'. Hidden key with "
					"display character '%2' and unicode '%3' skipped.\n", fingerUnicode, QChar(keyUnicode), keyUnicode);
				continue;
			}
			if (modifierUnicode != -1 && modifierKeyConn.m_targetKey == NULL) {
				warnings += i18n("Unknown modifier/control key with unicode '%1'. Hidden key with "
					"display character '%2' and unicode '%3' skipped.\n", modifierUnicode, QChar(keyUnicode), keyUnicode);
				continue;
			}
    		// KTouchKeyConnector(int keyUnicode, KTouchKey * target_key, KTouchKey * modifier_key)
			m_connectors[keyUnicode] = KTouchKeyConnector(keyUnicode,
														  targetKeyConn.m_targetKey,
														  modifierKeyConn.m_targetKey);
        }
        else {
            //qdebug() << i18n("Missing key type in line '%1'.",line);
        }
        // TODO : calculate the maximum extent of the keyboard on the fly...
    } while (!in.atEnd() && !line.isNull());

	updateKeyPointers();

    return (!m_keys.isEmpty());  // empty file means error
}
// ----------------------------------------------------------------------------

// Loads keyboard data from file into an XML document
bool KTouchKeyboard::read(const QDomDocument& doc, QString& warnings) {
	QDomElement root = doc.documentElement();
	if (root.isNull() || root.tagName() != "KTouchKeyboard") return false;
	
	clear();	// clear current data
	
	// get the title
	QDomElement e = root.firstChildElement("Title");
	if (!e.isNull())	m_title = e.firstChild().nodeValue();
	else				m_title = i18n("untitled keyboard layout");
	// retrieve the comment
	e = root.firstChildElement("Comment");
	if (!e.isNull())	m_comment = e.firstChild().nodeValue();
	// retrieve the font suggestion
	e = root.firstChildElement("FontSuggestions");
	if (!e.isNull())	m_fontSuggestions = e.firstChild().nodeValue();
	// retrieve the language id
	e = root.firstChildElement("Language");
	if (!e.isNull())	m_language = e.firstChild().nodeValue();
	
	// read all keys
	e = root.firstChildElement("Keys");
	if (!e.isNull()) {
		QDomElement keyElement = e.firstChildElement("Key");
		while (!keyElement.isNull()) {
			// create new key
			KTouchKey * k = new KTouchKey(this);
			k->read(keyElement);
			m_keys.append(k);
			keyElement = keyElement.nextSiblingElement("Key");
		}
	}
	
	// read all key connectors
	e = root.firstChildElement("Connections");
	if (!e.isNull()) {
		QDomElement connectorElement = e.firstChildElement("KeyConnector");
		while (!connectorElement.isNull()) {
			// create new connector
			KTouchKeyConnector c;
			c.read(connectorElement,m_keys);
			m_connectors[c.m_keyUnicode] = c;
			connectorElement = connectorElement.nextSiblingElement("KeyConnector");
		}
	}
	kDebug() << "Read keyboard '"<< m_title << "' with " << m_keys.count() << " keys and " << m_connectors.count() << " characters"; 

	updateKeyPointers();

	// TODO : test if the keyboard was read correctly
	return true;
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
		(*it)->write(doc, keys, m_keys);
	// Store connectors
	QDomElement conns = doc.createElement("Connections");
	root.appendChild(conns);
    for (QMap<int, KTouchKeyConnector>::const_iterator it=m_connectors.begin(); it!=m_connectors.end(); ++it)
		it->write(doc, conns, m_keys);
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
	m_keys.push_back( new KTouchKey(this, 2*col+      0,     0, keyWidth, keyHeight, i18nc("Num-lock", "Num")) );
	m_keys.push_back( new KTouchKey(this, KTouchKey::Normal, 2*col+    col,     0, keyWidth, keyHeight, QChar('/') ) );
	m_keys.push_back( new KTouchKey(this, KTouchKey::Normal, 2*col+  2*col,     0, keyWidth, keyHeight, QChar('*') ) );
	m_keys.push_back( new KTouchKey(this, KTouchKey::Normal, 2*col+  3*col,     0, keyWidth, keyHeight, QChar('-') ) );
	m_keys.push_back( new KTouchKey(this, KTouchKey::Normal, 2*col+      0,   row, keyWidth, keyHeight, QChar('7') ) );
	m_keys.push_back( new KTouchKey(this, KTouchKey::Normal, 2*col+  1*col,   row, keyWidth, keyHeight, QChar('8') ) );
	m_keys.push_back( new KTouchKey(this, KTouchKey::Normal, 2*col+  2*col,   row, keyWidth, keyHeight, QChar('9') ) );
	m_keys.push_back( new KTouchKey(this, KTouchKey::Finger, 2*col+  	 0, 2*row, keyWidth, keyHeight, QChar('4') ) );
	m_keys.push_back( new KTouchKey(this, KTouchKey::Finger, 2*col+  1*col, 2*row, keyWidth, keyHeight, QChar('5') ) );
	m_keys.push_back( new KTouchKey(this, KTouchKey::Finger, 2*col+  2*col, 2*row, keyWidth, keyHeight, QChar('6') ) );
	m_keys.push_back( new KTouchKey(this, KTouchKey::Normal, 2*col+  	 0, 3*row, keyWidth, keyHeight, QChar('1') ) );
	m_keys.push_back( new KTouchKey(this, KTouchKey::Normal, 2*col+  1*col, 3*row, keyWidth, keyHeight, QChar('2') ) );
	m_keys.push_back( new KTouchKey(this, KTouchKey::Normal, 2*col+  2*col, 3*row, keyWidth, keyHeight, QChar('3') ) );
	m_keys.push_back( new KTouchKey(this, KTouchKey::Normal, 2*col+  	 0, 4*row, 2*keyWidth+keySpacing, keyHeight, QChar('0') ) );
	m_keys.push_back( new KTouchKey(this, KTouchKey::Normal, 2*col+  2*col, 4*row, keyWidth, keyHeight, QChar('.') ) );
	m_keys.push_back( new KTouchKey(this, KTouchKey::Finger, 2*col+  3*col,   row, keyWidth, 2*keyHeight+keySpacing, QChar('+') ) );
	m_keys.push_back( new KTouchKey(this, KTouchKey::Enter,     2*col+  3*col, 3*row, keyWidth, 2*keyHeight+keySpacing, QChar() ) );
	m_keys.push_back( new KTouchKey(this, KTouchKey::Backspace, 2*col+  5*col,     0, keyWidth, keyHeight, QChar() ) );

	m_title = "Number keypad";
	m_comment = "Predefined keyboard layout";
	m_language.clear();
	// language does not apply to numbers... that's one of the nice things with math :-)
	m_fontSuggestions = "Monospace";
	updateKeyPointers();
}
// ----------------------------------------------------------------------------

void KTouchKeyboard::updateKeyPointers() {
	// loop over all keys and number the finger keys
	int fingerKeyIndex = 0;
	for (QList<KTouchKey*>::iterator it = m_keys.begin(); it != m_keys.end(); ++it) {
		if ((*it)->m_type == KTouchKey::Finger) {
			if (fingerKeyIndex == 8) {
				kDebug() << "Too many finger keys in keyboard!";
				fingerKeyIndex = 7;
			}
			(*it)->m_colorIndex = fingerKeyIndex++;
		}
		else {
			// now check of the key has a finger key index set and if this is the case, set the finger key pointers
			if ((*it)->m_fingerKeyIndex > -1 && (*it)->m_fingerKeyIndex < m_keys.count()) {
				// set the finger key pointer
				(*it)->m_fingerKey = m_keys[(*it)->m_fingerKeyIndex];
				// and the color index from the finger key
				(*it)->m_colorIndex = (*it)->m_fingerKey->m_colorIndex;
				//kDebug() << "Key " << (*it)->m_keyChar[0] << " has finger key " << (*it)->m_fingerKeyIndex;
			}
			else {
				(*it)->m_colorIndex = -1;
			}
		}
	}
	// now all normal keys have pointers to finger keys and also updated color indices for drawing.
}
// ----------------------------------------------------------------------------

void KTouchKeyboard::setFont(const QFont& f) {
	m_font = f;
	for (QList<KTouchKey*>::const_iterator it = m_keys.begin(); it != m_keys.end(); ++it) {
		(*it)->update();
	}
}
// ----------------------------------------------------------------------------

bool KTouchKeyboard::allowKeyClicks() const {
	// check if the keyboard editor allows us to click
	KTouchKeyboardEditorDialog * kbed = dynamic_cast<KTouchKeyboardEditorDialog *>(parent());
	if (kbed != NULL && 
        !kbed->selectFingerKeyButton->isChecked() && 
        !kbed->selectModifierKeyButton->isChecked()) return true;
	else return false;
}
// ----------------------------------------------------------------------------

void KTouchKeyboard::deleteKey(KTouchKey * k) {
	// check if key exists
	if (!m_keys.contains(k)) return;
	// disconnect key
	k->disconnect();
	// delete key
	delete k;
	// remove key from list
	int i = m_keys.indexOf(k);
	if (i != -1)	m_keys.removeAt(i);
	// loop over all remaining keys and remove all references to the key (in case it was a finger key)
	for (QList<KTouchKey*>::const_iterator it = m_keys.begin(); it != m_keys.end(); ++it) {
		if ((*it)->m_fingerKey == k) {
			(*it)->m_fingerKey = NULL;
			(*it)->m_colorIndex = -1;
		}
	}
	// loop over all key connectors and remove all references to the deleted key
	QList<int> connectorsToRemove;
	for (QMap<int, KTouchKeyConnector>::iterator it = m_connectors.begin(); it != m_connectors.end(); ++it) {
		if ((*it).m_targetKey == k) {
			connectorsToRemove.append(it.key());
			continue;
		}
		// if just the modifier key was deleted, simply set the pointer to zero
		if ((*it).m_modifierKey == k)
			(*it).m_modifierKey = NULL;
	}
	for (int i=0; i<connectorsToRemove.count(); ++i) {
		//kDebug() << "Removing connector for unicode " << connectorsToRemove[i];
		m_connectors.remove(connectorsToRemove[i]);
	}
}
// ----------------------------------------------------------------------------
