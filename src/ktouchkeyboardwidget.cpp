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

#include <QtGui>

#include <kdebug.h>
#include <ktemporaryfile.h>
#include <klocale.h>
#include <kio/netaccess.h>
#include <kstandarddirs.h>
#include <kmessagebox.h>

// #define USE_NEW_KEYBOARD

#include "prefs.h"

KTouchKeyboardWidget::KTouchKeyboardWidget(QWidget *parent)
  : QGraphicsView(parent), m_currentLayout("")
{
    setMinimumHeight(100);          // when it's smaller you won't see anything

    scene = new QGraphicsScene(this);

    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing);

    setBackgroundBrush(palette().brush(QPalette::Window));
    setFrameStyle(QFrame::NoFrame);
}
// --------------------------------------------------------------------------

KTouchKeyboardWidget::~KTouchKeyboardWidget() {
	qDeleteAll(m_keyList);
	m_keyList.clear();
}
// --------------------------------------------------------------------------

bool KTouchKeyboardWidget::loadKeyboard(QWidget * window, const KUrl& url, QString* errorMsg) {
    reset();

    QString target;
    if (KIO::NetAccess::download(url, target, window)) {
        QString msg;
        bool result = readKeyboard(target, msg);
        KIO::NetAccess::removeTempFile(target);
        if (!result && errorMsg!=NULL)
            *errorMsg = i18n("Could not read the keyboard layout file '%1'. ", url.url()) + msg;
        return result;
    }
    else {
        if (errorMsg!=NULL)
            *errorMsg = i18n("Could not download/open keyboard layout file from '%1'.", url.url());
        return false;
    }
}
// --------------------------------------------------------------------------


void KTouchKeyboardWidget::applyPreferences(QWidget * window, bool silent) {
    setVisible(!Prefs::hideKeyboard());

    // let's check whether the keyboard layout has changed
    if (Prefs::currentKeyboardFile() != m_currentLayout) {
        // if the layout is the number layout just create it and we're done
		//kDebug() << "[KTouchKeyboardWidget::applyPreferences]  keyboard = " << Prefs::currentKeyboardFile() << endl;
        if (Prefs::currentKeyboardFile()=="number.keyboard") {
            createDefaultKeyboard();
            return;
        }
        // ok, let's load this layout
        if (silent) {
            // during initialisation we don't want to have a message box, that's why this is silent
            if (!loadKeyboard(window, KUrl( Prefs::currentKeyboardFile() )))
                createDefaultKeyboard();
            else
                m_currentLayout=Prefs::currentKeyboardFile();
        }
        else {
            QString errorMsg;
            if (!loadKeyboard(window, KUrl( Prefs::currentKeyboardFile() ), &errorMsg)) {
                KMessageBox::error( 0, i18n("Error reading the keyboard layout; the default number keypad will "
                    "be created instead. You can choose another keyboard layout in the preferences dialog."),
                    errorMsg);
                createDefaultKeyboard();
            }
            else
                m_currentLayout=Prefs::currentKeyboardFile();
        }
    }

    // assign keyboard font to keys
    for (QList<KTouchBaseKey*>::iterator it = m_keyList.begin(); it != m_keyList.end(); ++it) {

        if (Prefs::overrideKeyboardFont()) {
            (*it)->m_font = Prefs::keyboardFont();
        }
        else  {
            (*it)->m_font = Prefs::font();
        }

        (*it)->m_font.setPointSizeF(8/(*it)->m_font_scale);

        (*it)->update();
    }

    newKey(m_nextKey);  // and finally display the "next to be pressed" key again
}
// --------------------------------------------------------------------------

