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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

class QWidget;
class QDomDocument;
class KURL;

#include <qvaluevector.h>
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
    KTouchLecture() { createDefault(); }
    /// Creates a default mini-lecture.
    void createDefault();
    /// Loads a lecture from file (returns true if successful).
    bool load(QWidget * window, const KURL& url);
    /// Loads a lecture (in XML format) from file (returns true if successful).
    bool loadXML(QWidget * window, const KURL& url);
    // /// Saves the lecture data to file (returns true if successful).
    // bool save(QWidget * window, const KURL& url) const;
    /// Saves the lecture data to file (returns true if successful).
    bool saveXML(QWidget * window, const KURL& url) const;
    /// Returns the number of levels in the lecture.
    unsigned int levelCount()       const { return m_lectureData.size(); }
    /// Returns a reference to the data of the level.
    /// If the level number is out of range the function will always return
    /// the level 0.
    const KTouchLevelData& level(unsigned int levelNum) const;
    /// Returns the title of the lecture.
    const QString& title() const { return m_title; }
    /// Sets the title of the lecture.
    void setTitle(const QString& title) { m_title = title; }

    QString                         m_title;        	///< The title of the lecture.
    QString                         m_comment;      	///< A comment.
    QString                         m_fontSuggestions;  ///< Font suggestions for this lecture.

  private:
    /// Loads a lecture from file
    bool readLecture(QTextStream& in);
    /// Loads a lecture from file into an XML document
    bool readLecture(QDomDocument& doc);
    /// Saves the lecture data to the current lecture URL
    void writeLecture(QTextStream& out) const;
    /// Saves the lecture data in the XML document
    void writeLecture(QDomDocument& doc) const;
  
    QValueVector<KTouchLevelData>   m_lectureData;  	///< The lecture data.
    
    /// The editor should be able to handle the internal lecture data (it's for convenience actually).
    friend class KTouchLectureEditor;
};

#endif  // KTOUCHLECTURE_H
