/***************************************************************************
 *   ktouchlecture.h                                                       *
 *   ---------------                                                       *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KTOUCHLECTURE_H
#define KTOUCHLECTURE_H

#include <qvaluevector.h>
#include <kurl.h>
#include "ktouchleveldata.h"

/** This class handles the lecture data and provides the lines to type.
 *  The 'lecture' is actually the 'document' in the KTouch program. It
 *  contains the level data (see KTouchLevelData).A lecture object
 *  contains <b>ALWAYS</b> at least one lecture.<p>
 *  The lecture data (excluding the current training level)
 *  can be read and written using the member functions loadLecture() and
 *  saveLecture(). The currently assigned filename will be returned
 *  via fileName(). If you need the complete URL then access m_lectureURL
 *  directly.<p>
 *  During a training session the program will occasionally need a new
 *  line of text. You can retrieve the data of a certain level using
 *  the member function level().
 */
class KTouchLecture {
  public:
    /// Constructor
    KTouchLecture();
    /// Loads a lecture from file
    bool loadLecture(const KURL& lectureURL);
    /// Saves the lecture data to the current lecture URL
    void saveLecture();
    /// Creates a default mini-lecture.
    void createDefault();
    /// Returns the filename of the current lecture file (convenience function).
    QString fileName()              const { return m_lectureURL.fileName(); };
    /// Returns the number of levels in the lecture
    unsigned int levelCount()       const { return m_lectureData.count(); };
    /// Returns whether the lecture has been modified or not.
    bool isModified()            const { return m_modified; };
    /// Sets the modified flag
    void setModified(bool flag)  { m_modified=flag; };
    /// Returns a reference to the data of the level.
    /// If the level number is out of range the function will always return
    /// the level 0.
    const KTouchLevelData& level(unsigned int levelNum) const;

    KURL        m_lectureURL;   ///< The full URL of the lecture file.

  private:

    QValueVector<KTouchLevelData>   m_lectureData;  ///< The lecture data.
    bool                            m_modified;     ///< Indicates whether the lecture data has been modified.

    /// The editor should be able to handle the internal lecture data (it's for convenience actually).
    friend class KTouchEditor;
};

#endif  // KTOUCHLECTURE_H
