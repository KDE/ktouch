/***************************************************************************
 *   ktouchkeys.cpp                                                        *
 *   --------------                                                        *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchkeys.h"
#include <qpen.h>
#include <kglobalsettings.h>
#include <kdebug.h>
//#include <utility>  // for std::min

#include "ktouchsettings.h"
#include "ktouchmacros.h"

// Initialisation of static variables
int KTouchFingerKey::m_fingerKeyCount = 0;
// -----------------------------------------------------------------------------


// ***** Implementation of class KTouchKey ****

void KTouchKey::paint(QPainter& p) const {
    // We simply paint the key using the current brush and pen, so the derived classes
    // will have to care about that
    p.fillRect(m_xS, m_yS, m_wS, m_hS, p.brush());
    p.drawRect(m_xS, m_yS, m_wS, m_hS);
}

void KTouchKey::resize(double scale) {
    m_xS = static_cast<int>(scale*m_x);
    m_yS = static_cast<int>(scale*m_y);
    m_wS = static_cast<int>(scale*m_w);
    m_hS = static_cast<int>(scale*m_h);
    m_font=KTouchConfig().m_keyboardFont;
    m_font.setPointSize( static_cast<int>(min(m_wS,m_hS)/m_font_scale) );

}
// -----------------------------------------------------------------------------



// **** Implementation of class KTouchNormalKey ****

KTouchNormalKey::KTouchNormalKey(const QChar& keyChar, const QString& keyText, int x, int y, int w, int h)
  : KTouchKey(keyChar, keyText, x, y, w, h), m_colorIndex(0)
{
    m_type = NORMAL_KEY;
    m_font_scale =2;
}

void KTouchNormalKey::paint(QPainter& p) const {
    KTouchKeyboardColor& colorScheme = KTouchConfig().m_keyboardColors[KTouchConfig().m_keyboardColorScheme];
    QColor textColor;
    if (m_isNextKey) {
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
    KTouchKey::paint(p);  // call the parents paint() function
    p.setPen(textColor);
   
    p.setFont( m_font );
    p.drawText(m_xS, m_yS, m_wS, m_hS, QPainter::AlignCenter, m_keyText);
}
// --------------------------------------------------------------



// **** Implementation of class KTouchFingerKey ****

KTouchFingerKey::KTouchFingerKey(const QChar& keyChar, const QString& keyText, int x, int y, int w, int h)
  : KTouchNormalKey(keyChar, keyText, x, y, w, h)
{
    m_colorIndex = m_fingerKeyCount++;
    if (m_colorIndex>=8) {
        kdDebug() << "[KTouchFingerKey::KTouchFingerKey]  Number of finger keys = "
                  << m_colorIndex << "! Setting colour index to 0" << endl;
        m_colorIndex=0;
    };
    m_type = FINGER_KEY;
    m_font_scale =2;
}

void KTouchFingerKey::paint(QPainter& p) const {
    KTouchKeyboardColor& colorScheme = KTouchConfig().m_keyboardColors[KTouchConfig().m_keyboardColorScheme];
		p.setFont( m_font );
    if (m_isActive) {
        p.setBrush( colorScheme.m_background[m_colorIndex] );
        p.setPen( colorScheme.m_frame );
        KTouchKey::paint(p);                                                    // draw background and frame
        p.setPen( QPen(colorScheme.m_frame,3) );
        p.drawRect(m_xS+2, m_yS+2, m_wS-4, m_hS-4);                             // mark it as "active"
        p.setPen( colorScheme.m_text );
        p.drawText(m_xS, m_yS, m_wS, m_hS, QPainter::AlignCenter, m_keyText);
    }
    else if (m_isNextKey) {
        p.setBrush( colorScheme.m_backgroundH );
        p.setPen( colorScheme.m_frame );
        KTouchKey::paint(p);
        p.setPen( colorScheme.m_textH );
        p.drawText(m_xS, m_yS, m_wS, m_hS, QPainter::AlignCenter, m_keyText);
    }
    else {
        p.setBrush( colorScheme.m_background[m_colorIndex] );
        p.setPen( colorScheme.m_frame );
        KTouchKey::paint(p);
        p.drawRoundRect(m_xS+2, m_yS+2, m_wS-4, m_hS-4);
        p.setPen( colorScheme.m_text );
        p.drawText(m_xS, m_yS, m_wS, m_hS, QPainter::AlignCenter, m_keyText);
    };
}
// --------------------------------------------------------------



// **** Implementation of class KTouchControlKey ****

KTouchControlKey::KTouchControlKey(const QChar& keyChar, const QString& keyText, int x, int y, int w, int h)
  : KTouchKey(keyChar, keyText, x, y, w, h)
{
    m_type = CONTROL_KEY;
    m_font_scale = 4;
}

void KTouchControlKey::paint(QPainter& p) const {
    KTouchKeyboardColor& colorScheme = KTouchConfig().m_keyboardColors[KTouchConfig().m_keyboardColorScheme];
    p.setFont( m_font );
    QColor textColor;
    if (m_isActive) {
        p.setBrush( colorScheme.m_cBackgroundH );
        p.setPen( colorScheme.m_frame );
        textColor = colorScheme.m_cTextH;
    }
    else if (m_isNextKey) {
        p.setBrush( colorScheme.m_backgroundH );
        p.setPen( colorScheme.m_frame );
        textColor = colorScheme.m_textH;
    }
    else {
        p.setBrush( colorScheme.m_cBackground );
        p.setPen( colorScheme.m_frame );
        textColor = colorScheme.m_cText;
    };
    KTouchKey::paint(p);
    p.setPen( textColor );
    int h=min(m_wS, m_hS);
    int ch = static_cast<int>(h*0.5);   // the height for the special chars
    if (m_keyText=="Shift") {
        int x = m_xS+h/2;
        int y = m_yS+m_hS/2;
        p.moveTo(x-ch/2, y);
        p.lineTo(x-ch/4, y);
        p.lineTo(x-ch/4, y+ch/2);
        p.lineTo(x+ch/4, y+ch/2);
        p.lineTo(x+ch/4, y);
        p.lineTo(x+ch/2, y);
        p.lineTo(x, y-ch/2);
        p.lineTo(x-ch/2, y);
    }
    else if (m_keyText=="CapsLock") {
        int x = m_xS+h/2;
        int y = m_yS+m_hS/2;
        p.moveTo(x-ch/2, y);
        p.lineTo(x-ch/4, y);
        p.lineTo(x-ch/4, y-ch/2);
        p.lineTo(x+ch/4, y-ch/2);
        p.lineTo(x+ch/4, y);
        p.lineTo(x+ch/2, y);
        p.lineTo(x, y+ch/2);
        p.lineTo(x-ch/2, y);
    }
    else if (m_keyText=="Tab") {
        int xleft = m_xS+h/2-ch/2;
        int xright = m_xS+min(m_wS-h/2+ch/2,h);
        int y = m_yS+m_hS/2;
        p.drawLine(xleft, y,xleft, y-ch/2);
        p.drawLine(xleft, y-ch/4, xright, y-ch/4);
        p.drawLine(xleft, y-ch/4, xleft+ch/2, y-static_cast<int>(ch*0.10));
        p.drawLine(xleft, y-ch/4, xleft+ch/2, y-static_cast<int>(ch*0.40));
        p.drawLine(xright, y, xright, y+ch/2);
        p.drawLine(xleft,  y+ch/4, xright, y+ch/4);
        p.drawLine(xright, y+ch/4, xright-ch/2, y+static_cast<int>(ch*0.10));
        p.drawLine(xright, y+ch/4, xright-ch/2, y+static_cast<int>(ch*0.40));

    }
    else if (m_keyText=="BackSpace") {
        int xleft = m_xS+h/2-ch/2;
        int xright = m_xS+min(m_wS-h/2+ch/2,h);
        int y = m_yS+m_hS/2;
        p.drawLine(xleft, y,xright, y);
        p.drawLine(xleft, y, xleft+ch/2, y-static_cast<int>(ch*0.15));
        p.drawLine(xleft, y, xleft+ch/2, y+static_cast<int>(ch*0.15));
    }
    else if (m_keyText=="Enter") {
        int xleft = m_xS+h/2-ch/2;
        int xright = m_xS+min(m_wS-h/2+ch/2,h);
        int y = m_yS+m_hS/2;
        p.drawLine(xright, y-ch/2,xright, y);
        p.drawLine(xleft, y,xright, y);
        p.drawLine(xleft, y, xleft+ch/3, y-static_cast<int>(ch*0.15));
        p.drawLine(xleft, y, xleft+ch/3, y+static_cast<int>(ch*0.15));
    }
    else if (m_keyText=="AltGr") {
        p.drawText(m_xS, m_yS, m_wS, m_hS, QPainter::AlignCenter | QPainter::AlignVCenter, "Alt Gr");
    }
    else
        p.drawText(m_xS, m_yS, m_wS, m_hS, QPainter::AlignCenter | QPainter::AlignVCenter, m_keyText);
}



