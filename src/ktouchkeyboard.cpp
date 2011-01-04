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

#include <tqfile.h>

#include <kdebug.h>
#include <ktempfile.h>
#include <kio/netaccess.h>
#include <klocale.h>

#include "prefs.h"

// --------------------------------------------------------------------------

// Clears the keyboard data
void KTouchKeyboard::clear() {
    m_keys.clear();
    m_connectors.clear();
	m_title = TQString::null;
	m_comment = TQString::null;
	m_language = TQString::null;
	m_fontSuggestions = TQString::null;
}
// ----------------------------------------------------------------------------

// Loads a keyboard tqlayout (old format) from file (returns true if successful).
bool KTouchKeyboard::load(TQWidget * window, const KURL& url) {
    // Ok, first download the contents as usual using the KIO lib
    // File is only downloaded if not local, otherwise it's just opened
    TQString target;
    bool result = false;
    //kdDebug() << "[KTouchKeyboard::load]  " << url << endl;
    if (KIO::NetAccess::download(url, target, window)) {
        // Ok, that was successful, store the lectureURL and read the file
        TQFile infile(target);
        if ( !infile.open( IO_ReadOnly ) )
            return false;   // Bugger it... couldn't open it...
        TQTextStream in( &infile );
        result = read(in);
    };
    KIO::NetAccess::removeTempFile(target);
    return result;
}
// ----------------------------------------------------------------------------

// Loads a lecture (in XML format) from file (returns true if successful).
bool KTouchKeyboard::loadXML(TQWidget * window, const KURL& url) {
    // Ok, first download the contents as usual using the KIO lib
    // File is only downloaded if not local, otherwise it's just opened
    TQString target;
    bool result = false;
    if (KIO::NetAccess::download(url, target, window)) {
        // Ok, that was successful, store the lectureURL and read the file
        TQFile infile(target);
        if ( !infile.open( IO_ReadOnly ) )
            return false;   // Bugger it... couldn't open it...
		TQDomDocument doc;
		doc.setContent( &infile );
        result = read(doc);
    }
    KIO::NetAccess::removeTempFile(target);
    return result;
}
// ----------------------------------------------------------------------------

