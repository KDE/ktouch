/***************************************************************************
 *   ktouchslideline.h                                                     *
 *   -----------------                                                     *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchslideline.h"
#include "ktouchslideline.moc"

#include <qpainter.h>
#include <qpixmap.h>
#include <qpoint.h>
#include <qfontmetrics.h>
#include <kdebug.h>
#include <kglobalsettings.h>

#include <math.h>

#include "ktouchsettings.h"
#include "ktouchmacros.h"

// don't use defines here... most of the time they are pure evil :-)
const int LINE_SPACING          = 2;    // the distance between the teacher and student line
const int HORIZONTAL_MARGIN     = 30;   // the horizontal distance from the lines to the widget border
const int VERTICAL_MARGIN       = 10;   // the vertical distance from the lines to the widget border
const int INNER_MARGIN          = 20;   // the margin inside the line boxes
const int DEAD_BORDER           = 40;   // the width of the no scrolling region near the border

KTouchSlideLine::KTouchSlideLine(QWidget *parent)
  : QWidget( parent ),
    m_teacherPixmap(NULL),
    m_studentPixmap(NULL),
    m_slideTimer(this),
    m_shift(0),
    m_enterCharWidth(0),
    m_spaceCharWidth(0),
    m_frameWidth(0),
    m_teacherTextWidth(0),
    m_teacherFrameX(0),
    m_teacherFrameXEnd(0),
    m_studentFrameX(0),
    m_studentFrameXEnd(0),
    m_cursorVisible(false),
    m_cursorTimer(this),
    m_cursorXPos(0),
    m_cursorYPos(0),
    m_cursorHeight(0)
{
    // set widget defaults (note: teacher and student text is empty after creation)
    setMinimumHeight(50);
    setMaximumHeight(150);
    setCursorTimerEnabled(true);

    connect( &m_cursorTimer, SIGNAL(timeout()), this, SLOT(toggleCursor()) );
    connect( &m_slideTimer, SIGNAL(timeout()), this, SLOT(slide()) );
}

KTouchSlideLine::~KTouchSlideLine() {
    delete m_teacherPixmap;
    delete m_studentPixmap;
};

void KTouchSlideLine::applyPreferences() {
    m_font = KTouchConfig().m_font;
    resizeEvent(NULL); // because we need to recreate the pixmap sizes
    // note: resizeFont() will be called implicitly by resizeEvent()
};

void KTouchSlideLine::setNewText(const QString& teacherText, const QString& studentText) {
   if(teacherText[0].direction()==QChar::DirR)
    m_rightJustify=true;
   else
    m_rightJustify=false;
 
    m_teacherText=teacherText;
    m_studentText=studentText;
    resizeEvent(NULL); // because we need to recreate the pixmap sizes
}

void KTouchSlideLine::setStudentText(const QString& text) {
    m_studentText=text;
    updateLines();
}


// *** Public slots

void KTouchSlideLine::setCursorTimerEnabled(bool on) {
    if (on)     m_cursorTimer.start(600);
    else        m_cursorTimer.stop();
    m_cursorVisible=false;
    drawCursor();
};


// *** Private slots

void KTouchSlideLine::toggleCursor() {
    m_cursorVisible=!m_cursorVisible;
    drawCursor();
};

void KTouchSlideLine::slide() {
    if (m_studentPixmap==NULL || m_teacherPixmap==NULL) return;
    // kdDebug() << "[KTouchSlideLine::slide]" << endl;
    // calculate new x positions depending on slide speed
    double speed = 1.0 + 0.2*KTouchConfig().m_slideSpeed;
    double m_teacherDX = (m_teacherFrameXEnd - m_teacherFrameX)/speed;
    double m_studentDX = (m_studentFrameXEnd - m_studentFrameX)/speed;
    if (fabs(m_teacherDX)>1.0)  m_teacherFrameX += m_teacherDX;
    if (fabs(m_studentDX)>1.0)  m_studentFrameX += m_studentDX;
    if (m_studentFrameX<m_teacherFrameX)
        m_studentFrameX=m_teacherFrameX;
    // now simply copy the required parts of the teacher and student pixmaps onto the widget
    if(m_rightJustify==false){
     bitBlt(this, HORIZONTAL_MARGIN + m_shift, VERTICAL_MARGIN,
           m_teacherPixmap, static_cast<int>(m_teacherFrameX), 0, m_frameWidth, m_teacherPixmap->height());
     bitBlt(this, HORIZONTAL_MARGIN + m_shift, height() - VERTICAL_MARGIN - m_studentPixmap->height(),
           m_studentPixmap, static_cast<int>(m_studentFrameX), 0, m_frameWidth, m_studentPixmap->height());
    }
    else
    {
     bitBlt(this, HORIZONTAL_MARGIN + m_shift, VERTICAL_MARGIN,
           m_teacherPixmap, m_teacherPixmap->width()-static_cast<int>(m_teacherFrameX)-m_frameWidth, 0, m_frameWidth, m_teacherPixmap->height());
     bitBlt(this, HORIZONTAL_MARGIN + m_shift, height() - VERTICAL_MARGIN - m_studentPixmap->height(),
           m_studentPixmap,  m_studentPixmap->width()-static_cast<int>(m_studentFrameX)-m_frameWidth, 0, m_frameWidth, m_studentPixmap->height());
    }
    // restart slide timer if necessary
    if (m_teacherDX!=0 || m_studentDX!=0)
        m_slideTimer.start(100, true);  // start singleshot timer to slide again
    drawCursor();
};



// *** Protected member functions (event implementation)

void KTouchSlideLine::paintEvent(QPaintEvent*) {
    if (m_studentPixmap==NULL || m_teacherPixmap==NULL)
        resizeEvent(NULL);
    else
        slide();
};

void KTouchSlideLine::resizeEvent( QResizeEvent * ) {
    if (m_teacherText.isEmpty()) return;  // can happen during startup
    // kdDebug() << "[KTouchSlideLine::resizeEvent]" << endl;
    resizeFont();
    // delete old pixmaps because we have to change its size
    delete m_teacherPixmap;
    delete m_studentPixmap;
    int h = (height() - 2*VERTICAL_MARGIN - LINE_SPACING)/2;
    // calculate teacher text width (in pixel)
    QFontMetrics fontMetrics( m_font );
    m_teacherTextWidth = fontMetrics.boundingRect(m_teacherText).width(); // store text length in pixel
    // calculate some space for the enter character (includes a small gap between text and enter arrow).
    m_enterCharWidth = fontMetrics.height(); // change that formula if you don't like the enter char width
    // calculate text needed for teacher line including margins, enter character
    int w = m_teacherTextWidth + m_enterCharWidth + 2*INNER_MARGIN;
    // we need to know the size of a space char very accurately, so let's calculate it the hard way.
    m_spaceCharWidth = fontMetrics.boundingRect("x  x").width() - fontMetrics.boundingRect("x x").width();
    // Now let's create the teachers pixmap and print the text into. We have to do this only when the teacher
    // text changes or the widget is resized, so we can safely do this outside the paintEvent() function.
    m_teacherPixmap = new QPixmap(w,h);
    QPainter painter;
    painter.begin (m_teacherPixmap, this);
    painter.setFont( m_font );
    //painter.setColor( KTouchConfig().m_teacherTextColor );
    painter.fillRect( m_teacherPixmap->rect(), QBrush(KTouchConfig().m_teacherBackground) );
    painter.setPen( KTouchConfig().m_teacherTextColor );
    // create a rectangle for the text drawing
    QRect textRect(INNER_MARGIN, 0, w-2*INNER_MARGIN, h);
    if(m_rightJustify==false)
    {
     painter.drawText(textRect, QPainter::AlignLeft | QPainter::AlignVCenter, m_teacherText);
    drawEnterChar(&painter, w - INNER_MARGIN - m_enterCharWidth, h/2, m_enterCharWidth);
    }
    else
    {
     painter.drawText(textRect, QPainter::AlignRight | QPainter::AlignVCenter, m_teacherText);
     drawEnterChar(&painter, INNER_MARGIN - m_enterCharWidth, h/2, m_enterCharWidth);
    }
    painter.end();
    // Let's now create the students pixmap, which will be drawn in the paintEvent (because it changes frequently).
    // We use 5 times as much space as the teacher widget -> see paintEvent for explaination
    m_studentPixmap = new QPixmap(5*w,h);
    // And finally calculate and store the vertical cursor information
    m_cursorHeight = fontMetrics.height();
    m_cursorYPos = height() - VERTICAL_MARGIN - (m_studentPixmap->height() + m_cursorHeight)/2;
    updateLines();
    update();  // here we need a full update!
};



// *** Private member functions (event implementation)

void KTouchSlideLine::resizeFont() {
    // this formula sets the font height to 65% of the line height
    m_font.setPointSize(static_cast<int>( (height()-2*VERTICAL_MARGIN-LINE_SPACING)/2*0.65) );
};

void KTouchSlideLine::drawCursor() {
    QPainter p(this);
    if (m_cursorVisible)    p.setPen( m_cursorColor );
    else                    p.setPen( m_cursorBackground );
    int myX = m_cursorXPos + m_studentFrameXEnd - static_cast<int>(m_studentFrameX);
    if(m_rightJustify==true )
    {
     /*the small distance between the beging of the pixmap and the cursor:*/
     int dx=myX/*location of cursor*/ - (HORIZONTAL_MARGIN + m_shift)/*start of pixmap*/;
     
     myX=(HORIZONTAL_MARGIN + m_shift/*start of pixmap*/)+ m_frameWidth-dx ;
     }
    if (myX>HORIZONTAL_MARGIN && myX<width()-HORIZONTAL_MARGIN)
        p.drawLine(myX, m_cursorYPos, myX, m_cursorYPos + m_cursorHeight);
};

