/***************************************************************************
 *   ktouchlecture.cpp                                                     *
 *   -----------------                                                     *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchlecture.h"

#include <qfile.h>
#include <qtextstream.h>

#include <kdebug.h>
#include <ktempfile.h>
#include <klocale.h>
#include <kio/netaccess.h>

KTouchLecture::KTouchLecture() {
    createDefault();
    // There must be always at least one level in the lecture since KTouch won't work
    // without lecture data. Or at least it would require a hell of work to program
    // everywhere exceptional behavior. And it's a LOT safer that way!!!
};


bool KTouchLecture::loadLecture(const KURL& lectureURL) {
    //kdDebug() << "[KTouchLecture::loadLecture]  Reading lecture file '" << lectureURL.url() << "'!" << endl;
    // Ok, first download the contents as usual using the KIO lib
    // NOTE: file is only downloaded if not local, otherwise it's just opened
    QString target;
    if (KIO::NetAccess::download(lectureURL, target)) {
        // Ok, that was successful, store the lectureURL and read the file
        QFile infile(target);
        if ( !infile.open( IO_ReadOnly ) )
            return false;   // Bugger it... couldn't open it...
        QTextStream in( &infile );
        QString line, lastline;
        // Empties the vector and create a default level so that when the reading fails at least
        // one level remains. If the reading was ok, we simply remove that in the end :-)
        createDefault();
        // now loop until end of file is reached
        do {
            // skip all empty lines or lines containing a comment (starting with '#')
            do {
                lastline = line;
                line = in.readLine();  }
            while (!line.isNull() && (line.isEmpty() || line[0]=='#'));

            // Check if end of file encountered and if that is the case -> bail out at next while
            if (line.isNull())  continue;

            // 'lastline' should now contain the optional level description
            // 'line' should now contain the "new chars"-line
            KTouchLevelData level;                          // create new level
            if (!level.readLevel(in, line, lastline)) {     // read it
                // uh oh, error while reading level data
                return false;
            };
            m_lectureData.append(level);                    // add it (object will be deleted by the list)
        } while (!in.atEnd() && !line.isNull());

        // finally remove the temporary file (if the file was actually downloaded, otherwise there
        // never was a temp file)
        KIO::NetAccess::removeTempFile(target);

        m_modified = false;
        if (m_lectureData.count()>1) {
            // Yippie, all went well... remove the default mini level
            m_lectureData.erase(m_lectureData.begin());
            m_lectureURL = lectureURL;
            return true;
        }
        else {
            // Hmm, no levels in the file. So we keep our default mini level and report an error.
            m_lectureURL="";
            return false;
        };
    }
    else
        return false;  // couldn't download -> couldn't read level and the current level didn't change!
}


void KTouchLecture::saveLecture() {
    QString tmpFile;
    KTempFile *temp=0;
    if (m_lectureURL.isLocalFile())
        tmpFile=m_lectureURL.path();    // for local files the path is sufficient
    else {
        temp=new KTempFile;             // for remote files create a temporary file first
        tmpFile=temp->name();
    }

    QFile outfile(tmpFile);
    if ( !outfile.open( IO_WriteOnly ) ) {
        if (temp)  delete temp;
        return;
    };

    QTextStream out( &outfile );
    out << "################################## "  << endl;
    out << "#                                # "  << endl;
    out << "#  Trainingfile for KTouch       # "  << endl;
    out << "#             (Håvard Frøiland)  # "  << endl;
    out << "################################## "  << endl;
    out << endl;

    int levelCounter=0;
    for (QValueVector<KTouchLevelData>::const_iterator it=m_lectureData.begin(); it!=m_lectureData.end(); ++it) {
        out << "################################" << endl;
        out << "# Level: " << ++levelCounter << endl;
        out << "#" << endl;
        it->writeLevel(out);
    };

    if (temp) {
        KIO::NetAccess::upload(tmpFile, m_lectureURL);
        temp->unlink();
        delete temp;
    }
    m_modified = false;
};


void KTouchLecture::createDefault() {
    m_lectureURL = "";
    m_lectureData.clear();      // remove everything else
    KTouchLevelData miniLevel;  // create the level which is by default a mini-level
    m_lectureData.append( miniLevel );
    m_modified = false;
};


const KTouchLevelData& KTouchLecture::level(unsigned int levelNum) const {
    if (levelNum>=m_lectureData.count())
        levelNum=0;
    return m_lectureData[levelNum];
};