// Saves the lecture data to file (returns true if successful).
bool KTouchKeyboard::saveXML(TQWidget * window, const KURL& url) const {
	// create the XML document
	TQDomDocument doc;
	write(doc);

	// and save it	
    TQString tmpFile;
    KTempFile *temp=0;
    if (url.isLocalFile())
        tmpFile=url.path();         // for local files the path is sufficient
    else {
        temp=new KTempFile;         // for remote files create a temporary file first
        temp->setAutoDelete(true);  // so we don't have to delete the file ourselves
        tmpFile=temp->name();
    }

    TQFile outfile(tmpFile);
    if ( !outfile.open( IO_WriteOnly ) ) {
        if (temp)  delete temp;
        // kdDebug() << "Error creating lecture file!" << endl;
        return false;
    };
	
    TQTextStream out( &outfile );
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
bool KTouchKeyboard::read(TQTextStream& in) {
    in.setEncoding(TQTextStream::UnicodeUTF8);
    TQString line;
    clear();          // empty the keyboard
    // now loop until end of file is reached
    do {
        // skip all empty lines or lines containing a comment (starting with '#')
        do {  line = in.readLine().stripWhiteSpace();  }
        while (!line.isNull() && (line.isEmpty() || line[0]=='#'));
        // Check if end of file encountered and if that is the case -> bail out at next while
        if (line.isNull())  continue;

        // 'line' should now contain a key specification
        TQTextStream lineStream(line, IO_ReadOnly);
        TQString keyType;
        int keyAscII;
        TQString keyText;
        int x(0), y(0), w(0), h(0);
        lineStream >> keyType >> keyAscII;
        if (keyType=="FingerKey") {
            lineStream >> keyText >> x >> y >> w >> h;
            if (w==0 || h==0)
                w=h=8; // default values for old keyboard files
			KTouchKey key(KTouchKey::FINGER, keyText[0], 0, x+1, y+1, w, h);
            m_keys.push_back(key);
			KTouchKeyConnector keycon(keyText[0], keyText[0], 0, 0); 
            m_connectors.push_back(keycon);
            kdDebug() << "[KTouchKeyboard::read]  FingerKey '" << keyText[0] << "'" << endl;
        }
        else if (keyType=="ControlKey") {
            lineStream >> keyText >> x >> y >> w >> h;
			KTouchKey key(KTouchKey::OTHER, 0, 0, x+1, y+1, w-2, h-2);
			key.m_otherKeyText = keyText;
            m_keys.push_back(key);
            kdDebug() << "[KTouchKeyboard::read]  ControlKey '" << keyText << "'" << endl;
        }
        else if (keyType=="NormalKey") {
            int fingerCharCode;
            lineStream >> keyText >> x >> y >> fingerCharCode;
            w=h=8; // default values for old keyboard files
			KTouchKey key(KTouchKey::NORMAL, keyText[0], 0, x+1, y+1, w, h);
            m_keys.push_back(key);
			KTouchKeyConnector keycon(keyText[0], keyText[0], fingerCharCode, 0); 
            m_connectors.push_back(keycon);
            kdDebug() << "[KTouchKeyboard::read]  NormalKey '" << keyText[0] << "' f = '"<< fingerCharCode<< "'" << endl;
        } else if (keyType=="HiddenKey") {
            int targetChar, fingerChar, controlChar;
            lineStream >> targetChar >> fingerChar >> controlChar;
			KTouchKeyConnector keycon(targetChar, targetChar, fingerChar, 0); 
            m_connectors.push_back(keycon);
        }
        else {
            //errorMsg = i18n("Missing key type in line '%1'.").arg(line);
            return false;
        }
        // calculate the maximum extent of the keyboard on the fly...
    } while (!in.atEnd() && !line.isNull());

    return (!m_keys.isEmpty());  // empty file means error
}
// ----------------------------------------------------------------------------

// Loads keyboard data from file into an XML document
bool KTouchKeyboard::read(const TQDomDocument& doc) {
	// clean current data
	kdDebug() << "Reading new keyboard tqlayout" << endl;
    m_keys.clear();
	m_connectors.clear();
	m_title = TQString::null;
	// retrieve the title
	TQDomNodeList entries = doc.elementsByTagName("Title");
	if (entries.count() >= 1)	m_title = entries.item(0).firstChild().nodeValue();
	else						m_title = i18n("untitled keyboard tqlayout");
	kdDebug() << "Title: " << m_title << endl;
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
	TQDomNode node = entries.item(0).firstChild();
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
        return false;
    };
	return false;
}
// ----------------------------------------------------------------------------

