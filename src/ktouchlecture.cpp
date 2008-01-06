/***************************************************************************
 *   ktouchlecture.cpp                                                     *
 *   -----------------                                                     *
 *   Copyright (C) 2000 by Håvard Frøiland haavard@users.sourceforge.net,  *
 *   2003 by Andreas Nicolai  ghorwin@users.sourceforge.net                *                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchlecture.h"

#include <QFile>
#include <QTextStream>
#include <QtXml/QDomDocument>

#include <kdebug.h>
#include <klocale.h>
#include <ktemporaryfile.h>
#include <kio/netaccess.h>

bool KTouchLecture::load(QWidget * window, const KUrl& url) {
    // Ok, first download the contents as usual using the KIO lib
    // File is only downloaded if not local, otherwise it's just opened
    QString target;
    bool result = false;
    if (KIO::NetAccess::download(url, target, window)) {
        // Ok, that was successful, store the lectureURL and read the file
        QFile infile(target);
        if ( !infile.open( QIODevice::ReadOnly ) )
            // FIXME : what about removing the tempfile?
            return false;   // Bugger it... couldn't open it...
        QTextStream in( &infile );
        result = readLecture(in);
    };
    KIO::NetAccess::removeTempFile(target);
    return result;
}
// ----------------------------------------------------------------------------

bool KTouchLecture::loadXML(QWidget * window, const KUrl& url) {
    // Ok, first download the contents as usual using the KIO lib
    // File is only downloaded if not local, otherwise it's just opened
    QString target;
    bool result = false;
    if (KIO::NetAccess::download(url, target, window)) {
        // Ok, that was successful, store the lectureURL and read the file
        QFile infile(target);
        if ( !infile.open( QIODevice::ReadOnly ) )
            // FIXME : What about removing the tempfile?
            return false;   // Bugger it... couldn't open it...
		QDomDocument doc;
		doc.setContent( &infile );
        result = readLectureXML(doc);
    }
    KIO::NetAccess::removeTempFile(target);
    return result;
}
// ----------------------------------------------------------------------------


bool KTouchLecture::saveXML(QWidget * window, const KUrl& url) const {
	// create the XML document
	QDomDocument doc;
	writeLectureXML(doc);

	// and save it
    QString tmpFile;
    KTemporaryFile *temp=0;
    if (url.isLocalFile())
        tmpFile=url.path();         // for local files the path is sufficient
    else {
        temp=new KTemporaryFile;         // for remote files create a temporary file first
        temp->open();
        tmpFile=temp->fileName();
    }

    QFile outfile(tmpFile);
    if ( !outfile.open( QIODevice::WriteOnly ) ) {
        if (temp)  delete temp;
        // kDebug() << "Error creating lecture file!";
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

const KTouchLevelData& KTouchLecture::level(unsigned int level_num) const {
    if (level_num >= static_cast<unsigned int>(m_lectureData.size()))
        level_num=0;
    return m_lectureData[level_num];
}
// ----------------------------------------------------------------------------

void KTouchLecture::setLevel(unsigned int level_num, const KTouchLevelData& level) {
    if (level_num >= static_cast<unsigned int>(m_lectureData.size()))
		m_lectureData.append(level);
	else
		m_lectureData[level_num] = level;
}
// ----------------------------------------------------------------------------

QSet<QChar> KTouchLecture::knownCharsInLevel(unsigned int level_num) const {
	// loop over all levels so far and create a set with all characters known
	QSet<QChar> knownChars;
	for (unsigned int i=0; i<=level_num && i<static_cast<unsigned int>(m_lectureData.count()); ++i) {
		const QString& s = m_lectureData[i].newChars();
		for (int j=0; j<s.count(); ++j) {
			knownChars.insert(s[j]);
		}
	}
	QString chars;
	foreach (const QChar &ch, knownChars)
		chars.append(ch);
	kDebug() << "Known chars: " << chars;
	return knownChars;
}
// ----------------------------------------------------------------------------

bool KTouchLecture::readLecture(QTextStream& in) {
    //kDebug() << "[KTouchLecture::loadLecture]  Reading lecture file '" << lectureURL.url() << "'!";
    QString line;
    // remove everything else
    m_lectureData.clear();
    // now loop until end of file is reached and break down the textfile into several strings containing the levels
    QStringList slist;
    QString current_level;  // used to store the current level data
    line = in.readLine();
    bool in_level = false;
    while (!in.atEnd() && !line.isNull()) {
        // only consider non-empty lines
        if (!line.isEmpty()) {
            // lecture title?
            if (line.indexOf("# Title:") == 0)
                m_title = line.right(line.length() - 8).trimmed();
            else if (line[0]!='#' || line.contains("# Comment:")) {
                // ok, after all those comment lines, we finally found the beginn of a level
                in_level = true;
                current_level += line + '\n';
            }
            else if (in_level) {
                // ok, a new comment found, if we were reading a level, store it
                slist.append(current_level);
                current_level.clear();
                in_level = false;
            }
        }
        line = in.readLine();
    }
    if (!current_level.isEmpty() && in_level)
        slist.append(current_level);

    //kDebug() << "Levels read = " << slist.count();

    // now read all the levels
    for (QStringList::Iterator it = slist.begin(); it!=slist.end(); ++it) {
        // create new level
        KTouchLevelData level;
        QTextStream t(&(*it), QIODevice::ReadOnly);
        // try to read it
        if (!level.readLevel(t)) {
            // uh oh, error while reading level data
            createDefault();
            return false;
        }
        // add it (object will be deleted by the list)
        m_lectureData.push_back(level);
    }

    if (m_lectureData.size()>1)
        return true;  // all ok
    else {
        // Hmm, no levels in the file. So we create our default mini level and report an error.
        createDefault();
        return false;
    }
}
// ----------------------------------------------------------------------------

bool KTouchLecture::readLectureXML(QDomDocument& doc) {
    QString line;
    m_lectureData.clear();      // clean current data
	// retrieve the title
	QDomNodeList entries = doc.elementsByTagName("Title");
	if (entries.count() >= 1)	m_title = i18nc("Lesson Name", entries.item(0).firstChild().nodeValue().toUtf8());
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
	for (int i=0; i<entries.count(); ++i) {
        KTouchLevelData level;
		level.readLevelXML(entries.item(i));
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

void KTouchLecture::writeLectureXML(QDomDocument& doc) const {
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
    for (QList<KTouchLevelData>::const_iterator it=m_lectureData.begin();
		it!=m_lectureData.end(); ++it)
	{
		it->writeLevelXML(doc, levels);
	}
}
// ----------------------------------------------------------------------------

