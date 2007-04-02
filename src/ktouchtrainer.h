/***************************************************************************
 *   ktouchtrainer.h                                                       *
 *   ---------------                                                       *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHTRAINER_H
#define KTOUCHTRAINER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qobject.h>

#include "ktouchstatisticsdata.h"

class QTimer;

class KTouchStatus;
class KTouchSlideLine;
class KTouchKeyboardWidget;
class KTouchLecture;

/// This is the master trainer object and does all the heavy work.
/// 
/// The training object gets the already filtered "new key pressed" events from the main window
/// and processes them. It manages the level and line adjustments, starts, pauses and restarts
/// a training, updates the widgets and and and...
/// <p>
/// Whether in training or not the user may switch levels using the level change buttons (if enabled)
/// or change the lecture. The slot levelUp() or levelDown() is called and the trainer is switching
/// levels accordingly.
/// <p>
/// There is always a current training session. On a typical program start the training session is paused.
/// When a training session is paused the timer is stopped and the program waits for the next keypress 
/// of the user and then the timer is restarted. Note that the Pause button (Pause action) is enabled/disabled from 
/// the main KTouch class.
/// <p>
/// When the user starts a new training session or quits the program the current level/session 
/// statistics data will be added to the user statistics and the stats will be cleared. If a new
/// level is started (during a session), only the level stats are stored and cleared, the session
/// statistics is kept. The only member functions, that add statistics data to the users statistics are
/// therefore storeTrainingStatistics() (for level and session stats) and statsChangeLevel() (stores only
/// level stats).
/// <p>
/// The trainer keeps record of the current level and session stats seperately and
/// stores them in the m_levelStats and m_sessionStats member variables. However, the word count is only
/// updated in these variables, once a slide line is completed (or a level/session/lecture is changed).
/// <p>
/// The member function updateWidgets() updates all training related widgets (status widget, slide line,
/// and the statusbar of the main window). In this member function also the word count of the current line
/// is updated.
class KTouchTrainer : public QObject {
    Q_OBJECT
  public:
    /// Constructor.
    KTouchTrainer(KTouchStatus *status, KTouchSlideLine *slideLine, KTouchKeyboardWidget *keyboard, KTouchLecture *lecture);
    /// Destructor
    virtual ~KTouchTrainer();
    /// Jumps to the first line in the current level (m_level) (Note: has no effect on the training statistics).
	/// The function resets the word count.
    void gotoFirstLine();
    /// Jumps to the first level, line 1 (Note: has no effect on the training statistics!).
	/// The function resets the word count.
    void gotoFirstLevel() { m_level=0; gotoFirstLine(); }
    /// Processes the character 'key' and updates the widgets and statistics.
    /// If the training session was started and in waiting state the timer is started and the actuall training begins.
    void keyPressed(QChar key);
    /// Will be called when a character has been removed (recalculates the variables and updates the widgets).
    void backspacePressed();
    /// Will be called when "enter" has been pressed (displays next line).
    void enterPressed();
    /// Will be called whenever the student text has changed and updates the slide line widget,
    /// the status widget and shows the next to be pressed key on the keyboard
    void updateWidgets();
    /// Starts a new training session for the current lecture.
    /// This function is called from class KTouch (when the "New" session action is executed).
	/// The function resets the statistics so the caller must ensure that the previous level
	/// and session statistics are properly stored.
	void startTraining(bool keepLevel);
    /// Pauses the current training session.
    /// This function is called from class KTouch (when the "Pause" action is executed).
	void pauseTraining();
    /// Continues the current training session.
    /// This function is called from class KTouch when a user presses a normal key while the training is in 
    /// "Pause" mode.
	void continueTraining();
	/// Stores the currently accumulated statistics in the statistics for the current user (but only, if
	/// some time has elapsed in the current training session). This function is called whenever a new
	/// training session is started or when the program is closed.
	void storeTrainingStatistics();
	/// Returns true, if the students line is correct so far.
	bool studentLineCorrect() const;
	/// Returns number of words that have been typed so far in the current line.
	unsigned int wordsInCurrentLine() const { return m_wordsInCurrentLine; }

	
    unsigned int        m_level;                ///< Current level number (zero based).
    unsigned int        m_line;                 ///< Current line number (zero based).

    QString             m_teacherText;          ///< The currently displayed teacher text.
    QString             m_studentText;          ///< The currently typed student text.

    bool                m_trainingPaused;       ///< Indicates whether we have paused the training session or not.
    QTimer             *m_trainingTimer;        ///< The timer responsable for updating the speed LCD widget.

	KTouchLevelStats	m_levelStats;			///< Contains the statistics for the current level alone.
	KTouchSessionStats	m_sessionStats;			///< Contains the statistics for the current session (multiple levels).

  public slots:
    /// Will be called when the level is increased.
    void levelUp();
    /// Will be called when the level is decreased.
    void levelDown();

  private slots:
    /// Will be called regularly by the training timer, increases the elapsed time and updates the speed LCD widget.
    void timerTick();

  private:
    /// Sets up a new teacher line (depending on m_level and m_line).
	/// The function resets the word count.
    void newLine();
    void levelAllComplete();
    void updateLevelChangeButtons();
	/// Updates the statusbar of the main window.
	void updateStatusBar() const;
	/// Updates the message in the statusbar of the main window.
	void updateStatusBarMessage(const QString& message) const;
	/// Updates the word count in the current student text line.
	void updateWordCount();
	/// Adds a correct character count to the current statistics.
	void statsAddCorrectChar(QChar key);
	/// Adds a wrong character count to the current statistics.
	void statsAddWrongChar(QChar key);
	/// Removes a correctly typed character count.
	void statsRemoveCorrectChar(QChar);
	/// Adds some elapsed time (in seconds) to the statistics.
	void statsAddTime(double dt);
	/// Stores level stats and resets level stats (this function is called from levelUp() and levelDown()
	/// right before a new level and new line is started).
	void statsChangeLevel();
	
    KTouchStatus       	   *m_statusWidget;         ///< Pointer to the status widget on top of the main window.
    KTouchSlideLine    	   *m_slideLineWidget;      ///< Pointer to the sliding line widget.
    KTouchKeyboardWidget   *m_keyboardWidget;       ///< Pointer to the keyboard widget.
    KTouchLecture      	   *m_lecture;              ///< Pointer to the lecture data.

    KURL             		m_levelUpSound;         ///< URL of the level up sound.
	KURL             		m_levelDownSound;       ///< URL of the level down sound.
	KURL             		m_typeWriterSound;      ///< URL of the typing sound.

	unsigned int			m_wordsInCurrentLine;	///< Number of words in the current typing line.
};

#endif  // KTOUCHTRAINER_H
