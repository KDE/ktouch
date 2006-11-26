/***************************************************************************
 *   ktouchlecture.h                                                       *
 *   ---------------                                                       *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHLECTURE_H
#define KTOUCHLECTURE_H

class QWidget;
class QDomDocument;

#include <QList>
#include <QTextStream>

class KUrl;

#include "ktouchleveldata.h"

/// This class handles the lecture data and provides the lines to type.
///
/// It contains the level data (see KTouchLevelData). A lecture object
/// contains <b>ALWAYS</b> at least one lecture.<p>
/// The lecture data can be read and written using the member functions 
/// readLecture() and writeLecture().<p>
/// During a training session the program will occasionally need a new
/// line of text. You can retrieve the data of a certain level using
/// the member function level() and then the line in this level (see
/// KTouchLevelData).
class KTouchLecture {
  public:
    /// Default Constructor
    KTouchLecture() { createDefault(); };
    /// Creates a default mini-lecture.
    void createDefault();
    /// Loads a lecture from old file format (returns true if successful).
    bool load(QWidget * window, const KUrl& url);
    /// Loads a lecture (in XML format) from file (returns true if successful).
    bool loadXML(QWidget * window, const KUrl& url);
    /// Saves the lecture data to file (returns true if successful).
    bool saveXML(QWidget * window, const KUrl& url) const;
    /// Returns the number of levels in the lecture.
    unsigned int levelCount() const { return m_lectureData.size(); };
    /// Returns a reference to the data of the level.
    /// If the level number is out of range the function will always return
    /// the level 0.
    const KTouchLevelData& level(unsigned int level_num) const;
    /// Returns the title of the lecture.
    const QString& title() const { return m_title; }
    /// Sets the title of the lecture.
    void setTitle(const QString& title) { m_title = title; }
    /// Sets the level data
    void setLevel(unsigned int level_num, const KTouchLevelData& level);

	QString		m_fontSuggestions;  ///< Font suggestions for this lecture.

  private:
    /// Loads a lecture from file
    bool readLecture(QTextStream& in);
    /// Loads a lecture from file into an XML document
    bool readLectureXML(QDomDocument& doc);
    /// Saves the lecture data in the XML document
    void writeLectureXML(QDomDocument& doc) const;
  
    QList<KTouchLevelData>       	m_lectureData;  	///< The lecture data.
    QString                         m_title;        	///< The title of the lecture.
    QString                         m_comment;      	///< A comment.

    
    /// The editor should be able to handle the internal lecture data (it's for convenience actually).
    friend class KTouchLectureEditorDialog;
};

#endif  // KTOUCHLECTURE_H