void KTouchKeyboardWidget::newKey(const QChar& nextChar) {
    // store next to be pressed character, the corresponding key and
    // the finger key will be highlighted
    m_nextKey = nextChar;

    if(Prefs::hideKeyboard())
        return;

    // first clean the markings on all keys
    for (QList<KTouchBaseKey*>::iterator it = m_keyList.begin(); it != m_keyList.end(); ++it) {
        KTouchBaseKey * key = *it;
        if (key->m_isActive || key->m_isNextKey) {
            key->m_isActive = key->m_isNextKey = false;
            key->update();
        }
    }

    if (Prefs::showAnimation()){ // only do this if we want to show animation.
        // find the key in the key connector list
        QList<KTouchKeyConnection>::iterator keyIt = m_connectorList.begin();
        while (keyIt!=m_connectorList.end() && (*keyIt).m_keyChar!=m_nextKey)  ++keyIt;
        // if found mark the appropriate keys
        if (keyIt!=m_connectorList.end()) {
            QChar targetChar = (*keyIt).m_targetKeyChar;
            QChar fingerChar = (*keyIt).m_fingerKeyChar;
            QChar controlChar = (*keyIt).m_controlKeyChar;
            // find the keys in the keylist
            for (QList<KTouchBaseKey*>::iterator it = m_keyList.begin(); it != m_keyList.end(); ++it) {
                KTouchBaseKey * key = *it;
                if (key->m_keyChar==QChar(0)) continue;    // skip decorative keys
                if (key->m_keyChar==targetChar) key->m_isNextKey=true;
                else if (key->m_keyChar==fingerChar)   key->m_isActive=true;
                else if (key->m_keyChar==controlChar)  key->m_isActive=true;
                if (key->m_isActive || key->m_isNextKey){
                    key->update();
                }
            }
        }
    }
}

void KTouchKeyboardWidget::resizeEvent(QResizeEvent *) {
    QRectF sbr = scene->itemsBoundingRect();
    qreal scale = qMin(width()/sbr.width(),height()/sbr.height()) * 0.9;

    QMatrix matrix;
    matrix.scale(scale, scale);
    setMatrix(matrix);
}

void KTouchKeyboardWidget::reset() {
    m_keyList.clear();

    setScene(0);
    delete(scene);
    scene = new QGraphicsScene(this);

    setScene(scene);
}

