/***************************************************************************
 *   ktouchkey.cpp                                                         *
 *   -------------                                                         *
 *   Copyright (C) 2000 by Håvard Frøiland, 2004 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchkey.h"
#include "ktouchkey.moc"

#include <QtXml>
#include <QtGui>

#include <kdebug.h>

#include "ktouchcolorscheme.h"
#include "ktouchkeyboard.h"

#include "prefs.h"

const double PEN_WIDTH = 1.0;

// **************************
// ***** Public functions ***
// **************************

KTouchKey::KTouchKey(QObject * parent) 
	: QObject(parent), m_state(NormalState), m_colorIndex(-1), m_type(Normal), 
	  m_x(0), m_y(0), m_w(0), m_h(0), m_fingerKeyIndex(-1), m_fingerKey(NULL)
{
	m_keyChar[0] = QChar();
	m_keyChar[1] = QChar();
	m_keyChar[2] = QChar();
	m_keyChar[3] = QChar();
	setPos(m_x, m_y);
	setZValue(0);
}
// -----------------------------------------------------------------------------

KTouchKey::KTouchKey(QObject * parent, keytype_t type, int x, int y, int w, int h, QChar ch)
	: QObject(parent), m_state(NormalState), m_colorIndex(-1), m_type(type), 
	  m_x(x), m_y(y), m_w(w), m_h(h), m_fingerKeyIndex(-1), m_fingerKey(NULL)
{
	m_keyChar[0] = ch;
	m_keyChar[1] = QChar();
	m_keyChar[2] = QChar();
	m_keyChar[3] = QChar();
	setPos(m_x, m_y);
}
// ----------------------------------------------------------------------------

KTouchKey::KTouchKey(QObject * parent, int x, int y, int w, int h, const QString &text) 
	: QObject(parent), m_state(NormalState), m_colorIndex(-1), m_x(x), m_y(y), m_w(w), m_h(h),
	  m_fingerKeyIndex(-1), m_fingerKey(NULL)
{
	m_type = Other;
	m_keyChar[0] = QChar();
	m_keyText = text;
	
	setPos(m_x, m_y);
}
// ----------------------------------------------------------------------------

// Reads the key data from the DomElement
bool KTouchKey::read(QDomElement e) {
	if (e.hasAttribute("Type")) {
		QString typetext = e.attribute("Type");
		m_type = keyType(typetext);
		if (m_type == Other) {
			if (e.hasAttribute("KeyText"))
				m_keyText = e.attribute("KeyText");
		}
	}
	QDomElement charElement = e.firstChildElement("Char");
	while (!charElement.isNull()) {
		position_t pos = TopLeft;
		if (charElement.hasAttribute("Position")) {
			QString position = charElement.attribute("Position");
			if (position == "TopLeft")  			pos = TopLeft;
			else if  (position == "TopRight")		pos = TopRight;
			else if  (position == "BottomLeft")		pos = BottomLeft;
			else if  (position == "BottomRight")	pos = BottomRight;
		}
		if (charElement.hasAttribute("Unicode"))
			m_keyChar[pos] = QChar(charElement.attribute("Unicode").toInt());
		else
			m_keyChar[pos] = QChar();
		// do we have a character?
		QDomNode chNode = charElement.firstChild();
		if (!chNode.isNull()) {
			QString chstr = chNode.nodeValue();
			if (chstr.count() > 0) {
				QChar ch = chstr[0];
				if (m_keyChar[pos]!=QChar() && m_keyChar[pos]!=ch.unicode()) {
					kDebug() << "Character '" << ch << "' is defined with different unicode, the given unicode "
						"value is used and the display character. is discarted. If you want to use the display "
						"character, delete the unicode property." << endl;
				}
				if (m_keyChar[pos]==QChar())
					m_keyChar[pos] = ch.unicode();
			}
		}
		charElement = charElement.nextSiblingElement("Char");
	}

	if (e.hasAttribute("Width")) 
		m_w = e.attribute("Width").toInt();
	if (e.hasAttribute("Height")) 
		m_h = e.attribute("Height").toInt();
	if (e.hasAttribute("X")) 
		m_x = e.attribute("X").toInt();
	if (e.hasAttribute("Y")) 
		m_y = e.attribute("Y").toInt();
	if (e.hasAttribute("FingerKeyIndex")) 
		m_fingerKeyIndex = e.attribute("FingerKeyIndex").toInt();
	else
		m_fingerKeyIndex = -1;
	
	setPos(m_x, m_y);
	//kDebug() << "Key = " << m_keyChar[TopLeft] << " " << m_x << " " << m_y << " " << m_w << " " << m_h << endl;
	return true;
}
// ----------------------------------------------------------------------------

// Writes the key data into the DomElement
void KTouchKey::write(QDomDocument& doc, QDomElement& root, const QList<KTouchKey*>& keys) const {
	QDomElement element = doc.createElement("Key");
	element.setAttribute("Type", keyTypeString(m_type));
	if (m_type == Other)
		element.setAttribute("KeyText", m_keyText);
	// write the characters
	for (int i=0; i<4; ++i) {
		if (m_keyChar[i] != QChar()) {
			QDomElement char_element = doc.createElement("Char");
			char_element.setAttribute("Unicode", m_keyChar[i].unicode());
			QString position;
			switch (i) {
				case 0 : position = "TopLeft"; break;
				case 1 : position = "TopRight"; break;
				case 2 : position = "BottomLeft"; break;
				case 3 : position = "BottomRight"; break;
			}
			char_element.setAttribute("Unicode", m_keyChar[i].unicode());
			char_element.setAttribute("Position", position);
			QDomText t = doc.createTextNode(QString(m_keyChar[i]));
			char_element.appendChild(t);
			element.appendChild(char_element);
		}
	}

	element.setAttribute("X", m_x);
	element.setAttribute("Y", m_y);
	element.setAttribute("Width", m_w);
	element.setAttribute("Height", m_h);
	if (m_fingerKey != NULL) {
		int index = keys.indexOf(m_fingerKey);
		if (index != -1)
			element.setAttribute("FingerKeyIndex", index);
	}

	root.appendChild(element);
}
// ----------------------------------------------------------------------------

void KTouchKey::setState(state_t state) {
	m_state = state;
	update();	
}
// ----------------------------------------------------------------------------

QRectF KTouchKey::boundingRect() const {
	return QRectF(- PEN_WIDTH/2.0, - PEN_WIDTH/2.0, 
		m_w + PEN_WIDTH, m_h + PEN_WIDTH);
}
// ----------------------------------------------------------------------------

void KTouchKey::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

	// get the colorscheme from the configuration
    const KTouchColorScheme& colorScheme = KTouchColorScheme::m_colorSchemes[Prefs::currentColorScheme()];

	// first draw the background
	switch (m_state) {
	  case NormalState :
	  {
	  
		switch (m_type) {
			case Normal :
			case Finger :
			{
				//Q_ASSERT(m_colorIndex < 8);
				QLinearGradient grad(QPointF(0,0), QPointF(0.3*m_h,1.3*m_h));
				//kDebug() << m_keyChar[0] << "  m_colorIndex = " << m_colorIndex << endl;
				if (m_colorIndex >= 0 && m_colorIndex<8) {
					QColor c = colorScheme.m_background[m_colorIndex];
					grad.setColorAt(0,c);
					qreal h, s, v, a;
					c.getHsvF(&h, &s, &v, &a);
					c.setHsvF(h, s, v*0.8, a);
					grad.setColorAt(1,c);
					painter->setBrush( QBrush(grad) );
				}
				else {
					painter->setBrush( widget->palette().color(QPalette::Background) );
				}
				QPen p(colorScheme.m_frame);
				p.setWidth(0); // only one pixel!
				painter->setPen( p );
				painter->drawRect(0, 0, m_w, m_h);
			} break;
			
			default :
			{
				QLinearGradient grad(QPointF(0,0), QPointF(0,m_h));
				QColor c = colorScheme.m_cBackground;
				grad.setColorAt(0,c);
				qreal h, s, v, a;
				c.getHsvF(&h, &s, &v, &a);
				c.setHsvF(h, s, v*0.7, a);
				grad.setColorAt(1,c);
				painter->setBrush( QBrush(grad) );
				QPen p(colorScheme.m_frame);
				p.setWidth(0); // only one pixel!
				painter->setPen( p );
				painter->drawRect(0, 0, m_w, m_h);
			}
		} // switch 
		
	  } break; // NormalState

	  case HighlightedState :
	  {
	  
		switch (m_type) {
			case Normal :
			case Finger :
			{
				//Q_ASSERT(m_colorIndex < 8);
				QLinearGradient grad(QPointF(0,0), QPointF(0.3*m_h,1.3*m_h));
				QColor c = colorScheme.m_backgroundH;
				grad.setColorAt(0,c);
				qreal h, s, v, a;
				c.getHsvF(&h, &s, &v, &a);
				c.setHsvF(h, s, v*0.8, a);
				grad.setColorAt(1,c);
				painter->setBrush( QBrush(grad) );
				QPen p(colorScheme.m_frame);
				p.setWidth(0); // only one pixel!
				painter->setPen( p );
				painter->drawRect(0, 0, m_w, m_h);
			} break;
			
			default :
			{
				QLinearGradient grad(QPointF(0,0), QPointF(0,m_h));
				QColor c = colorScheme.m_cBackgroundH;
				grad.setColorAt(0,c);
				qreal h, s, v, a;
				c.getHsvF(&h, &s, &v, &a);
				c.setHsvF(h, s, v*0.7, a);
				grad.setColorAt(1,c);
				painter->setBrush( QBrush(grad) );
				QPen p(colorScheme.m_frame);
				p.setWidth(0); // only one pixel!
				painter->setPen( p );
				painter->drawRect(0, 0, m_w, m_h);
			}
		} // switch 
		
	  } break; // HighlightedState
	}

	// draw text/decoration
    KTouchKeyboard * kb = dynamic_cast<KTouchKeyboard *>(parent());
    QFont f = kb->font();
	
    painter->setRenderHint(QPainter::Antialiasing);

	QPen p;
	switch (m_state) {
		case HighlightedState : {
			switch (m_type) {
				case Normal :
				case Finger :
				{
					p.setColor(colorScheme.m_textH);
				} break;

				default :
				{
					p.setColor(colorScheme.m_cTextH);
				}
			}
		} break;

		default : 
		{
			switch (m_type) {
				case Normal :
				case Finger :
				{
					p.setColor(colorScheme.m_text);
				} break;

				default :
				{
					p.setColor(colorScheme.m_cText);
				}
			}
		}
	}

	p.setWidthF(m_h*0.05);
	painter->setPen( p );

	qreal h = qMin(m_w, m_h);
	qreal ch = h*0.5;   // the height for the special chars
	switch (m_type) {
	  case Normal :
	  case Finger : 
	  {
	  	// only one topleft character?
	  	if (m_keyChar[KTouchKey::TopLeft] != QChar() && 
	  		m_keyChar[KTouchKey::BottomLeft] == QChar())
	  	{ 
	  		// print the character a little bit bigger
    		f.setPointSizeF( qMax(0.01, m_h*0.4) );
    		painter->setFont( f );
			painter->drawText(QRectF(m_h*0.1, m_h*0.1, m_h*0.6, m_h*0.6), Qt::AlignCenter | Qt::AlignVCenter, m_keyChar[0]);
			// if we also have a bottom right character, print it small 
			if (m_keyChar[KTouchKey::BottomRight] != QChar()) {
				f.setPointSizeF( qMax(0.01, m_h*0.3) );
				painter->setFont( f );
				painter->drawText(QRectF(m_h*0.2, m_h*0.05, m_w - m_h*0.3, m_h*0.95), 
								  Qt::AlignRight | Qt::AlignBottom, m_keyChar[KTouchKey::BottomRight]);
			}
		}
		else {
    		f.setPointSizeF( qMax(0.01, m_h*0.3) );
    		painter->setFont( f );
    		// print each character in one corner
    		if (m_keyChar[KTouchKey::TopLeft] != QChar()) {
				painter->drawText(QRectF(m_h*0.2, m_h*0.05, m_w - m_h*0.4, m_h*0.92), 
								  Qt::AlignLeft | Qt::AlignTop, m_keyChar[KTouchKey::TopLeft]);
    		}
    		if (m_keyChar[KTouchKey::TopRight] != QChar()) {
				painter->drawText(QRectF(m_h*0.2, m_h*0.05, m_w - m_h*0.3, m_h*0.92), 
								  Qt::AlignRight | Qt::AlignTop, m_keyChar[KTouchKey::TopRight]);
    		}
    		if (m_keyChar[KTouchKey::BottomLeft] != QChar()) {
				painter->drawText(QRectF(m_h*0.2, m_h*0.05, m_w - m_h*0.4, m_h*0.92), 
								  Qt::AlignLeft | Qt::AlignBottom, m_keyChar[KTouchKey::BottomLeft]);
    		}
    		if (m_keyChar[KTouchKey::BottomRight] != QChar()) {
				painter->drawText(QRectF(m_h*0.2, m_h*0.05, m_w - m_h*0.3, m_h*0.92), 
								  Qt::AlignRight | Qt::AlignBottom, m_keyChar[KTouchKey::BottomRight]);
    		}
		}
		
	  } break;

	  case Enter : {
        qreal xleft = 0+h/2-ch/2;
        qreal xright = 0 + qMin(m_w-h/2+ch/2,h);
        qreal y = 0+m_h/2;

        painter->drawLine(QLineF(xright, y-ch/2,xright, y));
        painter->drawLine(QLineF(xleft, y,xright, y));
        painter->drawLine(QLineF(xleft, y, xleft+ch/3, y-static_cast<qreal>(ch*0.15)));
        painter->drawLine(QLineF(xleft, y, xleft+ch/3, y+static_cast<qreal>(ch*0.15)));
	  } break;
		
	  case Backspace : {
        qreal xleft = 0+h/2-ch/2;
        qreal xright = 0 + qMin(m_w-h/2+ch/2,h);
        qreal y = 0+m_h/2;

        painter->drawLine(QLineF(xleft, y,xright, y));
        painter->drawLine(QLineF(xleft, y, xleft+ch/2, y-ch*0.15));
        painter->drawLine(QLineF(xleft, y, xleft+ch/2, y+ch*0.15));
	  } break;
	  
	  case Shift : {
        qreal x = 0+h/2;
        qreal y = 0+m_h/2;
        painter->drawLine(QLineF(x-ch/2, y, x-ch/4, y));
        painter->drawLine(QLineF(x-ch/4, y, x-ch/4, y+ch/2));
        painter->drawLine(QLineF(x-ch/4, y+ch/2, x+ch/4, y+ch/2));
        painter->drawLine(QLineF(x+ch/4, y+ch/2, x+ch/4, y));
        painter->drawLine(QLineF(x+ch/4, y, x+ch/2, y));
        painter->drawLine(QLineF(x+ch/2, y, x, y-ch/2));
        painter->drawLine(QLineF(x, y-ch/2, x-ch/2, y));
	  } break;
	  
	  case CapsLock : {
        qreal x = 0+h/2;
        qreal y = 0+m_h/2;

        painter->drawLine(QLineF(x-ch/2, y, x-ch/4, y));
        painter->drawLine(QLineF(x-ch/4, y, x-ch/4, y-ch/2));
        painter->drawLine(QLineF(x-ch/4, y-ch/2, x+ch/4, y-ch/2));
        painter->drawLine(QLineF(x+ch/4, y-ch/2, x+ch/4, y));
        painter->drawLine(QLineF(x+ch/4, y, x+ch/2, y));
        painter->drawLine(QLineF(x+ch/2, y, x, y+ch/2));
        painter->drawLine(QLineF(x, y+ch/2, x-ch/2, y));
	  } break;

	  case Tab : {
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
	  } break;
	  
	  case Space : ; break; // nothing on space key
	  
	  case Other :
   		f.setPointSizeF( qMax(0.01, m_h*0.4) );
   		painter->setFont( f );
		painter->drawText(QRectF(m_h*0.15, m_h*0.15, m_w - m_h*0.3, m_h*0.7), Qt::AlignCenter | Qt::AlignVCenter, m_keyText);
		break;
	}
    painter->setRenderHint(QPainter::Antialiasing, false);
}
// -----------------------------------------------------------------------------

// **************************
// ***** Static functions ***
// **************************

QString KTouchKey::keyTypeString(KTouchKey::keytype_t t) {
	switch (t) {
		case Normal     : return "NORMAL";
		case Finger     : return "FINGER";
		case Enter		: return "ENTER";
		case Backspace 	: return "BACKSPACE";
		case Shift		: return "SHIFT";
		case CapsLock	: return "CAPSLOCK";
		case Tab		: return "TAB";
		case Space 		: return "SPACE";
		default			: return "OTHER";
	}
}
// -----------------------------------------------------------------------------

KTouchKey::keytype_t KTouchKey::keyType(const QString& str) {
	if (str=="NORMAL")  		return Normal;
	else if (str=="FINGER")  	return Finger;
	else if (str=="ENTER")  	return Enter;
	else if (str=="BACKSPACE")	return Backspace;
	else if (str=="SHIFT")  	return Shift;
	else if (str=="CAPSLOCK")	return CapsLock;
	else if (str=="TAB")  		return Tab;
	else if (str=="SPACE")  	return Space;
	else return Other;	
}
// -----------------------------------------------------------------------------

// *****************************
// ***** Protected functions ***
// *****************************

void KTouchKey::mousePressEvent(QGraphicsSceneMouseEvent * event) {
	if (event->button() | Qt::LeftButton) {
		emit clicked(this);
		// are we allowed to click/activate the key?
		KTouchKeyboard * kb = dynamic_cast<KTouchKeyboard *>(parent());
		if (kb != NULL && kb->allowKeyClicks()) {
			QPointF scene_pos = event->scenePos();
			m_dragXOffset = scene_pos.x() - m_x;
			m_dragYOffset = scene_pos.y() - m_y;
			setZValue(1);
			// let the event pass through so that the graphics view can handle the selection part
		}
		else {
			event->accept();
		}
	}
}
// -----------------------------------------------------------------------------

void KTouchKey::mouseMoveEvent(QGraphicsSceneMouseEvent * event) {
	// are we allowed to click/activate/move the key?
	KTouchKeyboard * kb = dynamic_cast<KTouchKeyboard *>(parent());
	if (kb != NULL && kb->allowKeyClicks()) {
		QPointF scene_pos = event->scenePos();
		m_x = int( (scene_pos.x() - m_dragXOffset + 5)/10 )*10;
		m_y = int( (scene_pos.y() - m_dragYOffset + 5)/10 )*10;
		setPos(m_x, m_y);
		emit positionChanged(this);
		// let the event pass through so that the graphics view can handle the movement part
	}
	else {
		// this is the default behavior when the keyboard is used during the lecture
		event->accept();
	}
}
// -----------------------------------------------------------------------------

void KTouchKey::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {
	//emit clicked(this);
	setZValue(0);
}
// -----------------------------------------------------------------------------
