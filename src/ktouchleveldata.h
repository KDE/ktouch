/***************************************************************************
 *   ktouchleveldata.h                                                     *
 *   -----------------                                                     *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHLEVELDATA_H
#define KTOUCHLEVELDATA_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qstring.h>
#include <qtextstream.h>
#include <qvaluevector.h>
#include <qdom.h>

class QDomDocument;
class QDomElement;

/// This class contains the data of one level in a lecture and provides
/// access to the lines of that level.
///
/// A lecture (KTouchLecture) typically contains multiple levels. Each
/// level contains a level description, a string with the
/// newly introduced characters and several lines of training text.
/// There is ALWAYS at least one line of training text in the level!<p>
/// During a training session you might want to retrieve a certain line of
/// the level. You can use the member function line(lineNumber) to get a
/// line of the level.
/// The member function count() returns the number of lines in this level.
class KTouchLevelData {
  public:
    /// Default constructor, creates a default level.
    KTouchLevelData() { createDefault(); }
    /// Constructor for creating a new level.
    KTouchLevelData(const QString& comment, const QString& newChars)
        : m_comment(comment), m_newChars(newChars) { createDefault(); }
    /// Returns the level description (comment).
    const QString& comment() const { return m_comment; }
    /// Returns the new characters string.
    const QString& newChars() const { return m_newChars; }
    /// Returns the line with number 'lineNumber' (0 means first line).
    ///
    /// If the line number is out of range, the first line will be returned.
    const QString& line(unsigned int lineNumber) const;
    /// Returns the number of lines.
    unsigned int count() const { return m_lines.size(); }

  private:
    /// Creates a default level.
    void createDefault();
    /// Reads a level from the input stream.
    /// @return Returns 'true', when a level could be read successfully, or 'false' otherwise.
    bool readLevel(QTextStream& in);
    /// Reads a level from a XML Dom Node.
    /// @return Returns 'true', when a level could be read successfully, or 'false' otherwise.
    bool readLevel(QDomNode in);
    /// Writes the level data into the output stream.
    void writeLevel(QTextStream& out) const;
    /// Writes the level data into the output stream.
    void writeLevel(QDomDocument& doc, QDomElement& root) const;

    QString                 m_comment;      ///< A comment for the level.
    QString                 m_newChars;     ///< Contains the newly introduced characters in this level.
    QValueVector<QString>   m_lines;        ///< The lines of the level.

    // so I don't have to allow access to read/write function to everybody :-)
    friend class KTouchLecture;
    friend class KTouchLectureEditor;
};

#endif // KTOUCHLEVELDATA_H
