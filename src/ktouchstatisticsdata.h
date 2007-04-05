/***************************************************************************
 *   ktouchstatisticsdata.h                                                *
 *   ----------------------                                                *
 *   Copyright (C) 2005 by Andreas Nicolai                                 *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHSTATISTICSDATA_H
#define KTOUCHSTATISTICSDATA_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qstring.h>
#include <qvaluevector.h>
#include <qmap.h>
#include <qdom.h>
#include <qdatetime.h>

#include <kurl.h>

#include <set>          // I'm using std::set here because QT is missing this container type

class QWidget;


// This file contains all class declarations dealing with the statistics obtained in KTouch. 


// *** KTouchCharStats ***

/// Contains the stats for a single character.
///
/// Basically in this class the number of times the character has been pressed is counted. Both
/// the number of times it was correctly pressed and the number of times it was missed are stored
/// which allows to calculate a relative "hit-miss-ratio".
/// Character statistics are created for every character that has been missed at some time in the
/// training session.
class KTouchCharStats {
  public:
    /// Default constructor.
    KTouchCharStats() : m_char(0), m_correctCount(0), m_wrongCount(0) {}
    /// Constructor with parameters.
    KTouchCharStats(QChar ch, unsigned int correct, unsigned int wrong)
      : m_char(ch), m_correctCount(correct), m_wrongCount(wrong) {}
    /// Reads the character statistics from a XML Dom Node.
    /// @return Returns 'true', when reading was successful or 'false' otherwise.
    bool read(QDomNode in);
    /// Writes the character statistics to the XML document.
    void write(QDomDocument& doc, QDomElement& root) const;
    /// Returns the miss-hit ratio (a value between 0-all correct and 100-all wrong).
    int missHitRatio() const;

    QChar           m_char;         ///< The character for which statistics are kept.
    unsigned int    m_correctCount; ///< How often the character has been typed correctly.
    unsigned int    m_wrongCount;   ///< How often the character has been missed (not typed when it ought to be typed).
};

/// Sort criterion: Returns 'true' when the hit-miss ratio of 'lhs' is worse then the one of 'rhs'.
inline bool higher_miss_hit_ratio(const KTouchCharStats & lhs, const KTouchCharStats & rhs) {
	return lhs.missHitRatio() > rhs.missHitRatio();
}
/// Comparison operator "less", returns 'true' when the char-code of 'lhs' is less then the one of 'rhs'
inline bool operator<(const KTouchCharStats &lhs, const KTouchCharStats &rhs) { return lhs.m_char<rhs.m_char; }
/// Comparison operator "greater", returns 'true' when the char-code of 'lhs' is greater then the one of 'rhs'
inline bool operator>(const KTouchCharStats &lhs, const KTouchCharStats &rhs) { return lhs.m_char>rhs.m_char; }
/// Comparison operator == : returns 'true' when the char-code of 'lhs' is equal to then the one of 'rhs'
inline bool operator==(const KTouchCharStats &lhs, const KTouchCharStats &rhs) { return lhs.m_char==rhs.m_char; }
/// Writes the content of a KTouchCharStats object into the text stream.
QTextStream& operator<<(QTextStream &out, const KTouchCharStats &ch);

// *** KTouchLevelStats ***

/// Contains the complete statistics obtained for a single level.
///
/// This class contains the statistics for a single run through a single level. It contains the 
/// statistics for all characters that have been mistyped in this level, the typing time, the word count, 
/// correct and total character count etc.
class KTouchLevelStats {
  public:
	/// Default constructor
	  KTouchLevelStats() { clear(); }
	/// Clears the data
	void clear();

    /// Reads the level statistics from a XML Dom Node.
    /// @return Returns 'true', when reading was successful or 'false' otherwise.
    bool read(QDomNode in);
    /// Writes the level statistics to the XML document.
    void write(QDomDocument& doc, QDomElement& root) const;
	/// Adds a correct character count to the current statistics
	void addCorrectChar(QChar key);
	/// Removes correct character count
	void removeCorrectChar();
	/// Adds a wrong character count to the current statistics
	void addWrongChar(QChar key);
    /// Returns the typing accuracy or "correctness" as value between 0 and 1.
	double correctness() const;
    /// Returns the average typing speed in chars per minute.
	double charSpeed() const;
    /// Returns the average typing speed in words per minute.
	double wordSpeed() const;

    std::set<KTouchCharStats>   m_charStats;        ///< Holds the statistics for mistyped characters.
	int							m_levelNum;			///< Number of the level in the lecture.
    double                		m_elapsedTime;      ///< Typing time in this level in seconds.
    unsigned int                m_words;            ///< The number of typed words.
    unsigned int                m_totalChars;       ///< The total number of chars typed this level.
    unsigned int                m_correctChars;     ///< The total number of correctly typed chars.
	QDateTime					m_timeRecorded;		///< The time at which the level stats were recorded.
};



// *** KTouchSessionStats ***

/// Contains the complete statistics obtained for a single training session (spanning several levels).
///
/// This class contains the statistics for a complete training session of a single lecture but may include several levels.
class KTouchSessionStats {
  public:
	/// Default constructor
	  KTouchSessionStats() { clear(); }
	/// Clears the data
	void clear();

    /// Reads the session statistics from a XML Dom Node.
    /// @return Returns 'true', when reading was successful or 'false' otherwise.
    bool read(QDomNode in);
    /// Writes the session statistics to the XML document.
    void write(QDomDocument& doc, QDomElement& root) const;
	/// Adds a correct character count to the current statistics
	void addCorrectChar(QChar key);
	/// Removes a correct character count
	void removeCorrectChar();
	/// Adds a wrong character count to the current statistics
	void addWrongChar(QChar key);

	std::set<unsigned int>		m_levelNums;		///< Numbers of the levels in this session.
	std::set<KTouchCharStats>   m_charStats;        ///< Holds the statistics for mistyped characters.
	double                		m_elapsedTime;      ///< Typing time in this session in seconds.
	unsigned int                m_words;            ///< The number of typed words.
	unsigned int                m_totalChars;       ///< The total number of chars typed this session.
	unsigned int                m_correctChars;     ///< The total number of correctly typed chars.
	QDateTime					m_timeRecorded;		///< The time at which the session stats were recorded.
};


// *** KTouchLectureStats ***

/// Contains the complete statistics obtained over time for a certain training lecture.
///
/// This class contains the statistics for a complete training lecture including separate level statistics as well
/// as session statistics.
class KTouchLectureStats {
  public:
	/// Default constructor
	KTouchLectureStats() {}
	/// Clears the data
	void clear();

    /// Reads a lecture statistics from a XML Dom Node.
    /// @return Returns 'true', when reading was successful or 'false' otherwise.
    bool read(QDomNode in);
    /// Writes the lecture statistics to the XML document.
    void write(QDomDocument& doc, QDomElement& root) const;

	QString								m_lectureTitle;	///< The descriptive title of the lecture.
	KURL								m_lectureURL;	///< The descriptive title of the lecture.

	QValueVector<KTouchLevelStats>  	m_levelStats;	///< The statistics for all levels in this lecture.
	QValueVector<KTouchSessionStats>  	m_sessionStats;	///< The statistics for all session for in this lecture.
};


// *** KTouchStatisticsData ***

/// Contains the complete statistics for a certain user of KTouch.
///
/// All user statistics is kept in this class. The statistics data is read when KTouch is started. New 
/// statistics data is added when the lecture is changed or when the programm is closed. During
/// normal training sessions the statistics remains unchanged. During training the current level and 
/// session statistics is obtained by and stored in the KTouchTrainer object.
class KTouchStatisticsData {
  public:
	typedef QMap<KURL, KTouchLectureStats> 	LectureStatsMap;
	  
	/// Default constructor
	KTouchStatisticsData() {}
	/// Clears the data
	void clear();

    /// Reads the complete statistics from a XML file
    /// @return Returns 'true', when reading was successful or 'false' otherwise.
    bool read(QWidget * window, const KURL& url);
    /// Writes the statistics data to the XML document.
    bool write(QWidget * window, const KURL& url) const;
	
	LectureStatsMap		m_lectureStats;		///< All statistics of all lectures, stored based on their URLs.
	
  private:
    /// Reads the statistics data from file into an XML document
	bool readStats(QDomDocument& doc);
    /// Saves the statistics data in the XML document
	void writeStats(QDomDocument& doc) const;
};

#endif  // KTOUCHSTATISTICSDATA_H
