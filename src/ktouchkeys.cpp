/***************************************************************************
 *   ktouchkeys.cpp                                                        *
 *   --------------                                                        *
 *   Copyright (C) 2000 by Håvard Frøiland, 2004 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchkeys.h"
#include "ktouchkeys.moc"

#include <kdebug.h>
#include "ktouchcolorscheme.h"

#include "prefs.h"

KTouchBaseKey::KTouchBaseKey(const QChar& keyChar, const QString& keyText, int colorIndex, int x, int y, int w, int h)
      : m_colorIndex(colorIndex), m_keyChar(keyChar), m_keyText(keyText), m_isActive(false), m_isNextKey(false),
        m_w(w), m_h(h), m_type(NORMAL_KEY), stage(0), timer()
    {
        setPos(x, y);
        connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
    };


KTouchNormalKey::KTouchNormalKey(const QChar& keyChar, const QString& keyText, int colorIndex, int x, int y, int w, int h)
  : KTouchBaseKey(keyChar, keyText, colorIndex, x, y, w, h)
{
    m_type = NORMAL_KEY;
    m_font_scale =2;
}


void KTouchNormalKey::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    const KTouchColorScheme& colorScheme = KTouchColorScheme::m_colorSchemes[Prefs::currentColorScheme()];

    if (m_isNextKey) {
        painter->setBrush( colorScheme.m_backgroundH );
        painter->setPen( colorScheme.m_frame );
        painter->drawRoundRect(0, 0, m_w, m_h);
        painter->setBrush(colorScheme.m_cBackgroundH );
        painter->drawEllipse(0, 0, m_w, m_h);
    }
    else {
        painter->setBrush( colorScheme.m_background[m_colorIndex] );
        painter->setPen( colorScheme.m_frame );
        painter->drawRoundRect(0, 0, m_w, m_h);
    };

    painter->setPen( colorScheme.m_text );
    painter->setFont( m_font );
    painter->drawText(0, 0+1, m_w, m_h, Qt::AlignCenter, m_keyText);
}


KTouchFingerKey::KTouchFingerKey(const QChar& keyChar, const QString& keyText, int colorIndex, int x, int y, int w, int h)
  : KTouchNormalKey(keyChar, keyText, colorIndex, x, y, w, h)
{
    m_type = FINGER_KEY;
    m_font_scale =2;
}


void KTouchFingerKey::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    const KTouchColorScheme& colorScheme = KTouchColorScheme::m_colorSchemes[Prefs::currentColorScheme()];

    if (m_isActive) {
        painter->setPen( colorScheme.m_frame );
        painter->setBrush( colorScheme.m_background[m_colorIndex] );
        painter->drawRoundRect(0, 0, m_w, m_h);
        painter->drawEllipse(0, 0, m_w, m_h);
        painter->setPen( colorScheme.m_text );

    }
    else if (m_isNextKey) {

        painter->setBrush( colorScheme.m_backgroundH );
        painter->setPen( colorScheme.m_frame );
        painter->drawRoundRect(0, 0, m_w, m_h);
        painter->setPen( colorScheme.m_textH );

    }
    else {
        painter->setBrush( colorScheme.m_background[m_colorIndex] );
        painter->setPen( colorScheme.m_frame );
        painter->drawRoundRect(0, 0, m_w, m_h);
        painter->setBrush( colorScheme.m_cBackgroundH  );
        painter->drawEllipse(0, 0, m_w, m_h);
        painter->setPen( colorScheme.m_text );

    };
    painter->setFont( m_font );
    painter->drawText(QRectF(0, 0+1, m_w, m_h), Qt::AlignCenter, m_keyText);
}


KTouchControlKey::KTouchControlKey(const QChar& keyChar, const QString& keyText, int x, int y, int w, int h)
  : KTouchBaseKey(keyChar, keyText, 0, x, y, w, h)
{
    m_type = CONTROL_KEY;
    m_font_scale = 4;
}


void KTouchControlKey::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    const KTouchColorScheme& colorScheme = KTouchColorScheme::m_colorSchemes[Prefs::currentColorScheme()];

    QColor textColor;

    if (m_isActive) {
        painter->setBrush( colorScheme.m_cBackgroundH );
        painter->setPen( colorScheme.m_frame );
        textColor = colorScheme.m_cTextH;
    }
    else if (m_isNextKey) {
        painter->setBrush( colorScheme.m_backgroundH );
        painter->setPen( colorScheme.m_frame );
        textColor = colorScheme.m_textH;
    }
    else {
        painter->setBrush( colorScheme.m_cBackground );
        painter->setPen( colorScheme.m_frame );
        textColor = colorScheme.m_cText;
    };

    painter->drawRoundRect(0, 0, m_w, m_h);

    painter->setPen( QPen(textColor,0.2) );
    qreal h = qMin(m_w, m_h);
    qreal ch = h*0.5;   // the height for the special chars
    if (m_keyText=="Shift") {
        qreal x = 0+h/2;
        qreal y = 0+m_h/2;
        painter->drawLine(QLineF(x-ch/2, y, x-ch/4, y));
        painter->drawLine(QLineF(x-ch/4, y, x-ch/4, y+ch/2));
        painter->drawLine(QLineF(x-ch/4, y+ch/2, x+ch/4, y+ch/2));
        painter->drawLine(QLineF(x+ch/4, y+ch/2, x+ch/4, y));
        painter->drawLine(QLineF(x+ch/4, y, x+ch/2, y));
        painter->drawLine(QLineF(x+ch/2, y, x, y-ch/2));
        painter->drawLine(QLineF(x, y-ch/2, x-ch/2, y));
    }
    else if (m_keyText=="CapsLock") {
        qreal x = 0+h/2;
        qreal y = 0+m_h/2;

        painter->drawLine(QLineF(x-ch/2, y, x-ch/4, y));
        painter->drawLine(QLineF(x-ch/4, y, x-ch/4, y-ch/2));
        painter->drawLine(QLineF(x-ch/4, y-ch/2, x+ch/4, y-ch/2));
        painter->drawLine(QLineF(x+ch/4, y-ch/2, x+ch/4, y));
        painter->drawLine(QLineF(x+ch/4, y, x+ch/2, y));
        painter->drawLine(QLineF(x+ch/2, y, x, y+ch/2));
        painter->drawLine(QLineF(x, y+ch/2, x-ch/2, y));
    }
    else if (m_keyText=="Tab") {
        qreal xleft = 0+h/2-ch/2;
        qreal xright = 0 + qMin(m_w-h/2+ch/2,h);
        qreal y = 0+m_h/2;

        painter->drawLine(QLineF(xleft, y,xleft, y-ch/2));
        painter->drawLine(QLineF(xleft, y-ch/4, xright, y-ch/4));
        painter->drawLine(QLineF(xleft, y-ch/4, xleft+ch/2, y- ch*0.10));
        painter->drawLine(QLineF(xleft, y-ch/4, xleft+ch/2, y- ch*0.40));
        painter->drawLine(QLineF(xright, y, xright, y+ch/2));
        painter->drawLine(QLineF(xleft,  y+ch/4, xright, y+ch/4));
        painter->drawLine(QLineF(xright, y+ch/4, xright-ch/2, y+ch*0.10));
        painter->drawLine(QLineF(xright, y+ch/4, xright-ch/2, y+ch*0.40));

    }
    else if (m_keyText=="BackSpace") {
        qreal xleft = 0+h/2-ch/2;
        qreal xright = 0 + qMin(m_w-h/2+ch/2,h);
        qreal y = 0+m_h/2;

        painter->drawLine(QLineF(xleft, y,xright, y));
        painter->drawLine(QLineF(xleft, y, xleft+ch/2, y-ch*0.15));
        painter->drawLine(QLineF(xleft, y, xleft+ch/2, y+ch*0.15));

    }
    else if (m_keyText=="Enter") {
        qreal xleft = 0+h/2-ch/2;
        qreal xright = 0 + qMin(m_w-h/2+ch/2,h);
        qreal y = 0+m_h/2;

        painter->drawLine(QLineF(xright, y-ch/2,xright, y));
        painter->drawLine(QLineF(xleft, y,xright, y));
        painter->drawLine(QLineF(xleft, y, xleft+ch/3, y-static_cast<qreal>(ch*0.15)));
        painter->drawLine(QLineF(xleft, y, xleft+ch/3, y+static_cast<qreal>(ch*0.15)));

    }
    else if (m_keyText=="AltGr") {
        painter->setFont( m_font );
        painter->drawText(0, 0, m_w, m_h, Qt::AlignCenter | Qt::AlignVCenter, "Alt Gr");
    }
    else {
        painter->setFont( m_font );
        painter->drawText(0, 0, m_w, m_h, Qt::AlignCenter | Qt::AlignVCenter, m_keyText);
    }
}



