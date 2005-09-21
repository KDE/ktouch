/***************************************************************************
 *   ktouchslideline.h                                                     *
 *   -----------------                                                     *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHSLIDELINE_H
#define KTOUCHSLIDELINE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qwidget.h>
#include <qtimer.h>
//Added by qt3to4:
#include <QPixmap>
#include <QResizeEvent>
#include <QPaintEvent>
class QPixmap;
class QPainter;

/// This widget just displays the sliding text.
/// 
/// KTouchSlideLine is a fire-and-forget widget, so you don't have to care about any
/// drawing details. After the initial setup (setting the font, colours etc.) you simple
/// need to call setTeacherText() or setStudentText() and everything else (including the
/// choice of the background colour) is done by the widget.<p>
/// So, how does the sliding work. The basic idea is that when the teachers and the
/// students text won't fit in the widgets width only part of the text is shown. Depending
/// on the amount of text that has been already typed, both lines
/// (the teachers and the students line) will move. However, if the student
/// had entered a wrong text than the lines shouldn't move before he did not
/// correct his mistake. This ensures that you will always start at the left side
/// of the screen and end up at the right side.<p>
/// The calculation is very simple. Both the teachers and the students line are drawn
/// in pixmaps. Then you only need to calculate the parts of that pixmaps that have to be
/// copied on the screen and you're done! ??? Well, not quite so simple....<P>
/// The calculation is actually the tricky bit but here's the principle (
/// If you really want to understand the code, better draw a sketch - I needed several
/// sketches :-).<br>
/// After drawing the text into the pixmaps (easy) we calculate the ratio between
/// "typed text" and "total text per line" and multiply it with the horizontal space available.
/// This has to be done three times: For the global widget coordinates, for the teacher line
/// and for the student line. Then we simply convert from local to global coordinates and
/// store the new coordinates.
/// The timed function slide() will then slide the widgets into position.<p>
/// The calculation and the drawing are made in the updateLines() member function. Since
/// the teachers pixmap won't change when the students string changes it will only be renewed
/// when it's size changes (or the teacher text changes). This is done in the resizeEvent().<br>
/// The properties for the slide line: correctColor, errorColor, font can be manipulated
/// using the usual access functions.
class KTouchSlideLine : public QWidget {
    Q_OBJECT
  private:
    bool m_rightJustify; // do we right align the widget-for langauges like hebrew which is written from right to left.
  public:
    /// Constructor
    KTouchSlideLine(QWidget *parent);
    /// Destructor, free memory allocated for the line pixmap.
    ~KTouchSlideLine();
    /// Applies the preferences (font and colours).
    void applyPreferences();
    /// Sets the teacher and student text (usually called when a new line is started).
    void setNewText(const QString& teacherText, const QString& studentText);
    /// Sets the student text (called whenever the student string changes).
    void setStudentText(const QString& text);
	/// Change the font of the slide line widget.
	void setFont(const QFont& font);

  public slots:
    /// Starts or stops the cursor blinking timer.
    void setCursorTimerEnabled(bool on);

  private slots:
    /// Turns the cursor on or off and triggers an update (this function triggered by the cursor timer).
    void toggleCursor();
    /// Slides the lines into position (this function is triggered by the sliding timer).
    void slide();

  protected:
    /// Simply updates the widget: calls updateLines() and slide()
    void paintEvent( QPaintEvent * );
    /// Will be called when the widget is resized.
    /// This event will first recalculate the font size. Then the teachers and the students widget
    /// will be created and the teachers text will be drawn on the teachers pixmap. Finally update()
    /// is called.
    void resizeEvent ( QResizeEvent * );

  private:
    /// Will recalculate the font size depending on the height of the widget.
    void resizeFont();
    /// Just draws the cursor (if visible)
    void drawCursor();
    /// Draws the "enter" character at the given position (y is the y-position of the arrow).
    void drawEnterChar(QPainter *painter, int cursorPos, int y, int enterWidth);
    /// Calculates the correct text length taking trailing spaces into account
    int  textWidth(const QFontMetrics& fontMetrics, const QString& text);
    /// Redraws the student pixmaps and updates the frame x positions
    void updateLines();

    QFont       m_font;             ///< The font for the sliding lines.
    QString     m_teacherText;      ///< The teachers text.
    QString     m_studentText;      ///< The students text.
    QPixmap    *m_teacherPixmap;    ///< Pixmap used to draw the teacher sliding line.
    QPixmap    *m_studentPixmap;    ///< Pixmap used to draw the student sliding line.

    QTimer      m_slideTimer;       ///< This is the timer for the sliding of the lines.
    int         m_shift;            ///< The horizontal shift of the slide lines (used for centering)
    int         m_enterCharWidth;   ///< The width of the enter character (including the small gap).
    int         m_spaceCharWidth;   ///< The width of a space char - this is different to QFontMetrics::width(' ').
    int         m_frameWidth;       ///< The width of the frame that is copied from the pixmaps onto the widget.
    int         m_teacherTextWidth; ///< The length of the teacher line (in pixel) WITHOUT enter character.
    double      m_teacherFrameX;    ///< The current X-position in the teachers pixmap (local coordinates).
    int         m_teacherFrameXEnd; ///< The final X-position in the teachers pixmap (local coordinates).
    double      m_studentFrameX;    ///< The current X-position in the students pixmap (local coordinates).
    int         m_studentFrameXEnd; ///< The final X-position in the student pixmap (local coordinates).

    bool        m_cursorVisible;    ///< Flag which indicates the current state of the cursor.
    QTimer      m_cursorTimer;      ///< This is the cursor on/off timer.
    int         m_cursorXPos;       ///< The global X-coordinate of the cursor.
    int         m_cursorYPos;       ///< The global Y-coordinate of the cursor (student line).
    int         m_cursorHeight;     ///< The height of the cursor.
    QColor      m_cursorColor;      ///< Defines the colour of the cursor (when turned on).
    QColor      m_cursorBackground; ///< Defines the background colour of the cursor (when turned off).
};

#endif  // KTOUCHSLIDELINE_H
