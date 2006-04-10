/***************************************************************************
 *   ktouchslideline.h                                                     *
 *   -----------------                                                     *
 *   Copyright (C) 2000 by Håvard Frøiland, 2006 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
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
#include <kdebug.h>

#include <cmath>
#include <algorithm>

#include "prefs.h"
#include "ktouchcolorscheme.h"

// uncomment the following define to enable extended debugging
//#define SLIDE_LINE_DEBUGGING


// --- don't touch the lines below ---
#ifdef SLIDE_LINE_DEBUGGING

#define DRAW_TEACHER_CURSOR
#define KD_DEBUG(x)  kdDebug() << x 

#else // SLIDE_LINE_DEBUGGING

#define KD_DEBUG(x) ;

#endif // SLIDE_LINE_DEBUGGING
// --- don't touch the lines above ---


KTouchSlideLine::KTouchSlideLine(QWidget *parent)
  : QWidget( parent ),
    m_teacherPixmap(NULL),
    m_studentPixmap(NULL),
    m_slideTimer(this),
    m_cursorVisible(false),
    m_cursorTimer(this)
{
    // set widget defaults (note: teacher and student text is empty after creation)
    setMinimumHeight(50);
    setMaximumHeight(150);
    // NOTE : the maximum widget height is adjusted again in applyPreferences()

    setCursorTimerEnabled(true);

	m_marginCursor = 0;
 	m_cursorRangeLen = 0;
	m_slideLineHeight = 0;

	m_xCharWidth = 0;
	m_spaceCharWidth = 0;
	m_cursorHeight = 0;

	m_teacherTextLen = 0;
	m_correctTextLen = 0;
	m_studentTextLen = 0;
	m_xCursorTeacher = 0;
	m_xCursorStudent = 0;
	m_yCursorStudent = 0;

	// the x frame coordinates initially to zero
	m_xFrameTeacher = 0; 
	m_xFrameTeacherCurrent = 0;
    m_xFrameStudent = 0;  
    m_xFrameStudentCurrent = 0;

    connect( &m_cursorTimer, SIGNAL(timeout()), this, SLOT(toggleCursor()) );
    connect( &m_slideTimer, SIGNAL(timeout()), this, SLOT(slide()) );
}
// ----------------------------------------------------------------------------

KTouchSlideLine::~KTouchSlideLine() {
    delete m_teacherPixmap;
    delete m_studentPixmap;
}
// ----------------------------------------------------------------------------

void KTouchSlideLine::applyPreferences() {
	KD_DEBUG( "[KTouchSlideLine::applyPreferences]" << endl );

	// set maximum widget height (also determines maximum font size)
    setMaximumHeight(Prefs::maxSlidingWidgetHeight());

	// set font face of we override the lecture font
	if (Prefs::overrideLectureFont())
    	setFont(Prefs::font());
	else
		updateSlidingLines();
		
	// Note: The function setFont() in turn calls resizeFont() and through that
	//       the whole widget gets updated.
}
// ----------------------------------------------------------------------------

void KTouchSlideLine::setNewText(const QString& teacher_text, const QString& student_text) {
	KD_DEBUG( "[KTouchSlideLine::setNewText]" << endl );

    m_teacherText=teacher_text;
    m_studentText=student_text;
	KD_DEBUG( "  m_teacherText   = '" << m_teacherText << "'" << endl );
	KD_DEBUG( "  m_studentText   = '" << m_studentText << "'" << endl );

    updateSlidingLines();
}
// ----------------------------------------------------------------------------

void KTouchSlideLine::setStudentText(const QString& text) {
	KD_DEBUG( "[KTouchSlideLine::setStudentText]" << endl );
    m_studentText=text;
    updateStudentLine();
}
// ----------------------------------------------------------------------------

void KTouchSlideLine::setFont(const QFont& font) {
	KD_DEBUG( "[KTouchSlideLine::setFont]" << endl );
	// set the font for the slide line, unless the configuration overrides it
	if (Prefs::overrideLectureFont())		m_font = Prefs::font();
	else									m_font = font;

	// set point size of the font
	resizeFont();
	// Note: this function also takes care of updating the lines
}
// ----------------------------------------------------------------------------

bool KTouchSlideLine::canAddCharacter(const QString& new_student_text) {
    QFontMetrics fontMetrics( m_font );
	int new_text_len = textLen(fontMetrics, new_student_text);
	int allowed_student_length = m_studentPixmap->width() - 2*m_marginCursor;
	return (new_text_len <= allowed_student_length);
}
// ----------------------------------------------------------------------------


// *** Public slots ***

void KTouchSlideLine::setCursorTimerEnabled(bool on) {
    if (on)     m_cursorTimer.start(600);
    else        m_cursorTimer.stop();
    m_cursorVisible=false;
    drawCursor();
}
// ----------------------------------------------------------------------------


// *** Private slots ***

void KTouchSlideLine::toggleCursor() {
    m_cursorVisible=!m_cursorVisible;
    drawCursor();
}
// ----------------------------------------------------------------------------

void KTouchSlideLine::slide() {
	KD_DEBUG( "[KTouchSlideLine::slide]" << endl );
    if (m_studentPixmap==NULL || m_teacherPixmap==NULL) return;
    // calculate new x positions depending on slide speed
    double speed = 1.0 + 0.2*Prefs::slidingSpeed();

	KD_DEBUG( "  speed = " << speed << endl );

	// Teacher's line
	double dx_teacher = m_xFrameTeacher - m_xFrameTeacherCurrent;
    if (fabs(dx_teacher) < 1.0)	{	// if we are already close enough, don't slide anylonger
		m_xFrameTeacherCurrent = m_xFrameTeacher;
		dx_teacher = 0;
	}
	else {
    	double dx_teacher_new = dx_teacher/speed;
		m_xFrameTeacherCurrent += dx_teacher_new;
	}
    int int_xFrameTeacherCurrent = static_cast<int>(ceil(m_xFrameTeacherCurrent));
    KD_DEBUG( "  m_xFrameTeacher = " << m_xFrameTeacher << " current = "
            << int_xFrameTeacherCurrent << " (" << m_xFrameTeacherCurrent << ")" << endl );
    if (!Prefs::right2LeftTyping()) {
     	bitBlt(this, m_marginHorWidget, m_marginVerWidget,
           	m_teacherPixmap, int_xFrameTeacherCurrent, 0, m_slideLineWidth, m_slideLineHeight);
	}
	else {
	}

	// Student's line
	double dx_student = m_xFrameStudent - m_xFrameStudentCurrent;
    if (fabs(dx_student) < 1.0) {	// if we are already close enough, don't slide anylonger
		m_xFrameStudentCurrent = m_xFrameStudent;
		dx_student = 0;
	}
	else {
    	double dx_student_new = dx_student/speed;
		m_xFrameStudentCurrent += dx_student_new;
	}
    // NOTE : use ceil() to always round up the value, otherwise we get a funny "jumping"
    //        curser effect
    int int_xFrameStudentCurrent = static_cast<int>(ceil(m_xFrameStudentCurrent));
    KD_DEBUG( "  m_xFrameStudent = " << m_xFrameStudent << " current = "
            << int_xFrameStudentCurrent << " (" << m_xFrameStudentCurrent << ")" << endl );
    if (!Prefs::right2LeftTyping()) {
     	bitBlt(this, m_marginHorWidget, m_marginVerWidget + m_slideLineHeight + m_slideLineDist,
           	m_studentPixmap, int_xFrameStudentCurrent, 0, m_slideLineWidth, m_slideLineHeight);
	}
	else {
	}

    // restart slide timer if necessary
    if (dx_teacher != 0 || dx_student != 0)
        m_slideTimer.start(100, true);  // start singleshot timer to slide again
    drawCursor();
}
// ----------------------------------------------------------------------------


// *** Protected member functions (event implementation) ***

void KTouchSlideLine::paintEvent(QPaintEvent*) {
    KD_DEBUG( "[KTouchSlideLine::paintEvent]" << endl );
    if (m_studentPixmap==NULL || m_teacherPixmap==NULL)
        updateSlidingLines();
    else
        slide();
}
// ----------------------------------------------------------------------------

void KTouchSlideLine::resizeEvent ( QResizeEvent * ) {
    KD_DEBUG( "[KTouchSlideLine::resizeEvent]" << endl );
	// required input member variables:  none

	// when the widget is resized, the whole geometry is invalidated, so we do:
	//   1. recalculate and store the geometry of the sliding lines
	//   2. resize the font
	//   3. recreate the sliding lines (this is done from the resizeFont() function

	// TODO : make these values depending on widget size
	m_marginVerWidget = 10; 
	m_slideLineDist =  5;
	m_marginCursor = 20;
	m_slideLineHeight = (height() - 2*m_marginVerWidget - m_slideLineDist)/2;

	KD_DEBUG( "  m_slideLineDist    = " << m_slideLineDist  << endl );
	KD_DEBUG( "  m_slideLineHeight  = " << m_slideLineHeight  << endl );
	KD_DEBUG( "  m_marginCursor     = " << m_marginCursor  << endl );

	// now resize the font
	resizeFont();
}
// ----------------------------------------------------------------------------



// *** Private member functions (event implementation)

int KTouchSlideLine::textLen(const QFontMetrics& fontMetrics, const QString& text) {
	//KD_DEBUG( "[KTouchSlideLine::textLen]" << endl;

	// required input member variables:  m_xCharWidth

	// add an x to the string and subtract the width of the single x afterwards,
	// so that in case of "blablabla   " still the correct size is returned
    int w;
	if (!Prefs::right2LeftTyping())
		w = fontMetrics.boundingRect(text + "x").width() - m_xCharWidth;
	else
		w = fontMetrics.boundingRect("x" + text).width() - m_xCharWidth;
    return w;
}
// ----------------------------------------------------------------------------

void KTouchSlideLine::resizeFont() {
	KD_DEBUG( "[KTouchSlideLine::resizeFont]" << endl );
	if (m_slideLineHeight == 0)  return; // can happen during startup

	// required input member variables:  m_slideLineHeight

	// TODO : add support for fixed/overridden font size

    // this formula sets the font height to 65% of the line height
    m_font.setPointSize(static_cast<int>(m_slideLineHeight*0.65));
	// set the cursor height
	m_cursorHeight = static_cast<int>(m_slideLineHeight*0.65);
	KD_DEBUG( "  m_cursorHeight   = " << m_cursorHeight  << endl );
	// calculate the margin between the top of the student line and the top of the cursor.
	int y_line_margin = (m_slideLineHeight - m_cursorHeight)/2;
	// set the y coordinate of the cursor
	m_yCursorStudent = height() - m_marginVerWidget - m_slideLineHeight + y_line_margin;
	KD_DEBUG( "  m_yCursorStudent = " << m_yCursorStudent  << endl );
	// get font infos
    QFontMetrics fontMetrics( m_font );
	// width of a single x character
	m_xCharWidth = fontMetrics.boundingRect("x").width();
	// width of a single space character
    m_spaceCharWidth = fontMetrics.boundingRect("x  x").width() - fontMetrics.boundingRect("x x").width();
	// update slide lines
	updateSlidingLines();
}
// ----------------------------------------------------------------------------

void KTouchSlideLine::drawCursor() {
	// required input member variables:  m_xCursorStudent, m_yCursorStudent, m_cursorHeight
	//									 m_xCursorTeacher
	//									 m_marginHorWidget, m_slideLineDist, m_slideLineHeight
	//									 m_xFrameTeacher, m_xFrameTeacherCurrent,
	//									 m_xFrameStudent, m_xFrameStudentCurrent,

    QPainter p(this);

#ifdef DRAW_TEACHER_CURSOR
	QColor col_tt = Prefs::commonTypingLineColors() ?
			     Prefs::teacherTextColor() :
				 KTouchColorScheme::m_colorSchemes[Prefs::currentColorScheme()].m_teacherTextColor;
	QColor col_tb = Prefs::commonTypingLineColors() ?
			     Prefs::teacherBackgroundColor() :
				 KTouchColorScheme::m_colorSchemes[Prefs::currentColorScheme()].m_teacherBackground;

    if (m_cursorVisible)    p.setPen( col_tt );
    else                    p.setPen( col_tb );

	int dx_teacher = static_cast<int>(m_xFrameTeacher - m_xFrameTeacherCurrent);
	int cursor_x_teacher = m_marginHorWidget + m_xCursorTeacher + dx_teacher;

	// don't draw cursor if outside frame
	if (cursor_x_teacher > m_marginHorWidget && cursor_x_teacher < width() - m_marginHorWidget) {
    	p.drawLine(cursor_x_teacher, m_yCursorStudent - m_slideLineDist - m_slideLineHeight,
				   cursor_x_teacher, m_yCursorStudent + m_cursorHeight - m_slideLineDist - m_slideLineHeight);
	}
#endif


    if (m_cursorVisible)    p.setPen( m_cursorColor );
    else                    p.setPen( m_cursorBackground );

	int dx_student = static_cast<int>(m_xFrameStudent - m_xFrameStudentCurrent);
	int cursor_x_student = m_marginHorWidget + m_xCursorStudent + dx_student;

	// don't draw cursor if outside frame
	if (cursor_x_student > m_marginHorWidget && cursor_x_student < width() - m_marginHorWidget) {
//        KD_DEBUG( "  cursor_x_student    = " << cursor_x_student  << endl );
    	p.drawLine(cursor_x_student, m_yCursorStudent,
				   cursor_x_student, m_yCursorStudent + m_cursorHeight);
	}
}
// ----------------------------------------------------------------------------

void KTouchSlideLine::drawEnterChar(QPainter *painter, int cursorPos, int y, int enterWidth) {
	// required input member variables:  none

    int gap = std::min(2,static_cast<int>(0.2*enterWidth));
    int enterHeight = static_cast<int>(0.4*enterWidth);
    int arrowSize = static_cast<int>(enterWidth/4.0);     // mind the difference between 4 and 4.0
    painter->drawLine(cursorPos+enterWidth, y, cursorPos+enterWidth, y-enterHeight);    // vertical line
    painter->drawLine(cursorPos+gap, y, cursorPos+enterWidth, y);                       // arrow
    painter->drawLine(cursorPos+gap, y, cursorPos+gap+arrowSize, y+arrowSize);          // arrow
    painter->drawLine(cursorPos+gap, y, cursorPos+gap+arrowSize, y-arrowSize);          // arrow
}
// ----------------------------------------------------------------------------

void KTouchSlideLine::updateSlidingLines() {
	KD_DEBUG( "[KTouchSlideLine::updateSlidingLines]" << endl );

	// required input member variables:  m_teacherText, m_studentText, m_marginCursor, m_cursorRangeLen
	//									 m_slideLineHeight, m_xCharWidth

	// check that input variables are ok, some of this stuff can happen during startup,
	// but we MUST NOT allow these variables here
    if (m_teacherText.isEmpty()) 	return;  
    if (m_slideLineHeight == 0) 	return;

	// first update some variables
    QFontMetrics fontMetrics( m_font );
	m_teacherTextLen = textLen(fontMetrics, m_teacherText);
	KD_DEBUG( "  m_teacherTextLen = " << m_teacherTextLen  << endl );

	// assume long text first and calculate variables for maximum slide line size
	m_marginHorWidget = 30; // TODO : make dependent of widget width
	m_slideLineWidth = width() - 2*m_marginHorWidget;
	m_cursorRangeLen = m_slideLineWidth - 2*m_marginCursor;

	// adjust m_marginCursor and m_cursorRangeLen if text is smaller then available widget width
	if (m_teacherTextLen < m_cursorRangeLen) {
		KD_DEBUG( "  --> short text, correcting variables" << endl );
		m_cursorRangeLen = m_teacherTextLen;
		m_slideLineWidth = m_cursorRangeLen + 2*m_marginCursor;
		m_marginHorWidget = (width() - m_slideLineWidth)/2;
	}
	KD_DEBUG( "  m_cursorRangeLen   = " << m_cursorRangeLen  << endl );
	KD_DEBUG( "  m_slideLineWidth   = " << m_slideLineWidth  << endl );
	KD_DEBUG( "  m_marginHorWidget  = " << m_marginHorWidget << endl );

    // delete old pixmaps because we have to change their sizes anyway
    delete m_teacherPixmap;
	m_teacherPixmap = NULL; // just a precaution
    delete m_studentPixmap;
	m_studentPixmap = NULL; // just a precaution

	// create the teacher pixmap
	int w = 2*m_marginCursor + m_teacherTextLen; // TODO : add size of enter character
	int h = m_slideLineHeight;
    m_teacherPixmap = new QPixmap(w,h);
	KD_DEBUG( "  m_teacherPixmap  = " << w << " x " << h << endl );

	// draw the teacher pixmap text
    QPainter painter;
    painter.begin (m_teacherPixmap, this);
    painter.setFont( m_font );

	QColor col_tt = Prefs::commonTypingLineColors() ?
			     Prefs::teacherTextColor() :
				 KTouchColorScheme::m_colorSchemes[Prefs::currentColorScheme()].m_teacherTextColor;
	QColor col_tb = Prefs::commonTypingLineColors() ?
			     Prefs::teacherBackgroundColor() :
				 KTouchColorScheme::m_colorSchemes[Prefs::currentColorScheme()].m_teacherBackground;

    painter.fillRect( m_teacherPixmap->rect(), QBrush(col_tb) );
    painter.setPen( col_tt );
    // create a rectangle for the text drawing
    QRect textRect(m_marginCursor, 0, w-2*m_marginCursor, h);
	// left aligned
    if (!Prefs::right2LeftTyping()) {
     	painter.drawText(textRect, QPainter::AlignLeft | QPainter::AlignVCenter, m_teacherText);
    	// TODO : draw enter char after the text
    }
	// right aligned, e.g. for Hebrew text
    else {
		painter.drawText(textRect, QPainter::AlignRight | QPainter::AlignVCenter, m_teacherText);
		// TODO : draw enter character at left of text
    }
    painter.end();
	// done with the teachers text which will not change so quickly again

	// create student line pixmap thats slightly longer than the teacher's pixmap
    m_studentPixmap = new QPixmap(w+100,h);
	KD_DEBUG( "  m_studentPixmap  = " << w+100 << " x " << h << endl );

	// update (draw) student line and calculate coordinates needed for sliding
	repaint(true); // trigger a paint event to erase the background
	updateStudentLine();
}
// ----------------------------------------------------------------------------

void KTouchSlideLine::updateStudentLine() {
	KD_DEBUG( "[KTouchSlideLine::updateStudentLine]" << endl );
	if (m_teacherPixmap == NULL || m_studentPixmap == NULL)  return;

	// required input member variables:  m_teacherText, m_studentText, m_marginCursor, m_cursorRangeLen
	//									 m_slideLineHeight, m_xCharWidth, m_teacherTextLen

	if (m_teacherTextLen == 0)  return;

    QFontMetrics fontMetrics( m_font );
	m_studentTextLen = textLen(fontMetrics, m_studentText);
	KD_DEBUG( "  m_studentTextLen = " << m_studentTextLen << endl );

	// we now need to find out, how much of the students text was typed correctly
    int teacherLen = m_teacherText.length();
    int studentLen = m_studentText.length();

	unsigned int min_len = QMIN(teacherLen, studentLen);

	QString correctText;
	if (!Prefs::right2LeftTyping()) {
		// for text typed from left to right
		for (unsigned int i=0; i<min_len; ++i) {
			if (m_teacherText[i] == m_studentText[i])	correctText += m_teacherText[i];
			else break;
		}
	}
	else {
		// for text typed from right to left
		for (unsigned int i=min_len; i>0; --i) {
			if (m_teacherText[i-1] == m_studentText[i-1])	correctText = m_teacherText[i] + correctText;
			else break;
		}
	}

    bool error = (correctText.length() != min_len);
	KD_DEBUG( "  error in text?   = " << error << endl );
	KD_DEBUG( "  correctText      = '" << correctText << "'" << endl );
	m_correctTextLen = textLen(fontMetrics, correctText);
	KD_DEBUG( "  m_correctTextLen = " << m_correctTextLen << endl );

	// *** Teacher slide line ***

	// adjust some of the coordinates
    // NOTE: use ceil to get consistent rounding
	m_xCursorTeacher = static_cast<int>(m_marginCursor + ceil(double(m_correctTextLen)/m_teacherTextLen * m_cursorRangeLen));
	KD_DEBUG( "  m_xCursorTeacher = " << m_xCursorTeacher << endl );

	// TODO : transform coordinates when Prefs::right2LeftTyping() is true

	// find the coordinates of the current position in the Teacher's pixmap
	m_xCursorTPixmap = m_marginCursor + m_correctTextLen;
	KD_DEBUG( "  m_xCursorTPixmap = " << m_xCursorTPixmap << endl );

	// find the left x coordinates of the frame to be copied from the teacher pixmap, but first store the shift.
	m_xFrameTeacher = m_xCursorTPixmap - m_xCursorTeacher;
	KD_DEBUG( "  m_xFrameTeacher  = " << m_xFrameTeacher << endl );


	// *** Student slide line ***

	// make sure our student text length is < then allowed length
	int allowed_student_length = m_studentPixmap->width() - 2*m_marginCursor;
	if (m_studentTextLen > allowed_student_length) {
		KD_DEBUG( "  WARNING : m_studentTextLen ("<< m_studentTextLen <<") > allowed_student_length ("<< allowed_student_length <<")" << endl );
		m_studentTextLen = allowed_student_length;
	}

	// adjust some of the coordinates
	if (m_studentTextLen > m_teacherTextLen)
		m_xCursorStudent = m_marginCursor + m_cursorRangeLen;
	else
		m_xCursorStudent = static_cast<int>(m_marginCursor + ceil(double(m_studentTextLen)/m_teacherTextLen * m_cursorRangeLen));
	KD_DEBUG( "  m_xCursorStudent = " << m_xCursorStudent << endl );

	// find the coordinates of the current position in the Students's pixmap
	m_xCursorSPixmap = m_marginCursor + m_studentTextLen;

	// find the left x coordinates of the frame to be copied from the teacher pixmap, but first store the shift.
	m_xFrameStudent = m_xCursorSPixmap - m_xCursorStudent;
	KD_DEBUG( "  m_xFrameTeacher  = " << m_xFrameStudent << endl );

    // now let's draw the students pixmap
    QPainter painter;
    painter.begin (m_studentPixmap, this);
    if (Prefs::colorOnError()) {
        // draw the student line depending on the colour settings
        if (error) {
			// determine colors depending on preferences settings
			m_cursorBackground = Prefs::commonTypingLineColors() 	? 	Prefs::errorBackgroundColor() :
				 KTouchColorScheme::m_colorSchemes[Prefs::currentColorScheme()].m_errorBackground;
			m_cursorColor = Prefs::commonTypingLineColors() 		?	Prefs::errorTextColor() :
				 KTouchColorScheme::m_colorSchemes[Prefs::currentColorScheme()].m_errorTextColor;
        }
        else {
			// determine colors depending on preferences settings
			m_cursorBackground = Prefs::commonTypingLineColors() 	? 	Prefs::studentBackgroundColor() :
				 KTouchColorScheme::m_colorSchemes[Prefs::currentColorScheme()].m_studentBackground;
			m_cursorColor = Prefs::commonTypingLineColors() 		? 	Prefs::studentTextColor() :
				 KTouchColorScheme::m_colorSchemes[Prefs::currentColorScheme()].m_studentTextColor;
        }
		painter.fillRect (m_studentPixmap->rect(), QBrush(m_cursorBackground));
		painter.setPen( m_cursorColor );
    }
    else {
        // use always student text colors
        m_cursorColor = Prefs::studentTextColor();
        painter.setPen( m_cursorColor );
        m_cursorBackground = Prefs::studentBackgroundColor();
        painter.fillRect( m_studentPixmap->rect(), QBrush(m_cursorBackground) );
    }
    // draw the text
    painter.setFont( m_font );
    QRect textRect(m_marginCursor, 0, m_studentPixmap->width()-2*m_marginCursor, m_studentPixmap->height());
    if (Prefs::right2LeftTyping())
       painter.drawText(textRect, QPainter::AlignRight | QPainter::AlignVCenter, m_studentText);
    else 
       painter.drawText(textRect, QPainter::AlignLeft | QPainter::AlignVCenter, m_studentText);
    painter.end();
	// done painting the students line

	// turn on cursor blinking	
    m_cursorVisible = true;
    m_cursorTimer.start(800);

    slide(); // start the sliding 
}