void KTouchSlideLine::drawEnterChar(QPainter *painter, int cursorPos, int y, int enterWidth) {
    int gap = min(2,static_cast<int>(0.2*enterWidth));
    int enterHeight = static_cast<int>(0.4*enterWidth);
    int arrowSize = static_cast<int>(enterWidth/4.0);     // mind the difference between 4 and 4.0
    painter->drawLine(cursorPos+enterWidth, y, cursorPos+enterWidth, y-enterHeight);    // vertical line
    painter->drawLine(cursorPos+gap, y, cursorPos+enterWidth, y);                       // arrow
    painter->drawLine(cursorPos+gap, y, cursorPos+gap+arrowSize, y+arrowSize);          // arrow
    painter->drawLine(cursorPos+gap, y, cursorPos+gap+arrowSize, y-arrowSize);          // arrow
};

int KTouchSlideLine::textWidth(const QFontMetrics& fontMetrics, const QString& text) {
    int w=fontMetrics.boundingRect(text).width();
    if (text.length()>0 && text[text.length()-1]==' ')
        w += m_spaceCharWidth;
    return w;
};

void KTouchSlideLine::updateLines() {
    if (m_teacherText.isEmpty()) return;  // can happen during startup, but we MUST NOT allow an empty teacher text here
    int teacherLen = m_teacherText.length();
    int studentLen = m_studentText.length();
    // We need to know whether the students text has been typed correctly or not.
    // We could set this in the main widget but then we would have an additional connectivity
    // and potential error source (and it's not time critical anyway... the drawing stuff is).
    bool error;
    if (teacherLen>=studentLen && m_teacherText.left(studentLen)==m_studentText)    error=false;
    else                                                                            error=true;
    // now let's draw the students pixmap
    QPainter painter;
    painter.begin (m_studentPixmap, this);
    if (KTouchConfig().m_useErrorColor) {
        // draw the student line depending on the colour settings
        if (error) {
            m_cursorBackground = KTouchConfig().m_errorBackground;
            painter.fillRect (m_studentPixmap->rect(), m_cursorBackground);
            m_cursorColor = KTouchConfig().m_errorTextColor;
            painter.setPen( m_cursorColor );
        }
        else {
            m_cursorBackground = KTouchConfig().m_studentBackground;
            painter.fillRect (m_studentPixmap->rect(), QBrush(m_cursorBackground) );
            m_cursorColor = KTouchConfig().m_studentTextColor;
            painter.setPen( m_cursorColor );
        };
    }
    else {
        // use always student text colors
        m_cursorColor = KTouchConfig().m_studentTextColor;
        painter.setPen( m_cursorColor );
        m_cursorBackground = KTouchConfig().m_studentBackground;
        painter.fillRect( m_studentPixmap->rect(), QBrush(m_cursorBackground) );
    };
    // draw the text
    painter.setFont( m_font );
    QFontMetrics fontMetrics = painter.fontMetrics();
    QRect textRect(INNER_MARGIN, 0, m_studentPixmap->width()-2*INNER_MARGIN, m_studentPixmap->height());
    if(m_rightJustify)
       painter.drawText(textRect, QPainter::AlignRight | QPainter::AlignVCenter, m_studentText);
    else 
       painter.drawText(textRect, QPainter::AlignLeft | QPainter::AlignVCenter, m_studentText);
    // and calculate the cursor position (local coordinates) in the student pixmap
    // the cursor position is the distance from the beginning of the student pixmap (text margin included)
    int studentTextLength=textWidth(fontMetrics,m_studentText);
    int studentCursorPos = INNER_MARGIN + studentTextLength;    
    // Ok, the text is drawn, now let's calculate the information for the slide() function
    int allowedWidth = width() - 2*HORIZONTAL_MARGIN;   // the maximum width available in the widget
    if (m_teacherPixmap->width() <= allowedWidth) {
        // line is shorter then the space: calculate m_shift to draw it centered
        m_shift = (allowedWidth-m_teacherPixmap->width())/2;
        allowedWidth=m_teacherPixmap->width();
    }
    else  m_shift=0; // no shift, let's slide
    // store the frame size that will be copied
    m_frameWidth = allowedWidth;

    // calculate the relative cursor positions in the output line
    QString typedText = m_teacherText.left(studentLen);
    int typedTextLength=textWidth(fontMetrics, typedText);
    double CPosFactor = min(1.0, static_cast<double>(typedTextLength)/m_teacherTextWidth);
    // calculate the local coordinate of the cursor in the teacher line
    int teacherCursorPos = INNER_MARGIN + typedTextLength;
    // now calculate the horizontal offset
    int xDistance = INNER_MARGIN + static_cast<int>( CPosFactor*(allowedWidth-2*INNER_MARGIN-m_enterCharWidth) );
    m_cursorXPos = HORIZONTAL_MARGIN + m_shift + xDistance + min(2,static_cast<int>(fontMetrics.height()*0.1));
    m_teacherFrameXEnd = teacherCursorPos - xDistance;
    m_studentFrameXEnd = studentCursorPos - xDistance;
    painter.end();

    m_cursorVisible = true;
    m_cursorTimer.start(800);
    slide();
};
