/***************************************************************************
 *   ktouchtrainingsession.cpp                                             *
 *   -------------------------                                             *
 *   Copyright (C) 2003 by Andreas Nicolai                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHTRAININGSESSION_H
#define KTOUCHTRAININGSESSION_H

#include <set>              // I'm using std::set here because QT is missing this container type
#include <list>             // std::list because it has a fast sort() member function
#include "ktouchcharstats.h"

class QTextStream;

/** Contains the data of one training session.
 *  The data is collection in sessions to simplify the generation of statistics. In each session
 *  the average typing speed, the number of characters (correct and total) and all missed characters
 *  will be stored. Missed characters are stored when a user does not type the required character but
 *  something else. This allows evaluation of the most often missed char to focus on.
 */
class KTouchTrainingSession {
  public:
    /// Default constructor.
    KTouchTrainingSession() : m_totalChars(0), m_correctChars(0), m_words(0), m_elapsedTime(0), m_wordBuffer("") {};
    /// Constructor which creates a training session from a string (see asString() ).
    KTouchTrainingSession(QString str);
    /// Resets the session to the initial state
    void reset();
    /// Returns the session data as string use for writing the current session stats (session management).
    QString asString() const;
    /// Updates the status variables when a correct key was pressed.
    void addCorrectChar(QChar ch);
    /// Updates the status variables when a wrong key was pressed.
    void addWrongChar(QChar missedChar);
    /// Returns the typing accuracy or "correctness" (0..1).
    double correctness() const { return (m_totalChars==0) ? 1.0 : static_cast<double>(m_correctChars)/m_totalChars; };
    /// Returns the average typing speed in chars per minute.
    double charSpeed() const { return (m_elapsedTime==0) ? 0 : 60000.0*static_cast<double>(m_correctChars)/m_elapsedTime; };
    /// Returns the average typing speed in words per minute.
    double wordSpeed() const { return (m_elapsedTime==0) ? 0 : 60000.0*static_cast<double>(m_words)/m_elapsedTime; };
    /// Returns a value list of the characters stats sorted after the worst miss/hit ratio.
    std::list<KTouchCharStats> sortedCharStats() const;

    unsigned int                m_totalChars;       ///< The total number of chars typed this session.
    unsigned int                m_correctChars;     ///< The total number of correctly typed chars.
    unsigned int                m_words;            ///< The number of typed words.
    unsigned int                m_elapsedTime;      ///< Training time in this session in seconds.
    std::set<KTouchCharStats>   m_charStats;        ///< Holds the characters counters

  private:
    QString                     m_wordBuffer;       ///< Used for the word count.
};

#endif  // KTOUCHTRAININGSESSION_H
