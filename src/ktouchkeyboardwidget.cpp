/***************************************************************************
 *   ktouchkeyboardwidget.cpp                                              *
 *   ------------------------                                              *
 *   Copyright (C) 2000 by Håvard Frøiland, 2004 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchkeyboardwidget.h"
#include "ktouchkeyboardwidget.moc"

#include <algorithm>
#include <set>

#include <qfile.h>

#include <kdebug.h>
#include <ktempfile.h>
#include <klocale.h>
#include <kio/netaccess.h>
#include <kstandarddirs.h>
#include <kmessagebox.h>

#include "prefs.h"
#include "ktouchkeyconnector.h"

// the margin between keyboard and widget frame
const int MARGIN = 10;

QMap<QChar, int>	 			KTouchKeyboardWidget::m_keyCharMap;
// --------------------------------------------------------------------------


KTouchKeyboardWidget::KTouchKeyboardWidget(QWidget *parent)
  : QWidget(parent), m_keyboardWidth(100), m_keyboardHeight(60), m_currentLayout(""),
	m_hideKeyboard(false)
{
    setMinimumHeight(100);          // when it's smaller you won't see anything
    m_keyList.setAutoDelete(true);  // the list is responsable for cleaning up
}


bool KTouchKeyboardWidget::loadKeyboard(QWidget * window, const KURL& url, QString* errorMsg) {
    QString target;
    if (KIO::NetAccess::download(url, target, window)) {
        QString msg;
        bool result = readKeyboard(target, msg);
        KIO::NetAccess::removeTempFile(target);
        if (!result && errorMsg!=NULL)
            *errorMsg = i18n("Could not read the keyboard layout file '%1'. ").arg(url.url()) + msg;
        return result;
    }
    else {
        if (errorMsg!=NULL)
            *errorMsg = i18n("Could not download/open keyboard layout file from '%1'.").arg(url.url());
        return false;
    }
}


void KTouchKeyboardWidget::saveKeyboard(QWidget * window, const KURL& url) {
    QString tmpFile;
    KTempFile *temp=0;
    if (url.isLocalFile())
        tmpFile=url.path();             // for local files the path is sufficient
    else {
        temp=new KTempFile;             // for remote files create a temporary file first
        tmpFile=temp->name();
    }

    QFile outfile(tmpFile);
    if ( !outfile.open( IO_WriteOnly ) ) {
        if (temp)  delete temp;
        return;
    }

    QTextStream out( &outfile );
    out << "########################################## \n";
    out << "#                                        # \n";
    out << "#  Keyboard layout file for KTouch       # \n";
    out << "#                                        # \n";
    out << "########################################## \n";
    out << "#\n";
    out << endl;

    for (KTouchBaseKey * key = m_keyList.first(); key; key = m_keyList.next()) {
        switch (key->type()) {
           case KTouchBaseKey::FINGER_KEY   : out << "FingerKey  "; break;
           case KTouchBaseKey::NORMAL_KEY   : out << "NormalKey  "; break;
           case KTouchBaseKey::CONTROL_KEY  : out << "ControlKey "; break;
           default : out << "NormalKey  "; break;
        }
        QRect rect=key->frame();
        out << key->m_keyChar.unicode() << '\t' << key->m_keyText << '\t'
            << rect.left() << '\t' << rect.top() << '\t' << rect.width() << '\t' << rect.height() << endl;
    }

    if (temp) {
        KIO::NetAccess::upload(tmpFile, url, window);
        temp->unlink();
        delete temp;
    }
}

void KTouchKeyboardWidget::applyPreferences(QWidget * window, bool silent) {
	m_hideKeyboard = Prefs::hideKeyboard();
	if (m_hideKeyboard) 
    	setMaximumHeight(100);
	else
    	setMaximumHeight(10000);
    // let's check whether the keyboard layout has changed
    if (Prefs::currentKeyboardFile() != m_currentLayout) {
        // if the layout is the number layout just create it and we're done
		//kdDebug() << "[KTouchKeyboardWidget::applyPreferences]  keyboard = " << Prefs::currentKeyboardFile() << endl;
        if (Prefs::currentKeyboardFile()=="number.keyboard") {
            createDefaultKeyboard();
            resizeEvent(NULL);
            return;
        }
        // ok, let's load this layout
        if (silent) {
            // during initialisation we don't want to have a message box, that's why this is silent
            if (!loadKeyboard(window, KURL::fromPathOrURL( Prefs::currentKeyboardFile() )))
                createDefaultKeyboard();
            else
                m_currentLayout=Prefs::currentKeyboardFile();
        }
        else {
            QString errorMsg;
            if (!loadKeyboard(window, KURL::fromPathOrURL( Prefs::currentKeyboardFile() ), &errorMsg)) {
                KMessageBox::error( 0, i18n("Error reading the keyboard layout; the default number keypad will "
                    "be created instead. You can choose another keyboard layout in the preferences dialog."),
                    errorMsg);
                createDefaultKeyboard();
            }
            else
                m_currentLayout=Prefs::currentKeyboardFile();
        }
    }
    updateColours();    // we recreate the colour connections
	// assign keyboard font to keys
    for (KTouchBaseKey * key = m_keyList.first(); key; key = m_keyList.next()) {
		if (Prefs::overrideKeyboardFont())
			key->m_font = Prefs::keyboardFont();
		else
			key->m_font = Prefs::font();
	}
	// kdDebug() << "[KTouchKeyboard::applyPreferences]  Assigned key font" << endl;
    resizeEvent(NULL);  // paint the keyboard
    newKey(m_nextKey);  // and finally display the "next to be pressed" key again
}


void KTouchKeyboardWidget::newKey(const QChar& nextChar) {
	if (m_hideKeyboard) return;
    QPainter painter(this);
    painter.translate(m_shift, MARGIN);
    // first clean the markings on all keys
    for (KTouchBaseKey * key = m_keyList.first(); key; key = m_keyList.next()) {
        if (key->m_isActive || key->m_isNextKey) {
            key->m_isActive=key->m_isNextKey=false;
            key->paint(painter);
        }
    }

    if (Prefs::showAnimation()){ // only do this if we want to show animation.
        // find the key in the key connector list
        QValueList<KTouchKeyConnection>::iterator keyIt = m_connectorList.begin();
        while (keyIt!=m_connectorList.end() && (*keyIt).m_keyChar!=nextChar)  ++keyIt;
        // if found mark the appropriate keys
        if (keyIt!=m_connectorList.end()) {
            QChar targetChar = (*keyIt).m_targetKeyChar;
            QChar fingerChar = (*keyIt).m_fingerKeyChar;
            QChar controlChar = (*keyIt).m_controlKeyChar;
            // find the keys in the keylist
            for (KTouchBaseKey * key = m_keyList.first(); key; key = m_keyList.next()) {
                if (key->m_keyChar==QChar(0)) continue;    // skip decorative keys
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


void KTouchKeyboardWidget::paintEvent(QPaintEvent *) {
	if (m_hideKeyboard) return;
    QPainter p(this);
    p.translate(m_shift, MARGIN);
    // just print all visible keys
    for (KTouchBaseKey * key = m_keyList.first(); key; key = m_keyList.next())
        key->paint(p);

/*	// TODO : later

	const KTouchColorScheme& colorScheme = KTouchColorScheme::m_colorSchemes[Prefs::colorScheme()];
	for (QValueVector<KTouchKey>::iterator it = m_keys.begin(); it != m_keys.end(); ++it) {
		// determine colors
    	QColor textColor;
		if (it->m_type == KTouchKey::NORMAL || it->m_type == KTouchKey::FINGER) {
			if (is_next_key) {
				// mark the key as "next"
				p.setBrush( colorScheme.m_backgroundH );
				p.setPen( colorScheme.m_frame );
				textColor = colorScheme.m_textH;
			}
			else {
				p.setBrush( colorScheme.m_background[m_colorIndex] );
				p.setPen( colorScheme.m_frame );
				textColor = colorScheme.m_text;
			};
		}
		else {
    		p.setBrush( colorScheme.m_cBackground );
			p.setPen( colorScheme.m_cText );
		}

    	p.setPen(textColor);
    	p.fillRect(m_xS, m_xS, m_xS, m_xS, p.brush());
    	p.drawRect(m_xS, m_xS, m_xS, m_xS);

    	p.setFont( m_font );
    	p.drawText(it->m_xS, it->m_yS, it->m_wS, it->m_hS,
			QPainter::AlignCenter, m_keyText);

	}
*/
	// TODO : later copy pre-rendered and pre-scaled characters to screen
}