// Saves keyboard data in the XML document
void KTouchKeyboard::write(TQDomDocument& doc) const {
    TQDomElement root = doc.createElement( "KTouchKeyboard" );
    doc.appendChild(root);
	// Store title and ensure that the file tqcontains a title!
	TQDomElement title = doc.createElement("Title");
	TQDomText titleText;
	if (m_title.isEmpty())	titleText = doc.createTextNode( i18n("untitled keyboard tqlayout") );
	else					titleText = doc.createTextNode(m_title);
	title.appendChild(titleText);
	root.appendChild(title);
	// Store comment if given
	if (!m_comment.isEmpty()) {
		TQDomElement e = doc.createElement("Comment");
		TQDomText t = doc.createTextNode(m_comment);
		e.appendChild(t);
		root.appendChild(e);
	}
	// Store font suggestion if given
	if (!m_fontSuggestions.isEmpty()) {
		TQDomElement e = doc.createElement("FontSuggestions");
		TQDomText t = doc.createTextNode(m_fontSuggestions);
		e.appendChild(t);
		root.appendChild(e);
	}
	// Store language idif given
	if (!m_language.isEmpty()) {
		TQDomElement e = doc.createElement("Language");
		TQDomText t = doc.createTextNode(m_language);
		e.appendChild(t);
		root.appendChild(e);
	}
	// Store keys
	TQDomElement keys = doc.createElement("KeyDefinitions");
	root.appendChild(keys);
    for (TQValueVector<KTouchKey>::const_iterator it=m_keys.begin(); it!=m_keys.end(); ++it)
		it->write(doc, keys);
	// Store connectors
	TQDomElement conns = doc.createElement("KeyConnections");
	root.appendChild(conns);
    for (TQValueVector<KTouchKeyConnector>::const_iterator it=m_connectors.begin(); it!=m_connectors.end(); ++it)
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
    // First let's create the visible tqlayout.
	// This means we have to create all keys that will be displayed.
    // Note: purely decorative keys get a key character code of 0!
    m_keys.clear();
	m_keys.push_back( KTouchKey(KTouchKey::OTHER,    0, 0, 2*col+      0,     0, keyWidth, keyHeight) );
	m_keys.back().m_otherKeyText = i18n("Num-lock", "Num");
	m_keys.push_back( KTouchKey(KTouchKey::NORMAL, '/', 0, 2*col+    col,     0, keyWidth, keyHeight) );
	m_keys.push_back( KTouchKey(KTouchKey::NORMAL, '*', 0, 2*col+  2*col,     0, keyWidth, keyHeight) );
	m_keys.push_back( KTouchKey(KTouchKey::NORMAL, '-', 0, 2*col+  3*col,     0, keyWidth, keyHeight) );
	m_keys.push_back( KTouchKey(KTouchKey::NORMAL, '7', 0, 2*col+      0,   row, keyWidth, keyHeight) );
	m_keys.push_back( KTouchKey(KTouchKey::NORMAL, '8', 0, 2*col+  1*col,   row, keyWidth, keyHeight) );
	m_keys.push_back( KTouchKey(KTouchKey::NORMAL, '9', 0, 2*col+  2*col,   row, keyWidth, keyHeight) );
	m_keys.push_back( KTouchKey(KTouchKey::FINGER, '4', 0, 2*col+  	 0, 2*row, keyWidth, keyHeight) );
	m_keys.push_back( KTouchKey(KTouchKey::FINGER, '5', 0, 2*col+  1*col, 2*row, keyWidth, keyHeight) );
	m_keys.push_back( KTouchKey(KTouchKey::FINGER, '6', 0, 2*col+  2*col, 2*row, keyWidth, keyHeight) );
	m_keys.push_back( KTouchKey(KTouchKey::NORMAL, '1', 0, 2*col+  	 0, 3*row, keyWidth, keyHeight) );
	m_keys.push_back( KTouchKey(KTouchKey::NORMAL, '2', 0, 2*col+  1*col, 3*row, keyWidth, keyHeight) );
	m_keys.push_back( KTouchKey(KTouchKey::NORMAL, '3', 0, 2*col+  2*col, 3*row, keyWidth, keyHeight) );
	m_keys.push_back( KTouchKey(KTouchKey::NORMAL, '0', 0, 2*col+  	 0, 4*row, 2*keyWidth+keySpacing, keyHeight) );
	m_keys.push_back( KTouchKey(KTouchKey::NORMAL, '.', 0, 2*col+  2*col, 4*row, keyWidth, keyHeight) );
	m_keys.push_back( KTouchKey(KTouchKey::FINGER, '+', 0, 2*col+  3*col,   row, keyWidth, 2*keyHeight+keySpacing) );
	m_keys.push_back( KTouchKey(KTouchKey::ENTER,    0, 0, 2*col+  3*col, 3*row, keyWidth, 2*keyHeight+keySpacing) );
	m_keys.push_back( KTouchKey(KTouchKey::BACKSPACE,0, 0, 2*col+  5*col,     0, keyWidth, keyHeight) );

    // now we need to create the connections between the characters that can be typed and the
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
	
	m_title = "Number keypad";
	m_comment = "Predefined keyboard tqlayout";
	m_language = TQString::null;
	// language does not apply to numbers... that's one of the nice things with math :-)
	m_fontSuggestions = "Monospace";
    m_width = 8*col;
    m_height = 5*row;
}
// ----------------------------------------------------------------------------

void KTouchKeyboard::updateConnections() {
	for (TQValueVector<KTouchKeyConnector>::iterator it = m_connectors.begin(); it != m_connectors.end(); ++it)
		(*it).updateConnections(m_keys);
}
// ----------------------------------------------------------------------------


