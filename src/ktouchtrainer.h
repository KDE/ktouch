/***************************************************************************
 *   ktouchtrainer.h                                                       *
 *   ---------------                                                       *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHTRAINER_H
#define KTOUCHTRAINER_H

#include <qstring.h>
#include <qobject.h>
#include <qdatetime.h>
#include <qvaluelist.h>

#include "ktouchtrainingsession.h"

class QTimer;

class KTouchStatus;
class KTouchSlideLine;
class KTouchKeyboard;
class KTouchLecture;

/** This is the master trainer and does all the heavy work.
 *  The training object gets the already sorted "new key pressed" events from the main window
 *  and processes them. It manages the level and line adjustments, starts, stops, pauses and restarts
 *  a training, updates the widgets and and and...<p>
 *  Whether in training or not the user may switch levels using the level change buttons (if enabled)
 *  or change the lecture. Apart from the slots levelUp(), levelDown() the member functions goFirstLine()
 *  and goFirstLevel() are called (and basically they do what they are named after).<p>
 *  There is always a current training session. On a typical program start the training session is paused.
 *  When a training session is paused the timer is stopped and typed characters won't be accepted.
 *  When the user continues the training session we first wait for his first keypress (that's why we need
 *  the flag m_waiting) and after that we start the timer.<p>
 *  When the user starts a new training session the current session data will be added to the session history
 *  and the current session will be reset.<p>
 *  When the user quits the program his current training session is stored. During next startup this
 */
class KTouchTrainer : public QObject {
    Q_OBJECT
  public:
    /// Constructor.
    KTouchTrainer(KTouchStatus *status, KTouchSlideLine *slideLine, KTouchKeyboard *keyboard, KTouchLecture *lecture);
    /// Destructor
    virtual ~KTouchTrainer();
    /// Jumps to the first line in the current level (m_level) (Note: has no effect on the training session).
    void goFirstLine();
    /// Jumps to the first level, line 1 ((Note: has no effect on the training session).
    void goFirstLevel() { m_level=0; goFirstLine(); }
    /** Processes the character 'key', recalculates the variables and updates the widgets.
     *  If the training session was started and in waiting state the timer is started and the actuall training begins.
     */
    void keyPressed(QChar key);
    /// Will be called when a character has been removed and recalculates the variables and updates the widgets.
    void backspacePressed();
    /// Will be called when "enter" has been pressed (displays next line).
    void enterPressed();
    /// Reads the training session history.
    void readSessionHistory();
    /// Saves the training session history.
    void writeSessionHistory();

    unsigned int        m_level;                ///< Current level number (zero based).
    unsigned int        m_line;                 ///< Current line number (zero based).
    QString             m_teacherText;          ///< The currently displayed teacher text.
    QString             m_studentText;          ///< The currently typed student text.

    bool                m_waiting;              ///< Indicates when we are waiting for the first keypress after session start.
    bool                m_trainingPaused;       ///< Indicates whether we have paused the training session or not.
    QTimer             *m_trainingTimer;        ///< The timer responsable for updating the speed LCD widget.

    KTouchTrainingSession               m_session;          ///< The current training session.
    QValueList<KTouchTrainingSession>   m_sessionHistory;   ///< Contains the previous sessions.

  signals:
    /// Will be emitted when the statusbar message changes.
    void statusbarMessageChanged(const QString& text);
    /// Will be emitted whenever a char has been pressed.
    void statusbarStatsChanged(unsigned int correctChars, unsigned int totalChars, unsigned int words);

  public slots:
    /// Will be called when the level is increased.
    void levelUp();
    /// Will be called when the level is decreased.
    void levelDown();
    /// Starts a new training session.
    void startNewTrainingSession(bool keepLevel);
    /// Pauses the current training session.
    void pauseTraining();
    /// Continues the current training session.
    void continueTraining();

  private slots:
    /// Will be called regularly by the training timer, increases the elapsed time and updates the speed LCD widget.
    void timerTick();

  private:
    /// Sets up a new teacher line (depending on m_level and m_line).
    void newLine();
    /// Checks whether a training session is paused and doesn't accept typed chars (returns 'false' in this case).
    bool typingAllowed();

    KTouchStatus       *m_statusWidget;         ///< Pointer to the status widget on top of the main window.
    KTouchSlideLine    *m_slideLineWidget;      ///< Pointer to the sliding line widget.
    KTouchKeyboard     *m_keyboardWidget;       ///< Pointer to the keyboard widget.
    KTouchLecture      *m_lecture;              ///< Pointer to the lecture data.
};

#endif  // KTOUCHTRAINER_H
