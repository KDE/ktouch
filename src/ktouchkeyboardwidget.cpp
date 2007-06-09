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

#include <kdebug.h>
#include <ktemporaryfile.h>
#include <klocale.h>
#include <kio/netaccess.h>
#include <kstandarddirs.h>
#include <kmessagebox.h>

#include "prefs.h"
#include "ktouchkeyboard.h"
#include "ktouchkey.h"

KTouchKeyboardWidget::KTouchKeyboardWidget(QWidget *parent)
  : QGraphicsView(parent), m_currentLayout("")
{
    setMinimumHeight(100);          // when it's smaller you won't see anything

	// create keyboard object
	m_keyboard = new KTouchKeyboard(this);

	// setup graphics view
    m_scene = new QGraphicsScene(this);
    setScene(m_scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::TextAntialiasing);
    setBackgroundBrush(palette().brush(QPalette::Window));
    setFrameStyle(QFrame::NoFrame);
}
// --------------------------------------------------------------------------

KTouchKeyboardWidget::~KTouchKeyboardWidget() {
	// TODO : cleanup key graphics items
}
// --------------------------------------------------------------------------

bool KTouchKeyboardWidget::loadKeyboard(QWidget * window, const KUrl& url, QString& errorMsg) {
    reset();

    QString target;
    if (KIO::NetAccess::download(url, target, window)) {
        QString msg;
        bool result = true; //readKeyboard(target, msg);
        KIO::NetAccess::removeTempFile(target);
        if (!result)
			errorMsg = i18n("Could not read the keyboard layout file '%1'. %2", url.url(), msg);
        return result;
    }
    else {
		errorMsg = i18n("Could not download/open keyboard layout file from '%1'.", url.url());
        return false;
    }
}
// --------------------------------------------------------------------------

void KTouchKeyboardWidget::applyPreferences(QWidget * window, bool silent) {
    setVisible(Prefs::showKeyboard());

    // let's check whether the keyboard layout has changed
    if (Prefs::currentKeyboardFile() != m_currentLayout) {
        // if the layout is the number layout just create it and we're done
		//kDebug() << "[KTouchKeyboardWidget::applyPreferences]  keyboard = " << Prefs::currentKeyboardFile() << endl;
        if (Prefs::currentKeyboardFile()=="number.keyboard") {
            createDefaultKeyboard();
            return;
        }
        // ok, let's load this layout
        QString errorMsg;
        if (silent) {
            if (!loadKeyboard(window, KUrl( Prefs::currentKeyboardFile() ), errorMsg)) {
            	// during initialisation we don't want to have a message box, that's why we
				// simply ignore the error message here
                createDefaultKeyboard();
			}
            else
                m_currentLayout=Prefs::currentKeyboardFile();
        }
        else {
            if (!loadKeyboard(window, KUrl( Prefs::currentKeyboardFile() ), errorMsg)) {
                KMessageBox::error( 0, i18n("Error reading the keyboard layout; the default number keypad will "
                    "be created instead. You can choose another keyboard layout in the preferences dialog."),
                    errorMsg);
                createDefaultKeyboard();
            }
            else
                m_currentLayout=Prefs::currentKeyboardFile();
        }
    }

    // assign keyboard font
	if (Prefs::overrideKeyboardFont()) {
		m_keyboard->setFont(Prefs::keyboardFont());
	}
	else  {
		m_keyboard->setFont(Prefs::font());
	}

    newKey(m_nextKey);  // and finally display the "next to be pressed" key again
}
// --------------------------------------------------------------------------

void KTouchKeyboardWidget::newKey(const QChar& nextChar) {
    // store next to be pressed character, the corresponding key and
    // the finger key will be highlighted
    m_nextKey = nextChar;

    if(!Prefs::showKeyboard())
        return;

    // first clean the markings on all keys
	for(QList<KTouchKey*>::iterator it = m_keyboard->m_keys.begin(); it != m_keyboard->m_keys.end(); ++it )
		(*it)->m_state = KTouchKey::NormalState;

    if (Prefs::showAnimation()){ // only do this if we want to show animation.
        // find the key in the key connector list
/*        QList<KTouchKeyConnection>::iterator keyIt = m_connectorList.begin();
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
                if (key->m_keyChar==targetChar) key->setNextKey(true);
                else if (key->m_keyChar==fingerChar) key->setActive(true);
                else if (key->m_keyChar==controlChar)  key->setActive(true);
            }
        }
*/
    }
}
// --------------------------------------------------------------------------

void KTouchKeyboardWidget::resizeEvent(QResizeEvent *) {
    QRectF sbr = m_scene->itemsBoundingRect();
	if (sbr.width() <= 0)  sbr.setWidth(1);
	if (sbr.height() <= 0)  sbr.setHeight(1);
    qreal scale = qMin(width()/sbr.width(),height()/sbr.height()) * 0.9;

    QMatrix matrix;
    matrix.scale(scale, scale);
    setMatrix(matrix);
}
// --------------------------------------------------------------------------

void KTouchKeyboardWidget::reset() {
	m_keyboard->clear();

	// FIXME: Do we really need the code below? All we need to do is reset
	//        the bounding rectangle, right?
    setScene(0);
    delete(m_scene);
    m_scene = new QGraphicsScene(this);

    setScene(m_scene);
}
// --------------------------------------------------------------------------

void KTouchKeyboardWidget::createDefaultKeyboard() {
    reset();

/*    // let's create a default keyboard
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
        m_scene->addItem(key);
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
*/
}
// --------------------------------------------------------------------------

/*
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
            m_scene->addItem(key);
            m_keyList.append( key );
            m_connectorList.append( KTouchKeyConnection(keyAscII, keyAscII, 0, 0) );
// 			kDebug() << "read : F : unicode = " << keyAscII << " char = " << QChar(keyAscII) << endl;
        }
        else if (keyType=="ControlKey") {
            lineStream >> keyText >> x >> y >> w >> h;
            key = new KTouchControlKey(keyAscII, keyText, x+1, y+1, w-2, h-2);
            m_scene->addItem(key);
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

            m_scene->addItem(key);

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
// --------------------------------------------------------------------------
*/