/*

bool KTouchKeyboard::loadKeyboard(TQWidget * window, const KURL& url, TQString* errorMsg) {
    TQString target;
    if (KIO::NetAccess::download(url, target, window)) {
        TQString msg;
        bool result = readKeyboard(target, msg);
        KIO::NetAccess::removeTempFile(target);
        if (!result && errorMsg!=NULL)
            *errorMsg = i18n("Could not read the keyboard tqlayout file '%1'. ").arg(url.url()) + msg;
        return result;
    }
    else {
        if (errorMsg!=NULL)
            *errorMsg = i18n("Could not download/open keyboard tqlayout file from '%1'.").arg(url.url());
        return false;
    }
}


void KTouchKeyboard::saveKeyboard(TQWidget * window, const KURL& url) {
    TQString tmpFile;
    KTempFile *temp=0;
    if (url.isLocalFile())
        tmpFile=url.path();             // for local files the path is sufficient
    else {
        temp=new KTempFile;             // for remote files create a temporary file first
        tmpFile=temp->name();
    }

    TQFile outfile(tmpFile);
    if ( !outfile.open( IO_WriteOnly ) ) {
        if (temp)  delete temp;
        return;
    }

    TQTextStream out( &outfile );
    out << "########################################## \n";
    out << "#                                        # \n";
    out << "#  Keyboard tqlayout file for KTouch       # \n";
    out << "#                                        # \n";
    out << "########################################## \n";
    out << "#\n";
    out << endl;

    for (KTouchKey * key = m_keyList.first(); key; key = m_keyList.next()) {
        switch (key->type()) {
           case KTouchKey::FINGER_KEY   : out << "FingerKey  "; break;
           case KTouchKey::NORMAL_KEY   : out << "NormalKey  "; break;
           case KTouchKey::CONTROL_KEY  : out << "ControlKey "; break;
           default : out << "NormalKey  "; break;
        }
        TQRect rect=key->frame();
        out << key->m_keyChar.tqunicode() << '\t' << key->m_keyText << '\t'
            << rect.left() << '\t' << rect.top() << '\t' << rect.width() << '\t' << rect.height() << endl;
    }

    if (temp) {
        KIO::NetAccess::upload(tmpFile, url, window);
        temp->unlink();
        delete temp;
    }
}

void KTouchKeyboard::applyPreferences(TQWidget * window, bool silent) {
    // let's check whether the keyboard tqlayout has changed
    if (KTouchConfig().m_currentKeyboardFile!=m_currentLayout) {
        // if the tqlayout is the number tqlayout just create it and we're done
        if (KTouchConfig().m_currentKeyboardFile=="number.keyboard") {
            createDefaultKeyboard();
            resizeEvent(NULL);
            return;
        }
        // ok, let's load this tqlayout
        if (silent) {
            // during initialisation we don't want to have a message box, that's why this is silent
            if (!loadKeyboard(window, KURL::fromPathOrURL( KTouchConfig().m_currentKeyboardFile )))
                createDefaultKeyboard();
            else
                m_currentLayout=KTouchConfig().m_currentKeyboardFile;
        }
        else {
            TQString errorMsg;
            if (!loadKeyboard(window, KURL::fromPathOrURL( KTouchConfig().m_currentKeyboardFile ), &errorMsg)) {
                KMessageBox::error( 0, i18n("Error reading the keyboard tqlayout; the default number keypad will "
                    "be created instead. You can choose another keyboard tqlayout in the preferences dialog."),
                    errorMsg);
                createDefaultKeyboard();
            }
            else
                m_currentLayout=KTouchConfig().m_currentKeyboardFile;
        }
    }

    updateColours();    // we recreate the colour connections,
    resizeEvent(NULL);  // paint the keyboard
    newKey(m_nextKey);  // and finally display the "next to be pressed" key again
}


void KTouchKeyboard::newKey(const TQChar& nextChar) {
    TQPainter painter(this);
    painter.translate(m_shift, MARGIN);
    // first clean the markings on all keys
    for (KTouchKey * key = m_keyList.first(); key; key = m_keyList.next()) {
        if (key->m_isActive || key->m_isNextKey) {
            key->m_isActive=key->m_isNextKey=false;
            key->paint(painter);
        }
    }

    if (Prefs::showAnimation()){ // only do this if we want to show animation.
        // find the key in the key connector list
        TQValueList<KTouchKeyConnector>::iterator keyIt = m_connectorList.begin();
        while (keyIt!=m_connectorList.end() && (*keyIt).m_keyChar!=nextChar)  ++keyIt;
        // if found mark the appropriate keys
        if (keyIt!=m_connectorList.end()) {
            TQChar targetChar = (*keyIt).m_targetKeyChar;
            TQChar fingerChar = (*keyIt).m_fingerKeyChar;
            TQChar controlChar = (*keyIt).m_controlKeyChar;
            // find the keys in the keylist
            for (KTouchKey * key = m_keyList.first(); key; key = m_keyList.next()) {
                if (key->m_keyChar==TQChar(0)) continue;    // skip decorative keys
                if (key->m_keyChar==targetChar) key->m_isNextKey=true;
                else if (key->m_keyChar==fingerChar)   key->m_isActive=true;
                else if (key->m_keyChar==controlChar)  key->m_isActive=true;
                if (key->m_isActive || key->m_isNextKey)
                    key->paint(painter);
            }
        }
        m_nextKey = nextChar;
    }
}


void KTouchKeyboard::paintEvent(TQPaintEvent *) {
    TQPainter painter(this);
    painter.translate(m_shift, MARGIN);
    // just print all visible keys
    for (KTouchKey * key = m_keyList.first(); key; key = m_keyList.next())
        key->paint(painter);
}


void KTouchKeyboard::resizeEvent(TQResizeEvent *) {
    double hScale = static_cast<double>(width()-2*MARGIN)/m_keyboardWidth;
    double vScale = static_cast<double>(height()-2*MARGIN)/m_keyboardHeight;
    double scale = std::min(hScale, vScale);
    m_shift = (width() - static_cast<int>(m_keyboardWidth*scale))/2;
    for (KTouchKey * key = m_keyList.first(); key; key = m_keyList.next())
        key->resize(scale);     // resize all keys
    update();                   // and finally redraw the keyboard
}


void KTouchKeyboard::createDefaultKeyboard() {
    // let's create a default keyboard
    const int keySpacing = 4;
    const int keyHeight = 20;
    const int keyWidth = 20;
    const int col = keyWidth+keySpacing;
    const int row = keyHeight+keySpacing;
    // first let's create the "visible" keys, that means all keys that will be displayed
    // Note: purely decorative keys get a key char code of 0!
    m_keyList.clear();
    m_keyList.append( new KTouchControlKey( 0, "Num",       0,     0, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '/', "/",       col,     0, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '*', "*",     2*col,     0, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '-', "-",     3*col,     0, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '1', "1",         0,   row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '2', "2",       col,   row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '3', "3",     2*col,   row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchFingerKey( '4', "4",         0, 2*row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchFingerKey( '5', "5",       col, 2*row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchFingerKey( '6', "6",     2*col, 2*row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '7', "7",         0, 3*row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '8', "8",       col, 3*row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '9', "9",     2*col, 3*row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '0', "0",         0, 4*row, 2*keyWidth+keySpacing, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '.', ".",     2*col, 4*row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchFingerKey( '+', "+",     3*col,   row, keyWidth, 2*keyHeight+keySpacing) );
    m_keyList.append( new KTouchControlKey(13, "Enter", 3*col, 3*row,keyWidth, 2*keyHeight+keySpacing) );
    m_keyList.append( new KTouchControlKey(8, "BackSpace", 5*col, 0, 2*keyWidth+keySpacing, keyHeight) );
    m_keyboardWidth = 7*col;
    m_keyboardHeight = 5*row;
    // now we need to create the connections between the characters that can be typed and the
    // keys that need to be displayed on the keyboard
    // The arguments to the constructor are: keychar, targetkey, fingerkey, controlkey
    m_connectorList.clear();
    m_connectorList.append( KTouchKeyConnector('/', '/', '5', 0) );
    m_connectorList.append( KTouchKeyConnector('*', '*', '6', 0) );
    m_connectorList.append( KTouchKeyConnector('-', '-', '+', 0) );
    m_connectorList.append( KTouchKeyConnector('+', '+',   0, 0) );
    m_connectorList.append( KTouchKeyConnector('.', '.', '6', 0) );
    m_connectorList.append( KTouchKeyConnector('1', '1', '4', 0) );
    m_connectorList.append( KTouchKeyConnector('2', '2', '5', 0) );
    m_connectorList.append( KTouchKeyConnector('3', '3', '6', 0) );
    m_connectorList.append( KTouchKeyConnector('4', '4',   0, 0) );
    m_connectorList.append( KTouchKeyConnector('5', '5',   0, 0) );
    m_connectorList.append( KTouchKeyConnector('6', '6',   0, 0) );
    m_connectorList.append( KTouchKeyConnector('7', '7', '4', 0) );
    m_connectorList.append( KTouchKeyConnector('8', '8', '5', 0) );
    m_connectorList.append( KTouchKeyConnector('9', '9', '6', 0) );
    m_connectorList.append( KTouchKeyConnector('0', '0',   0, 0) );
    m_connectorList.append( KTouchKeyConnector(  8,   8,   0, 0) );
    m_connectorList.append( KTouchKeyConnector( 13,  13, '+', 0) );
    updateColours();
    m_currentLayout="number";
}


bool KTouchKeyboard::readKeyboard(const TQString& fileName, TQString& errorMsg) {
    TQFile infile(fileName);
    if ( !infile.open( IO_ReadOnly ) ) {
        errorMsg = i18n("Could not open file.");
        return false;
    }
    TQTextStream in( &infile );
    in.setEncoding(TQTextStream::UnicodeUTF8);
    TQString line;
    m_keyList.clear();          // empty the keyboard
    m_connectorList.clear();    // clear the connections
    m_keyboardWidth=0;
    m_keyboardHeight=0;
    // now loop until end of file is reached
    do {
        // skip all empty lines or lines containing a comment (starting with '#')
        do {  line = in.readLine().stripWhiteSpace();  }
        while (!line.isNull() && (line.isEmpty() || line[0]=='#'));
        // Check if end of file encountered and if that is the case -> bail out at next while
        if (line.isNull())  continue;

        // 'line' should now contain a key specification
        TQTextStream lineStream(line, IO_ReadOnly);
        TQString keyType;
        int keyAscII;
        TQString keyText;
        int x(0), y(0), w(0), h(0);
        lineStream >> keyType >> keyAscII;
        if (keyType=="FingerKey") {
            lineStream >> keyText >> x >> y >> w >> h;
            if (w==0 || h==0)
                w=h=8; // default values for old keyboard files
            m_keyList.append( new KTouchFingerKey(keyAscII, keyText, x+1, y+1, w, h) );
            m_connectorList.append( KTouchKeyConnector(keyAscII, keyAscII, 0, 0) );
        }
        else if (keyType=="ControlKey") {
            lineStream >> keyText >> x >> y >> w >> h;
            m_keyList.append( new KTouchControlKey(keyAscII, keyText, x+1, y+1, w-2, h-2) );
            m_connectorList.append( KTouchKeyConnector(keyAscII, keyAscII, 0, 0) );
        }
        else if (keyType=="NormalKey") {
            int fingerCharCode;
            lineStream >> keyText >> x >> y >> fingerCharCode;
            w=h=8; // default values for old keyboard files
            // retrieve the finger key with the matching char
            m_keyList.append( new KTouchNormalKey(keyAscII, keyText, x+1, y+1, w, h) );
            m_connectorList.append( KTouchKeyConnector(keyAscII, keyAscII, fingerCharCode, 0) );
        } else if (keyType=="HiddenKey") {
            int targetChar, fingerChar, controlChar;
            lineStream >> targetChar >> fingerChar >> controlChar;
            m_connectorList.append( KTouchKeyConnector(keyAscII, targetChar, fingerChar, controlChar) );
        }
        else {
            errorMsg = i18n("Missing key type in line '%1'.").arg(line);
            return false;
        }
        // calculate the maximum extent of the keyboard on the fly...
        m_keyboardWidth = std::max(m_keyboardWidth, x+w);
        m_keyboardHeight = std::max(m_keyboardHeight, y+h);
    } while (!in.atEnd() && !line.isNull());
    updateColours();
    return (!m_keyList.isEmpty());  // empty file means error
}


void KTouchKeyboard::updateColours() {
    // loop over all key connections
    for (TQValueList<KTouchKeyConnector>::iterator it = m_connectorList.begin(); it!=m_connectorList.end(); ++it) {
        TQChar fingerChar = (*it).m_fingerKeyChar;
        if (fingerChar == TQChar(0)) continue;
        TQChar targetChar = (*it).m_targetKeyChar;
        KTouchKey * self=NULL;
        KTouchKey * colorSource=NULL;
        // loop over all keys to find the key pointers
        for (KTouchKey * key = m_keyList.first(); key; key = m_keyList.next()) {
            if (key->m_keyChar==targetChar) self=key;
            else if (key->m_keyChar==fingerChar) colorSource=key;
        }
        if (self && colorSource) {
            if (self->type()!=KTouchKey::NORMAL_KEY)
                continue;
            KTouchNormalKey *nk = dynamic_cast<KTouchNormalKey*>(self);
            if (colorSource->type()!=KTouchKey::FINGER_KEY) {
                kdDebug() << "[KTouchKeyboard::updateColours]  Colour source key '" << colorSource->m_keyText
                          << "' is not a finger key!" << endl;
                if (nk) {
                    nk->m_colorIndex = 0;
                }
                continue;
            }
            if (nk) {
                KTouchFingerKey *fk = dynamic_cast<KTouchFingerKey*>(colorSource);
                if (fk) {
                    nk->m_colorIndex = fk->m_colorIndex;
                }
            }
        }
    }
}

*/