void KTouchKeyboardWidget::resizeEvent(QResizeEvent *) {
	// kdDebug() << "[KTouchKeyboard::resizeEvent]  Window = " << width() << "x" << height() << endl;
	// kdDebug() << "[KTouchKeyboard::resizeEvent]  Keyboard = " << m_keyboardWidth << "x" << m_keyboardHeight << endl;
    double hScale = static_cast<double>(width()-2*MARGIN)/m_keyboardWidth;
    double vScale = static_cast<double>(height()-2*MARGIN)/m_keyboardHeight;
    double scale = std::max(1.0, std::min(hScale, vScale));
	// kdDebug() << "[KTouchKeyboard::resizeEvent]  using scale = " << scale << endl;
    m_shift = (width() - static_cast<int>(m_keyboardWidth*scale))/2;
    for (KTouchBaseKey * key = m_keyList.first(); key; key = m_keyList.next())
        key->resize(scale);     // resize all keys
	for (QValueVector<KTouchKey>::iterator it = m_keys.begin(); it != m_keys.end(); ++it) {
        it->resize(scale);     // resize all keys
	}
    update();                   // and finally redraw the keyboard
}


void KTouchKeyboardWidget::createDefaultKeyboard() {
    // let's create a default keyboard
    int keySpacing = 4;
    int keyHeight = 20;
    int keyWidth = 20;
    int col = keyWidth+keySpacing;
    int row = keyHeight+keySpacing;
    // first let's create the "visible" keys, that means all keys that will be displayed
    // Note: purely decorative keys get a key char code of 0!
    m_keyList.clear();
    m_keyList.append( new KTouchControlKey( 0, "Num",       0,     0, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '/', "/",       col,     0, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '*', "*",     2*col,     0, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '-', "-",     3*col,     0, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '7', "7",         0,   row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '8', "8",       col,   row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '9', "9",     2*col,   row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchFingerKey( '4', "4",         0, 2*row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchFingerKey( '5', "5",       col, 2*row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchFingerKey( '6', "6",     2*col, 2*row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '1', "1",         0, 3*row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '2', "2",       col, 3*row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '3', "3",     2*col, 3*row, keyWidth, keyHeight) );
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
    m_connectorList.append( KTouchKeyConnection('/', '/', '5', 0) );
    m_connectorList.append( KTouchKeyConnection('*', '*', '6', 0) );
    m_connectorList.append( KTouchKeyConnection('-', '-', '+', 0) );
    m_connectorList.append( KTouchKeyConnection('+', '+',   0, 0) );
    m_connectorList.append( KTouchKeyConnection('.', '.', '6', 0) );
    m_connectorList.append( KTouchKeyConnection(',', '.', '6', 0) );
    m_connectorList.append( KTouchKeyConnection('7', '7', '4', 0) );
    m_connectorList.append( KTouchKeyConnection('8', '8', '5', 0) );
    m_connectorList.append( KTouchKeyConnection('9', '9', '6', 0) );
    m_connectorList.append( KTouchKeyConnection('4', '4',   0, 0) );
    m_connectorList.append( KTouchKeyConnection('5', '5',   0, 0) );
    m_connectorList.append( KTouchKeyConnection('6', '6',   0, 0) );
    m_connectorList.append( KTouchKeyConnection('1', '1', '4', 0) );
    m_connectorList.append( KTouchKeyConnection('2', '2', '5', 0) );
    m_connectorList.append( KTouchKeyConnection('3', '3', '6', 0) );
    m_connectorList.append( KTouchKeyConnection('0', '0',   0, 0) );
    m_connectorList.append( KTouchKeyConnection(  8,   8,   0, 0) );
    m_connectorList.append( KTouchKeyConnection( 13,  13, '+', 0) );
    updateColours();
    m_currentLayout="number.keyboard";

	// create keyboard geometry for new keyboard data

    int sp = 10;
    int h = 50;
    int w = 50;
    col = w+sp;
    row = h+sp;

	m_keys.clear();
	m_keys.append( KTouchKey(                           0,     0, w, h, QString("Num")) );		// 0
	m_keys.append( KTouchKey(KTouchKey::NORMAL,   	  col,     0, w, h, '/') );			// 1
	m_keys.append( KTouchKey(KTouchKey::NORMAL, 	2*col,     0, w, h, '*') );			// 2
	m_keys.append( KTouchKey(KTouchKey::NORMAL, 	3*col,     0, w, h, '-') );			// 3
	m_keys.append( KTouchKey(KTouchKey::NORMAL,     	0,   row, w, h, '7') );			// 4
	m_keys.append( KTouchKey(KTouchKey::NORMAL,   	  col,   row, w, h, '8') );			// 5
	m_keys.append( KTouchKey(KTouchKey::NORMAL, 	2*col,   row, w, h, '9') );			// 6
	m_keys.append( KTouchKey(KTouchKey::FINGER,     	0, 2*row, w, h, '4') );			// 7  ***
	m_keys.append( KTouchKey(KTouchKey::FINGER,   	  col, 2*row, w, h, '5') );			// 8  ***
	m_keys.append( KTouchKey(KTouchKey::FINGER, 	2*col, 2*row, w, h, '6') );			// 9  ***
	m_keys.append( KTouchKey(KTouchKey::FINGER, 	3*col,   row, w, 2*h+sp, '+') ); 	// 10 ***
	m_keys.append( KTouchKey(KTouchKey::NORMAL,     	0, 3*row, w, h, '1') );			// 11
	m_keys.append( KTouchKey(KTouchKey::NORMAL,   	  col, 3*row, w, h, '2') );			// 12
	m_keys.append( KTouchKey(KTouchKey::NORMAL, 	2*col, 3*row, w, h, '3') );			// 13
	m_keys.append( KTouchKey(KTouchKey::NORMAL,     	0, 4*row, 2*w+sp, h, '0') );	// 14
	m_keys.append( KTouchKey(KTouchKey::NORMAL,   	2*col, 4*row, w, h, '.') );			// 15
	m_keys.append( KTouchKey(KTouchKey::ENTER,  	3*col, 3*row, w, 2*h+sp, 0) ); 		// 16
	m_keys.append( KTouchKey(KTouchKey::BACKSPACE,  5*col, 0, 2*w+sp, h, 0) ); 			// 17
	// number the keys for reference
	for (unsigned int i=0; i<m_keys.size(); ++i)
		m_keys[i].m_number = i;
	// create key connections
	m_keyConnections.clear();
	// KTouchKeyConnector(char, target, finger, modifier)
	m_keyConnections.append( KTouchKeyConnector('/',  1,  8, 0) );
	m_keyConnections.append( KTouchKeyConnector('*',  2,  9, 0) );
	m_keyConnections.append( KTouchKeyConnector('-',  3, 10, 0) );
	m_keyConnections.append( KTouchKeyConnector('7',  4,  7, 0) );
	m_keyConnections.append( KTouchKeyConnector('8',  5,  8, 0) );
	m_keyConnections.append( KTouchKeyConnector('9',  6,  9, 0) );
	m_keyConnections.append( KTouchKeyConnector('4',  7,  7, 0) ); // ***
	m_keyConnections.append( KTouchKeyConnector('5',  8,  8, 0) ); // ***
	m_keyConnections.append( KTouchKeyConnector('6',  9,  9, 0) ); // ***
	m_keyConnections.append( KTouchKeyConnector('+', 10, 10, 0) ); // ***
	m_keyConnections.append( KTouchKeyConnector('1', 11,  7, 0) );
	m_keyConnections.append( KTouchKeyConnector('2', 12,  8, 0) );
	m_keyConnections.append( KTouchKeyConnector('3', 13,  9, 0) );
	m_keyConnections.append( KTouchKeyConnector('0', 14,  7, 0) );
	m_keyConnections.append( KTouchKeyConnector('.', 15,  9, 0) );
	m_keyConnections.append( KTouchKeyConnector(QChar(13), 16, 10, 0) );
	m_keyConnections.append( KTouchKeyConnector(QChar(8), 17, 0, 0) );
}


bool KTouchKeyboardWidget::readKeyboard(const QString& fileName, QString& errorMsg) {
    QFile infile(fileName);
    if ( !infile.open( IO_ReadOnly ) ) {
        errorMsg = i18n("Could not open file.");
        return false;
    }
    QTextStream in( &infile );
    in.setEncoding(QTextStream::UnicodeUTF8);
    QString line;
    m_keyList.clear();          // empty the keyboard
    m_connectorList.clear();    // clear the connections
    m_keyboardWidth=0;
    m_keyboardHeight=0;
	std::set<QChar>  keys;
    // now loop until end of file is reached
    do {
        // skip all empty lines or lines containing a comment (starting with '#')
        do {  line = in.readLine().stripWhiteSpace();  }
        while (!line.isNull() && (line.isEmpty() || line[0]=='#'));
        // Check if end of file encountered and if that is the case -> bail out at next while
        if (line.isNull())  continue;

        // 'line' should now contain a key specification
        QTextStream lineStream(line, IO_ReadOnly);
        QString keyType;
        int keyAscII;
        QString keyText;
        int x(0), y(0), w(0), h(0);
        lineStream >> keyType >> keyAscII;
        if (keyType=="FingerKey") {
            lineStream >> keyText >> x >> y >> w >> h;
            if (w==0 || h==0)
                w=h=8; // default values for old keyboard files
            m_keyList.append( new KTouchFingerKey(keyAscII, keyText, x+1, y+1, w, h) );
            m_connectorList.append( KTouchKeyConnection(keyAscII, keyAscII, 0, 0) );
// 			kdDebug() << "read : F : unicode = " << keyAscII << " char = " << QChar(keyAscII) << endl;
        }
        else if (keyType=="ControlKey") {
            lineStream >> keyText >> x >> y >> w >> h;
            m_keyList.append( new KTouchControlKey(keyAscII, keyText, x+1, y+1, w-2, h-2) );
            m_connectorList.append( KTouchKeyConnection(keyAscII, keyAscII, 0, 0) );
// 			kdDebug() << "read : C : unicode = " << keyAscII << " char = " << keyText << endl;
        }
        else if (keyType=="NormalKey") {
            int fingerCharCode;
            lineStream >> keyText >> x >> y >> fingerCharCode;
            w=h=8; // default values for old keyboard files
            // retrieve the finger key with the matching char
            m_keyList.append( new KTouchNormalKey(keyAscII, keyText, x+1, y+1, w, h) );
            m_connectorList.append( KTouchKeyConnection(keyAscII, keyAscII, fingerCharCode, 0) );
// 			kdDebug() << "read : N : unicode = " << keyAscII << " char = " << QChar(keyAscII) << endl;
        } else if (keyType=="HiddenKey") {
            int targetChar, fingerChar, controlChar;
            lineStream >> targetChar >> fingerChar >> controlChar;
            m_connectorList.append( KTouchKeyConnection(keyAscII, targetChar, fingerChar, controlChar) );
//			kdDebug() << "read : H : unicode = " << keyAscII << " char = " << QChar(keyAscII) << " target = " << targetChar << " finger = " << fingerChar << " control = " << controlChar << endl;
			
        }
        else {
            errorMsg = i18n("Missing key type in line '%1'.").arg(line);
            return false;
        }
		if (keys.find(keyAscII)!=keys.end()) {
			kdDebug() << "WARNING: Duplicate entry for char '"+keyText+"' with Unicode " << keyAscII << endl;
		}
		else
			keys.insert(keyAscII);

		keys.insert(keyAscII);


        // calculate the maximum extent of the keyboard on the fly...
        m_keyboardWidth = std::max(m_keyboardWidth, x+w);
        m_keyboardHeight = std::max(m_keyboardHeight, y+h);
    } while (!in.atEnd() && !line.isNull());
//	kdDebug() << "showing all unicode numbers in this file" << endl;
/*	for (std::set<QChar>::iterator it = keys.begin(); it != keys.end(); ++it)
		kdDebug() << *it << endl;
*/
//	kdDebug() << "num chars = " << keys.size() << endl;
//	kdDebug() << "num key connections = " << m_connectorList.size() << endl;

    updateColours();
    return (!m_keyList.isEmpty());  // empty file means error
}


void KTouchKeyboardWidget::updateColours() {
//	kdDebug() << "KTouchKeyboardWidget::updateColours()" << endl;
    // old functionality : loop over all key connections
	m_keyCharMap.clear();
	unsigned int counter = 0;
    for (QValueList<KTouchKeyConnection>::iterator it = m_connectorList.begin(); it!=m_connectorList.end(); ++it) {
		// store finger and target characters
        QChar fingerChar = (*it).m_fingerKeyChar;
        QChar targetChar = (*it).m_targetKeyChar; // this is the _base_ character of the key that needs to be highlighted
        QChar ch = (*it).m_keyChar;

/*		kdDebug() << "Key #"<<++counter<<": " << ch << "(" << ch.unicode() << ") " 
				  << "target = " << targetChar << "(" << targetChar.unicode() << ") " 
				  << "finger = " << fingerChar << "(" << fingerChar.unicode() << ")" << endl;
*/
		m_keyCharMap[ch] = -1;

        if (fingerChar == QChar(0)) {
//			kdDebug() << "skipped char = " << targetChar << endl;
			continue; // skips keys that don't have finger keys assigned
		}

        KTouchBaseKey * self=NULL;
        KTouchBaseKey * colorSource=NULL;
        // loop over all keys to find the key pointers
        for (KTouchBaseKey * key = m_keyList.first(); key; key = m_keyList.next()) {
            if (key->m_keyChar==targetChar) self=key;
            else if (key->m_keyChar==fingerChar) colorSource=key;
        }
		// ok, by now we should have found a finger and target char pointer
        if (self && colorSource) {
// 			kdDebug() << "Key " << ++counter << " keychar = " << ch << " target = " << targetChar << "  finger = " << fingerChar << endl;
			// skip target keys that are actually control, finger or hidden keys
            if (self->type()!=KTouchBaseKey::NORMAL_KEY)  continue;
			// try to downcast to get a normal key pointer
            KTouchNormalKey *nk = dynamic_cast<KTouchNormalKey*>(self);
			// skip if we couldn't downcast
			if (!nk) {
				kdDebug() << "Couldn't downcast to normal key for targer char = " << targetChar << endl;
				continue;
			}
			// if our color source is not a valid finger key, skip with a warning
            if (colorSource->type()!=KTouchBaseKey::FINGER_KEY) {
                kdDebug() << "[KTouchKeyboard::updateColours]  Colour source key '" << colorSource->m_keyText
                          << "' is not a finger key!" << endl;
				continue;
			}
			// finally store the color code
            KTouchFingerKey *fk = dynamic_cast<KTouchFingerKey*>(colorSource);
            if (fk) {
				nk->m_colorIndex = fk->m_colorIndex;
				m_keyCharMap[ch] = fk->m_colorIndex;
//				kdDebug() << "char = " << targetChar << " index = " << fk->m_colorIndex << endl;
            }
        }
    }
//	kdDebug() << "Colormap has " << m_keyCharMap.count() << " entries" << endl;

/*
	// new functionality
	m_keyMap.clear();
	m_colorMap.clear();
	for (unsigned int i=0; i<m_keys.size(); ++i)
		m_keyMap[i] = -1;
	int c_index = 0;
    for (QValueVector<KTouchKeyConnector>::iterator it = m_keyConnections.begin();
		it!=m_keyConnections.end(); ++it)
	{
		if (it->m_targetKeyIndex == -1) continue;
		int n = it->m_targetKeyIndex;
		switch (m_keys[n].m_type) {
			case KTouchKey::NORMAL : m_keyMap[n] = it->m_fingerKeyIndex; break;
			case KTouchKey::FINGER : m_keyMap[n] = it->m_fingerKeyIndex; 
									 m_colorMap[n] = c_index++;
									 c_index %= 8; break;
			default : ;
		}
	}
*/
}

