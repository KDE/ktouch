/***************************************************************************
 *   ktouchkeyboard.cpp                                                    *
 *   ------------------                                                    *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchkeyboard.h"
#include "ktouchkeyboard.moc"

#include <qfile.h>
#include <qtextstream.h>

#include <kdebug.h>
#include <ktempfile.h>
#include <klocale.h>
#include <kio/netaccess.h>
#include <kglobal.h>
#include <kstandarddirs.h>
#include <kmessagebox.h>

#include "ktouchsettings.h"
#include "ktouchmacros.h"

// the margin between keyboard and widget frame
const int MARGIN = 10;

// --------------------------------------------------------------------------


KTouchKeyboard::KTouchKeyboard(QWidget *parent)
  : QWidget(parent), m_keyboardWidth(100), m_keyboardHeight(60), m_currentLayout("")
{
    setMinimumHeight(100);          // when it's smaller you won't see anything
    m_keyList.setAutoDelete(true);  // the list is responsable for cleaning up
}


bool KTouchKeyboard::loadKeyboard(const KURL& url, QString* errorMsg) {
    QString target;
    if (KIO::NetAccess::download(url, target)) {
        QString msg;
        bool result = readKeyboard(target, msg);
        KIO::NetAccess::removeTempFile(target);
        if (!result && errorMsg!=NULL)
            *errorMsg = i18n("Couldn't read the keyboard layout file '%1'! ").arg(url.url()) + msg;
        return result;
    }
    else {
        if (errorMsg!=NULL)
            *errorMsg = i18n("Could not download/open keyboard layout file from '%1'!").arg(url.url());
        return false;
    }
}


void KTouchKeyboard::saveKeyboard(const KURL& url) {
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

    for (KTouchKey * key = m_keyList.first(); key; key = m_keyList.next()) {
        switch (key->type()) {
           case KTouchKey::FINGER_KEY   : out << "FingerKey  "; break;
           case KTouchKey::NORMAL_KEY   : out << "NormalKey  "; break;
           case KTouchKey::CONTROL_KEY  : out << "ControlKey "; break;
           default : out << "NormalKey  "; break;
        }
        QRect rect=key->frame();
        out << key->m_keyChar.unicode() << '\t' << key->m_keyText << '\t'
            << rect.left() << '\t' << rect.top() << '\t' << rect.width() << '\t' << rect.height() << endl;
    }

    if (temp) {
        KIO::NetAccess::upload(tmpFile, url);
        temp->unlink();
        delete temp;
    }
}

void KTouchKeyboard::applyPreferences(bool silent) {
    // let's check whether the keyboard layout has changed
    if (KTouchConfig().m_keyboardLayout!=m_currentLayout) {
        // if the layout is the number layout just create it and we're done
        if (KTouchConfig().m_keyboardLayout=="number") {
            createDefaultKeyboard();
            resizeEvent(NULL);
            return;
        }
        // ok, let's load this layout
        KStandardDirs *dirs=KGlobal::dirs();
        QString fileName = dirs->findResource("data","ktouch/" + KTouchConfig().m_keyboardLayout + ".keyboard");
        if (silent) {
            // during initialisation we don't want to have a message box, that's why this is silent
            if (!loadKeyboard(fileName))
                createDefaultKeyboard();
            else
                m_currentLayout=KTouchConfig().m_keyboardLayout;
        }
        else {
            QString errorMsg;
            if (!loadKeyboard(fileName, &errorMsg)) {
                KMessageBox::error( 0, i18n("Error reading the keyboad layout! The default number keypad will "
                    "be created instead. You can choose another keyboard layout in the preferences dialog."),
                    errorMsg);
                createDefaultKeyboard();
            }
            else
                m_currentLayout=KTouchConfig().m_keyboardLayout;
        }
    }
    // set if we have animation or not.
    m_showAnimation = KTouchConfig().m_showAnimation;

    updateColours();    // we recreate the colour connections,
    resizeEvent(NULL);  // paint the keyboard
    newKey(m_nextKey);  // and finally display the "next to be pressed" key again
}


void KTouchKeyboard::newKey(const QChar& nextChar) {
    QPainter painter(this);
    painter.translate(m_shift, MARGIN);
    // first clean the markings on all keys
    for (KTouchKey * key = m_keyList.first(); key; key = m_keyList.next()) {
        if (key->m_isActive || key->m_isNextKey) {
            key->m_isActive=key->m_isNextKey=false;
            key->paint(painter);
        }
    }

    if (m_showAnimation){ // only do this if we want to show animation.
        // find the key in the key connector list
        QValueList<KTouchKeyConnector>::iterator keyIt = m_connectorList.begin();
        while (keyIt!=m_connectorList.end() && (*keyIt).m_keyChar!=nextChar)  ++keyIt;
        // if found mark the appropriate keys
        if (keyIt!=m_connectorList.end()) {
            QChar targetChar = (*keyIt).m_targetKeyChar;
            QChar fingerChar = (*keyIt).m_fingerKeyChar;
            QChar controlChar = (*keyIt).m_controlKeyChar;
            // find the keys in the keylist
            for (KTouchKey * key = m_keyList.first(); key; key = m_keyList.next()) {
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


void KTouchKeyboard::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.translate(m_shift, MARGIN);
    // just print all visible keys
    for (KTouchKey * key = m_keyList.first(); key; key = m_keyList.next())
        key->paint(painter);
}


void KTouchKeyboard::resizeEvent(QResizeEvent *) {
    double hScale = static_cast<double>(width()-2*MARGIN)/m_keyboardWidth;
    double vScale = static_cast<double>(height()-2*MARGIN)/m_keyboardHeight;
    double scale = min(hScale, vScale);
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


bool KTouchKeyboard::readKeyboard(const QString& fileName, QString& errorMsg) {
    QFile infile(fileName);
    if ( !infile.open( IO_ReadOnly ) ) {
        errorMsg = i18n("Couldn't open file!");
        return false;
    }
    QTextStream in( &infile );
    QString line;
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
            errorMsg = i18n("Missing key type in line '%1'!").arg(line);
            return false;
        }
        // calculate the maximum extent of the keyboard on the fly...
        m_keyboardWidth = max(m_keyboardWidth, x+w);
        m_keyboardHeight = max(m_keyboardHeight, y+h);
    } while (!in.atEnd() && !line.isNull());
    updateColours();
    return (!m_keyList.isEmpty());  // empty file means error
}


void KTouchKeyboard::updateColours() {
    // loop over all key connections
    for (QValueList<KTouchKeyConnector>::iterator it = m_connectorList.begin(); it!=m_connectorList.end(); ++it) {
        QChar fingerChar = (*it).m_fingerKeyChar;
        if (fingerChar == QChar(0)) continue;
        QChar targetChar = (*it).m_targetKeyChar;
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

