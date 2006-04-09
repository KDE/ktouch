/***************************************************************************
 *   ktouchlecture.cpp                                                     *
 *   -----------------                                                     *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchlecture.h"

#include <qfile.h>
#include <qtextstream.h>
#include <qstringlist.h>
#include <qdom.h>

#include <kdebug.h>
#include <klocale.h>
#include <ktempfile.h>
#include <kio/netaccess.h>

bool KTouchLecture::load(QWidget * window, const KURL& url) {
    // Ok, first download the contents as usual using the KIO lib
    // File is only downloaded if not local, otherwise it's just opened
    QString target;
    bool result = false;
    if (KIO::NetAccess::download(url, target, window)) {
        // Ok, that was successful, store the lectureURL and read the file
        QFile infile(target);
        if ( !infile.open( IO_ReadOnly ) ) {
    		KIO::NetAccess::removeTempFile(target);
            return false;   // Bugger it... couldn't open it...
		}
        QTextStream in( &infile );
        result = readLecture(in);
    };
    KIO::NetAccess::removeTempFile(target);
    return result;
}
// ----------------------------------------------------------------------------

bool KTouchLecture::loadXML(QWidget * window, const KURL& url) {
    // Ok, first download the contents as usual using the KIO lib
    // File is only downloaded if not local, otherwise it's just opened
    QString target;
    bool result = false;
    if (KIO::NetAccess::download(url, target, window)) {
        // Ok, that was successful, store the lectureURL and read the file
        QFile infile(target);
        if ( !infile.open( IO_ReadOnly ) ) {
		    KIO::NetAccess::removeTempFile(target);
            return false;   // Bugger it... couldn't open it...
		}
		QDomDocument doc;
		doc.setContent( &infile );
        result = readLecture(doc);
    }
    KIO::NetAccess::removeTempFile(target);
    return result;
}
// ----------------------------------------------------------------------------


bool KTouchLecture::saveXML(QWidget * window, const KURL& url) const {
	// create the XML document
	QDomDocument doc;
	writeLecture(doc);

	// and save it	
    QString tmpFile;
    KTempFile *temp=0;
    if (url.isLocalFile())
        tmpFile=url.path();         // for local files the path is sufficient
    else {
        temp=new KTempFile;         // for remote files create a temporary file first
        temp->setAutoDelete(true);  // so we don't have to delete the file ourselves
        tmpFile=temp->name();
    }

    QFile outfile(tmpFile);
    if ( !outfile.open( IO_WriteOnly ) ) {
        if (temp)  delete temp;
        // kdDebug() << "Error creating lecture file!" << endl;
        return false;
    };
	
    QTextStream out( &outfile );
    out << doc.toString();
    outfile.close();
    // if we have a temporary file, we still need to upload it
    if (temp) {
        KIO::NetAccess::upload(tmpFile, url, window);
        delete temp;
    }
    return true;
}
// ----------------------------------------------------------------------------

void KTouchLecture::createDefault() {
    m_lectureData.clear();      // remove everything else
    m_title = i18n("A default lecture...");
    m_fontSuggestions = "Monospace";
    KTouchLevelData miniLevel;  // create the level which is by default a mini-level
    m_lectureData.push_back( miniLevel );
}
// ----------------------------------------------------------------------------

const KTouchLevelData& KTouchLecture::level(unsigned int levelNum) const {
    if (levelNum>=m_lectureData.size())
        levelNum=0;
    return m_lectureData[levelNum];
}
// ----------------------------------------------------------------------------

bool KTouchLecture::readLecture(QTextStream& in) {
    //kdDebug() << "[KTouchLecture::loadLecture]  Reading lecture file '" << lectureURL.url() << "'!" << endl;
    QString line;
    // remove everything else
    m_lectureData.clear();      
    // now loop until end of file is reached and break down the textfile into several strings containing the levels
    QStringList slist;
    QString current_level = QString::null;  // used to store the current level data
    line = in.readLine();
    bool in_level = false;
    while (!in.atEnd() && !line.isNull()) {
        // only consider non-empty lines
        if (!line.isEmpty()) {
            // lecture title?
            if (line.find("# Title:") == 0)
                m_title = line.right(line.length() - 8).stripWhiteSpace();
            else if (line[0]!='#' || line.find("# Comment:")!=-1) {
                // ok, after all those comment lines, we finally found the beginn of a level
                in_level = true;
                current_level += line + '\n';
            }
            else if (in_level) {
                // ok, a new comment found, if we were reading a level, store it
                slist.append(current_level);
                current_level = QString::null;
                in_level = false;
            }
        }
        line = in.readLine();
    };
    if (!current_level.isEmpty() && in_level)
        slist.append(current_level);

    //kdDebug() << "Levels read = " << slist.count() << endl;

    // now read all the levels
    for (QStringList::Iterator it = slist.begin(); it!=slist.end(); ++it) {
        // create new level
        KTouchLevelData level;
        QTextStream t(&(*it), IO_ReadOnly);
        // try to read it
        if (!level.readLevel(t)) {
            // uh oh, error while reading level data
            createDefault();
            return false;
        };
        // add it (object will be deleted by the list)
        m_lectureData.push_back(level);
    }
        
    if (m_lectureData.size()>1)
        return true;  // all ok
    else {
        // Hmm, no levels in the file. So we create our default mini level and report an error.
        createDefault();
        return false;
    };
}
// ----------------------------------------------------------------------------

bool KTouchLecture::readLecture(QDomDocument& doc) {
    QString line;
    m_lectureData.clear();      // clean current data
	// retrieve the title
	QDomNodeList entries = doc.elementsByTagName("Title");
	if (entries.count() >= 1)	m_title = entries.item(0).firstChild().nodeValue();
	else						m_title = i18n("untitled lecture");
	// retrieve the comment
	entries = doc.elementsByTagName("Comment");
	if (entries.count() >= 1)
		m_comment = entries.item(0).firstChild().nodeValue();
	// retrieve the font suggestion
	entries = doc.elementsByTagName("FontSuggestions");
	if (entries.count() >= 1)
		m_fontSuggestions = entries.item(0).firstChild().nodeValue();
	// retrieve the levels
	entries = doc.elementsByTagName("Level");
	for (unsigned int i=0; i<entries.count(); ++i) {
        KTouchLevelData level;
		level.readLevel(entries.item(i));
        m_lectureData.push_back(level);
	}
    if (m_lectureData.size()>0)
        return true;  // all ok
    else {
        // Hmm, no levels in the file. So we create our default mini level and report an error.
        createDefault();
        return false;
    };
}
// ----------------------------------------------------------------------------

void KTouchLecture::writeLecture(QDomDocument& doc) const {
    QDomElement root = doc.createElement( "KTouchLecture" );
    doc.appendChild(root);
	// Store title and ensure that the file contains a title!
	QDomElement title = doc.createElement("Title");
	QDomText titleText;
	if (m_title.isEmpty())	titleText = doc.createTextNode( i18n("untitled lecture") );
	else					titleText = doc.createTextNode(m_title);
	title.appendChild(titleText);
	root.appendChild(title);
	// Store comment if given
	if (!m_comment.isEmpty()) {
		QDomElement comment = doc.createElement("Comment");
		QDomText commentText = doc.createTextNode(m_comment);
		comment.appendChild(commentText);
		root.appendChild(comment);
	}
	// Store font suggestion if given
	if (!m_fontSuggestions.isEmpty()) {
		QDomElement font = doc.createElement("FontSuggestions");
		QDomText fontText = doc.createTextNode(m_fontSuggestions);
		font.appendChild(fontText);
		root.appendChild(font);
	}
	// Store levels
	QDomElement levels = doc.createElement("Levels");
	root.appendChild(levels);
    for (QValueVector<KTouchLevelData>::const_iterator it=m_lectureData.begin(); 
		it!=m_lectureData.end(); ++it) 
	{
		it->writeLevel(doc, levels);
	}
}
// ----------------------------------------------------------------------------



// OLD and deprecated stuff

/*
bool KTouchLecture::save(QWidget * window, const KURL& url) const {
    QString tmpFile;
    KTempFile *temp=0;
    if (url.isLocalFile())
        tmpFile=url.path();         // for local files the path is sufficient
    else {
        temp=new KTempFile;         // for remote files create a temporary file first
        temp->setAutoDelete(true);  // so we don't have to delete the file ourselves
        tmpFile=temp->name();
    }

    QFile outfile(tmpFile);
    if ( !outfile.open( IO_WriteOnly ) ) {
        if (temp)  delete temp;
        // kdDebug() << "Error creating lecture file!" << endl;
        return false;
    };
    QTextStream out( &outfile );
    writeLecture(out);
    // TODO : check stream status to see if save worked
    outfile.close();
    // if we have a temporary file, we still need to upload it
    if (temp) {
        KIO::NetAccess::upload(tmpFile, url, window);
        delete temp;
    }
    return true;
}
// ----------------------------------------------------------------------------

void KTouchLecture::writeLecture(QTextStream& out) const {
    out << "###################################### "  << endl;
    out << "#                                    # "  << endl;
    out << "#  Training lecture file for KTouch  # "  << endl;
    out << "#                                    # "  << endl;
    out << "###################################### "  << endl << endl;
    out << "# Title: " << m_title << endl;
    out << endl;

    int levelCounter=0;
    for (QValueVector<KTouchLevelData>::const_iterator it=m_lectureData.begin(); it!=m_lectureData.end(); ++it) {
        out << "################################" << endl;
        out << "# Level: " << ++levelCounter << endl;
        out << "# " << endl;
        it->writeLevel(out);
    };
    out << endl;
}
// ----------------------------------------------------------------------------

*/
