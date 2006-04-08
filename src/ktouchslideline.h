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

#ifndef KTOUCHSLIDELINE_H
#define KTOUCHSLIDELINE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qwidget.h>
#include <qtimer.h>
class QPixmap;
class QPainter;

/// This widget just displays the sliding text.
/// 
/// KTouchSlideLine is a fire-and-forget widget, so you don't have to care about any
/// drawing details. After the initial setup (setting the font, colours etc.) you simple
/// need to call setNewText() when the teacher text changes or setStudentText() when
/// only the student line has changed. Everything else (including the
/// choice of the background colour) is done by the widget.
///
/// Please refer to the documentation 'sliding_line_widget.pdf' in the doc folder.
class KTouchSlideLine : public QWidget {
    Q_OBJECT
  public:
    /// Constructor
    KTouchSlideLine(QWidget *parent);
    /// Destructor, free memory allocated for the line pixmap.
    ~KTouchSlideLine();
    /// Applies the preferences (font and colours).
	/// This function calls setFont() which in turn results in a complete update
	/// of the widget and its sliding lines.
	/// @see setFont()
    void applyPreferences();
    /// Sets the teacher and student text (usually called when a new line is started).
	/// This function calls updateSlidingLines().
	/// @see updateSlidingLines()
    void setNewText(const QString& teacher_text, const QString& student_text);
    /// Sets the student text (called whenever the student string changes).
	/// This function calls updateStudentLine().
	/// @see updateStudentLine()
    void setStudentText(const QString& text);
	/// Changes the font of the slide line widget and updates everything else.
	/// This function is called when the font is changed. It updates the size
	/// of the sliding lines and the font size accordingly. Then it updates
	/// the widget.
	/// This function calls resizeFont().
	/// @see resizeFont()
	void setFont(const QFont& font);

	/// Returns true when the character can be added without problems.
	/// This function checks if the new possibly wrong student line
	/// would still fit into the student line pixmap and returns
	/// false if not.
	bool canAddCharacter(const QString& new_student_text);

  public slots:
    /// Starts or stops the cursor blinking timer.
    void setCursorTimerEnabled(bool on);

  private slots:
    /// Turns the cursor on or off (this function triggered by the cursor timer).
    void toggleCursor();
    /// Slides the lines into position (this function is triggered by the sliding timer).
    void slide();

  protected:
    /// Simply updates the widget.
	/// This function calls updateSlidingLines() if necessary, otherwise just slide().
	/// @see updateSlidingLines()
	/// @see slide()
    void paintEvent( QPaintEvent * );
    /// Will be called when the widget is resized.
    /// This event will first recalculate the geometry of the sliding lines.
	/// Then the font size will be updated and through that, the sliding lines will be
	/// updated.
    void resizeEvent ( QResizeEvent * );

  private:
    /// Calculates the correct text length (in pixels) taking trailing spaces into account
    int textLen(const QFontMetrics& fontMetrics, const QString& text);
    /// Will recalculate the font size and related variables depending on the height of the widget.
	/// This function calls updateSlidingLines().
	/// @see updateSlidingLines()
    void resizeFont();
    /// Just draws the cursor (if visible)
    void drawCursor();
    /// Draws the "enter" character at the given position (y is the y-position of the arrow).
    void drawEnterChar(QPainter *painter, int cursorPos, int y, int enterWidth);
    /// Recreates the student and teacher pixmaps and updates all related variables.
	/// In this function the teacher pixmap is drawn it will not be modified until the next
	/// call of updateSlidingLines(). This function also calls updateStudentLine().
	/// @see updateStudentLine()
    void updateSlidingLines();
	/// Redraws the student line alone and updates all related variables.
    void updateStudentLine();

    QFont       m_font;             ///< The font for the sliding lines.
    QString     m_teacherText;      ///< The teachers text.
    QString     m_studentText;      ///< The students text.
    QPixmap    *m_teacherPixmap;    ///< Pixmap used to draw the teacher sliding line, created in updateSlidingLines().
    QPixmap    *m_studentPixmap;    ///< Pixmap used to draw the student sliding line, created in updateSlidingLines().

    QTimer      m_slideTimer;       ///< This is the timer for the sliding of the lines.

    bool        m_cursorVisible;    ///< Flag which indicates the current state of the cursor.
    QTimer      m_cursorTimer;      ///< This is the cursor on/off timer.
    QColor      m_cursorColor;      ///< Defines the colour of the cursor (when turned on).
    QColor      m_cursorBackground; ///< Defines the background colour of the cursor (when turned off).

	// variables depending on size of widget, will be updated in resizeEvent()
	int			m_marginVerWidget;	///< Vertical margin between widget boundary and sliding line in pixels.
	int			m_slideLineDist;	///< Vertical distance between sliding lines in pixels.
	int			m_slideLineHeight;	///< Height of a sliding line in pixels.
	int			m_marginCursor;		///< The margin for cursor movement (minimum distance from left and right ends of slide line boundaries).

	// variables depending on font face and size, will be updated in the resizeFont() function
	int			m_xCharWidth;		///< The width (in pixel) of a small x (used to calculate space width and other stuff).
	int			m_spaceCharWidth;	///< The width of a space character (in pixel).
	int			m_cursorHeight;		///< Height of the cursor.
	int			m_yCursorStudent;   ///< Cursor top y coordinate (from top of visible area) in the student line.

	// newly defined variables, will be updated on the setNewText() and partially in setStudentText()
	int			m_marginHorWidget;	///< Horizontal margin between widget boundary and sliding line in pixels.
	int			m_slideLineWidth;	///< The length of the sliding line in pixels.
    int			m_cursorRangeLen;	///< Length (in pixel) that the cursor can move between left and right margin.

	int			m_teacherTextLen;   ///< The total length of the teacher line (in pixel) WITHOUT enter character.
	int			m_correctTextLen;   ///< The length of correctly typed text so far (in pixel).
	int			m_studentTextLen;	///< The total length of text typed (potentially wrong) in the student line (in pixel).

	int			m_xCursorTeacher;   ///< Cursor position (from left side of visible area) in the teacher line.
	int			m_xCursorTPixmap;   ///< Current cursor position in the teacher pixmap (from left in pixels).
	int			m_xCursorStudent;   ///< Cursor position (from left side of visible area) in the student line.
	int			m_xCursorSPixmap;   ///< Current cursor position in the student pixmap (from left in pixels).


    int         m_xFrameTeacher;    		///< The final X-position (position after slide is done) for the copy-frame in the teachers pixmap.
    double     	m_xFrameTeacherCurrent;   	///< The current X-position (position after slide is done) for the copy-frame in the teachers pixmap.

    int         m_xFrameStudent;    		///< The final X-position (position after slide is done) for the copy-frame in the students pixmap.
    double     	m_xFrameStudentCurrent;   	///< The current X-position (position after slide is done) for the copy-frame in the students pixmap.
};

#endif  // KTOUCHSLIDELINE_H