void KTouchKeyboardWidget::createDefaultKeyboard() {
    reset();

    // let's create a default keyboard
    const int keySpacing = 4;
    const int keyHeight = 20;
    const int keyWidth = 20;
    int col = keyWidth+keySpacing;
    int row = keyHeight+keySpacing;
    // first let's create the "visible" keys, that means all keys that will be displayed
    // Note: purely decorative keys get a key char code of 0!
    m_keyList.clear();
    m_keyList.append( new KTouchFingerKey( '4', "4", 0,         0, 2*row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchFingerKey( '5', "5", 1,       col, 2*row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchFingerKey( '6', "6", 2,     2*col, 2*row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchFingerKey( '+', "+", 3,     3*col,   row, keyWidth, 2*keyHeight+keySpacing) );
    m_keyList.append( new KTouchFingerKey( '.', ".", 4,     2*col, 4*row, keyWidth, keyHeight) );

    m_keyList.append( new KTouchNormalKey( '/', "/", 1,       col,     0, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '*', "*", 2,     2*col,     0, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '-', "-", 3,     3*col,     0, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '7', "7", 0,         0,   row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '8', "8", 1,       col,   row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '9', "9", 2,     2*col,   row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '1', "1", 0,         0, 3*row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '2', "2", 1,       col, 3*row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '3', "3", 2,     2*col, 3*row, keyWidth, keyHeight) );
    m_keyList.append( new KTouchNormalKey( '0', "0", 0,         0, 4*row, 2*keyWidth+keySpacing, keyHeight) );

    m_keyList.append( new KTouchControlKey(13, "Enter", 3*col, 3*row,keyWidth, 2*keyHeight+keySpacing) );
    m_keyList.append( new KTouchControlKey(8, "BackSpace", 5*col, 0, 2*keyWidth+keySpacing, keyHeight) );

    for (QList<KTouchBaseKey*>::iterator it = m_keyList.begin(); it != m_keyList.end(); ++it) {
        KTouchBaseKey * key = *it;
        scene->addItem(key);
    }

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

    m_currentLayout="number.keyboard";

    resizeEvent(0);
/*

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
	for (int i=0; i<m_keys.size(); ++i)
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
*/
}
// --------------------------------------------------------------------------

bool KTouchKeyboardWidget::readKeyboard(const QString& fileName, QString& errorMsg) {
    QFile infile(fileName);
    if ( !infile.open( QIODevice::ReadOnly ) ) {
        errorMsg = i18n("Could not open file.");
        return false;
    }
    QTextStream in( &infile );
    QString line;
    m_keyList.clear();          // empty the keyboard
    m_connectorList.clear();    // clear the connections
    int colorIndex(0);
    QSet<QChar>  keys;
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

        KTouchBaseKey *key;

        if (keyType=="FingerKey") {
            lineStream >> keyText >> x >> y >> w >> h;
            if (w==0 || h==0)
                w=h=8; // default values for old keyboard files

            key = new KTouchFingerKey(keyAscII, keyText, colorIndex, x+1, y+1, w, h);
            colorIndex ++;
            scene->addItem(key);
            m_keyList.append( key );
            m_connectorList.append( KTouchKeyConnection(keyAscII, keyAscII, 0, 0) );
// 			kDebug() << "read : F : unicode = " << keyAscII << " char = " << QChar(keyAscII) << endl;
        }
        else if (keyType=="ControlKey") {
            lineStream >> keyText >> x >> y >> w >> h;
            key = new KTouchControlKey(keyAscII, keyText, x+1, y+1, w-2, h-2);
            scene->addItem(key);
            m_keyList.append( key );
            m_connectorList.append( KTouchKeyConnection(keyAscII, keyAscII, 0, 0) );
// 			kDebug() << "read : C : unicode = " << keyAscII << " char = " << keyText << endl;
        }
        else if (keyType=="NormalKey") {
            int fingerCharCode;
            lineStream >> keyText >> x >> y >> fingerCharCode;
            w=h=8; // default values for old keyboard files
            // retrieve the finger key with the matching char

            int colorIndexForFingerKey = 0;
            for (QList<KTouchBaseKey*>::iterator it = m_keyList.begin(); it != m_keyList.end(); ++it) {
                KTouchBaseKey *other = *it;
                if (other->m_keyChar == fingerCharCode){
                    colorIndexForFingerKey = other->m_colorIndex;
                }
            }

            key = new KTouchNormalKey(keyAscII, keyText, colorIndexForFingerKey, x+1, y+1, w, h);

            scene->addItem(key);

            m_keyList.append( key );

            m_connectorList.append( KTouchKeyConnection(keyAscII, keyAscII, fingerCharCode, 0) );
        } else if (keyType=="HiddenKey") {
            int targetChar, fingerChar, controlChar;
            lineStream >> targetChar >> fingerChar >> controlChar;
            m_connectorList.append( KTouchKeyConnection(keyAscII, targetChar, fingerChar, controlChar) );

        }
        else {
            errorMsg = i18n("Missing key type in line '%1'.", line);
            return false;
        }

        if (keys.find(keyAscII)!=keys.end()) {
            kDebug() << "WARNING: Duplicate entry for char '"+keyText+"' with Unicode " << keyAscII << endl;
        }
        else
            keys.insert(keyAscII);

        keys.insert(keyAscII);

    } while (!in.atEnd() && !line.isNull());

    resizeEvent(0);

    return (!m_keyList.isEmpty());  // empty file means error
}



/*
removed functions

void KTouchKeyboardWidget::saveKeyboard(QWidget * window, const KUrl& url) {
    QString tmpFile;
    KTemporaryFile *temp=0;
    if (url.isLocalFile())
        tmpFile=url.path();             // for local files the path is sufficient
    else {
        temp=new KTemporaryFile;             // for remote files create a temporary file first
        temp->open();
        tmpFile=temp->fileName();
    }

    QFile outfile(tmpFile);
    if ( !outfile.open( QIODevice::WriteOnly ) ) {
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

	for (QList<KTouchBaseKey*>::iterator it = m_keyList.begin(); it != m_keyList.end(); ++it) {
        switch ((*it)->type()) {
           case KTouchBaseKey::FINGER_KEY   : out << "FingerKey  "; break;
           case KTouchBaseKey::NORMAL_KEY   : out << "NormalKey  "; break;
           case KTouchBaseKey::CONTROL_KEY  : out << "ControlKey "; break;
           default : out << "NormalKey  "; break;
        }
        QRect rect=(*it)->frame();
        out << (*it)->m_keyChar.unicode() << '\t' << (*it)->m_keyText << '\t'
            << rect.left() << '\t' << rect.top() << '\t' << rect.width() << '\t' << rect.height() << endl;
    }

    if (temp) {
        KIO::NetAccess::upload(tmpFile, url, window);
        delete temp;
    }
}
// --------------------------------------------------------------------------
*/
