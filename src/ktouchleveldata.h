/***************************************************************************
 *   ktouchleveldata.h                                                     *
 *   -----------------                                                     *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHLEVELDATA_H
#define KTOUCHLEVELDATA_H

#include <qstring.h>
#include <qvaluevector.h>
#include <qtextstream.h>

/** This class contains the data of one level in a lecture and provides
 *  access to the lines of that level.
 *  A lecture (KTouchLecture) typically contains multiple levels. Each
 *  level contains a level description, a string with the
 *  newly introduced characters and several lines of training text.
 *  There is ALWAYS at least one line of training text in the level!<p>
 *  During a training session you might want to retrieve a certain line of
 *  the level. You can use the member function line(lineNumber) to get a
 *  line of the level.
 *  The member function count() returns the number of lines in this level.
 */
class KTouchLevelData {
  public:
    /// Default constructor, necessary for list operations
    KTouchLevelData();
    /// Constructor for creating a new level.
    KTouchLevelData(const QString& comment, const QString& newChars);
    /// Returns the new characters string.
    const QString& newChars() const { return m_newChars; };
    /// Returns the line with number 'lineNumber'.
    /// If the line number is out of range, the first line will be returned.
    const QString& line(unsigned int lineNumber) const;
    /// Returns the number of lines.
    unsigned int lineCount() const { return m_lines.count(); };

  private:
    /// Creates a default level.
    void createDefault();
    /// Reads a level from the input stream (until empty line or EOF)
    /// Returns 'true', when a level could be read successfully, or 'false' otherwise.
    bool readLevel(QTextStream& in, const QString& newChars, const QString& comment);
    /// Writes the level data into the output stream
    void writeLevel(QTextStream& out) const;

    QString                 m_comment;      ///< A comment for the level
    QString                 m_newChars;     ///< Contains the newly introduced characters in this level
    QValueVector<QString>   m_lines;        ///< The lines of the level.
    unsigned int            m_currentLine;  ///< Indicates the current line in the level

    // so I don't have to allow access to read/write function to everybody :-)
    friend class KTouchLecture;
    friend class KTouchEditor;
};

#endif // KTOUCHLEVELDATA_H
