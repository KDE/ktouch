/***************************************************************************
 *   ktouchstatisticsdata.cpp                                              *
 *   ------------------------                                              *
 *   Copyright (C) 2005 by Andreas Nicolai                                 *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "config.h"
#include "ktouchstatisticsdata.h"

#include <qfile.h>
#include <qstringlist.h>
#include <qdom.h>

#include <kdebug.h>
#include <klocale.h>
#include <ktempfile.h>
#include <kio/netaccess.h>

#ifdef HAVE_SSTREAM
#  include <sstream>
#else
#  include <strstream>
#endif

// remove this define if XML output should be made uncompressed
#define COMPRESSED_XML_STATISTICS


// *** KTouchCharStats ***

int KTouchCharStats::missHitRatio() const {
	if (m_correctCount+m_wrongCount == 0) return 0;
	return static_cast<int>((static_cast<double>(m_wrongCount)/(m_correctCount+m_wrongCount)*100));
}
// ----------------------------------------------------------------------------

QTextStream& operator<<(QTextStream &out, const KTouchCharStats &ch) {
    return (out << ch.m_char.unicode() << " " << ch.m_correctCount << " " << ch.m_wrongCount);
}
// ----------------------------------------------------------------------------



// *** KTouchLevelStats ***

void KTouchLevelStats::clear() {
	m_levelNum = -1;
	m_elapsedTime = 0;
	m_words = 0;
	m_totalChars = 0;
	m_correctChars = 0;
	m_charStats.clear();
}
// ----------------------------------------------------------------------------

bool KTouchLevelStats::read(QDomNode in) {
	// read the level number
	QDomNode n = in.attributes().namedItem("Number");
	bool ok;
	if (!n.isNull())  {
		m_levelNum = n.nodeValue().toInt(&ok);
		if (!ok) {
			m_levelNum = -1;
			return false; // need level number!
		}
	}
	// read time
	n = in.attributes().namedItem("Time");
	if (!n.isNull()) {
		m_elapsedTime = n.nodeValue().toDouble(&ok);
		if (!ok) m_elapsedTime = 0;
	}
	// read total chars
	n = in.attributes().namedItem("Chars");
	if (!n.isNull()) {
		m_totalChars = n.nodeValue().toInt(&ok);
		if (!ok) m_totalChars = 0;
	}
	// read correct chars
	n = in.attributes().namedItem("Corrects");
	if (!n.isNull()) {
		m_correctChars = n.nodeValue().toInt(&ok);
		if (!ok) m_correctChars = 0;
	}
	// read number of words
	n = in.attributes().namedItem("Words");
	if (!n.isNull()) {
		m_words = n.nodeValue().toInt(&ok);
		if (!ok) m_words = 0;
	}
	// done with attributes, read recording time
	n = in.namedItem("Time");
	if (!n.isNull()) {
		QString timestring = n.firstChild().nodeValue();
		if (timestring != QString::null) 
			m_timeRecorded = QDateTime::fromString(timestring, Qt::ISODate);
	}
	// read characters
	n = in.namedItem("CharStats");
	if (!n.isNull()) {
		QString char_data = n.firstChild().nodeValue();
		//kdDebug() << "'" << char_data << "'" << endl;
#ifdef HAVE_SSTREAM
		std::stringstream strm(std::string(char_data.local8Bit()));
#else
		std::istrstream strm(char_data.local8Bit(), char_data.local8Bit().length());
#endif
		int ch, correct, wrong;
		while (strm >> ch >> correct >> wrong)
			m_charStats.insert( KTouchCharStats(QChar(ch), correct, wrong) );
		//kdDebug() << m_charStats.size() << endl;
	}
	return true;
}
// ----------------------------------------------------------------------------

void KTouchLevelStats::write(QDomDocument& doc, QDomElement& root) const {
	QDomElement level = doc.createElement("LevelStats");
	root.appendChild(level);
	level.setAttribute("Number", m_levelNum);
	level.setAttribute("Time", m_elapsedTime);
	level.setAttribute("Chars", m_totalChars);
	level.setAttribute("Corrects", m_correctChars);
	level.setAttribute("Words", m_words);
	// add time
	QDomElement e = doc.createElement("Time");
	QDomText tn = doc.createTextNode(m_timeRecorded.toString(Qt::ISODate));
	e.appendChild(tn);
	level.appendChild(e);
	// add char stats
	QString char_data;
    // we append for each missed char the char-unicode and the counters
	for (std::set<KTouchCharStats>::const_iterator it=m_charStats.begin(); it!=m_charStats.end(); ++it)
		char_data += QString(" %1 %2 %3").arg(it->m_char.unicode())
										 .arg(it->m_correctCount).arg(it->m_wrongCount);
	e = doc.createElement("CharStats");
	tn = doc.createTextNode(char_data);
	e.appendChild(tn);
	level.appendChild(e);
}
// ----------------------------------------------------------------------------

void KTouchLevelStats::addCorrectChar(QChar key) {
	++m_correctChars;
	++m_totalChars;
    // we only count non-space characters
	if (key!=QChar(' ')) {
		std::set<KTouchCharStats>::iterator it = m_charStats.find( KTouchCharStats(key,0,0) );
		if (it==m_charStats.end())
			m_charStats.insert( KTouchCharStats(key,1,0) );
		else
			++(const_cast<KTouchCharStats&>(*it).m_correctCount);
        // this const_cast is necessary because the current gcc 3.2 has a bug in the
        // implementation of set::find(). The non-const overload is missing and find() always
        // returns a const object. Maybe in the next release this will be fixed. Until then
        // the const_cast helps.
	}
}
// ----------------------------------------------------------------------------

void KTouchLevelStats::removeCorrectChar() {
	--m_correctChars;
}
// ----------------------------------------------------------------------------

void KTouchLevelStats::addWrongChar(QChar key) {
	++m_totalChars;
	if (key==QChar(8) || key==QChar(' ')) 
		return; // don't remember wrong backspaces or spaces
	std::set<KTouchCharStats>::iterator it = m_charStats.find( KTouchCharStats(key,0,0) );
	if (it==m_charStats.end())
		m_charStats.insert( KTouchCharStats(key,0,1) );
	else
		++(const_cast<KTouchCharStats&>(*it).m_wrongCount);
        // this const_cast is necessary because the current gcc 3.2 has a bug in the
        // implementation of set::find(). The non-const overload is missing and find() always
        // returns a const object. Maybe in the next release this will be fixed. Until then
        // the const_cast helps.
}
// ----------------------------------------------------------------------------

double KTouchLevelStats::correctness() const {
	return (m_totalChars==0) ? 1.0 : static_cast<double>(m_correctChars)/m_totalChars; 
}
// ----------------------------------------------------------------------------

double KTouchLevelStats::charSpeed() const { 
	return (m_elapsedTime==0) ? 0 : 60.0*m_correctChars/m_elapsedTime; 
}
// ----------------------------------------------------------------------------

double KTouchLevelStats::wordSpeed() const { 
	return (m_elapsedTime==0) ? 0 : 60.0*m_words/m_elapsedTime; 
}
// ----------------------------------------------------------------------------



// *** KTouchSessionStats ***

void KTouchSessionStats::clear() {
	m_elapsedTime = 0;
	m_words = 0;
	m_totalChars = 0;
	m_correctChars = 0;
	m_levelNums.clear();
	m_charStats.clear();
}
// ----------------------------------------------------------------------------

bool KTouchSessionStats::read(QDomNode in) {
	// read time
	QDomNode n = in.attributes().namedItem("Time");
	bool ok;
	if (!n.isNull()) {
		m_elapsedTime = n.nodeValue().toDouble(&ok);
		if (!ok) m_elapsedTime = 0;
	}
	// read total chars
	n = in.attributes().namedItem("Chars");
	if (!n.isNull()) {
		m_totalChars = n.nodeValue().toInt(&ok);
		if (!ok) m_totalChars = 0;
	}
	// read correct chars
	n = in.attributes().namedItem("Corrects");
	if (!n.isNull()) {
		m_correctChars = n.nodeValue().toInt(&ok);
		if (!ok) m_correctChars = 0;
	}
	// read number of words
	n = in.attributes().namedItem("Words");
	if (!n.isNull()) {
		m_words = n.nodeValue().toInt(&ok);
		if (!ok) m_words = 0;
	}
	// done with attributes, read recording time
	n = in.namedItem("Time");
	if (!n.isNull()) {
		QString timestring = n.firstChild().nodeValue();
		if (timestring != QString::null) 
			m_timeRecorded = QDateTime::fromString(timestring, Qt::ISODate);
	}
	// read level numbers
	n = in.namedItem("LevelNums");
	if (!n.isNull()) {
		QString str = n.firstChild().nodeValue();
#ifdef HAVE_SSTREAM
		std::stringstream strm(std::string(str.local8Bit()));
#else
		std::istrstream strm(str.local8Bit(), str.local8Bit().length());
#endif
		int l;
		while (strm >> l)
			m_levelNums.insert(l);
	}
	// read characters
	n = in.namedItem("CharStats");
	if (!n.isNull()) {
		QString char_data = n.firstChild().nodeValue();
		//kdDebug() << "'" << char_data << "'" << endl;
#ifdef HAVE_SSTREAM
		std::stringstream strm(std::string(char_data.local8Bit()));
#else
		std::istrstream strm(char_data.local8Bit(), char_data.local8Bit().length());
#endif
		int ch, correct, wrong;
		while (strm >> ch >> correct >> wrong)
			m_charStats.insert( KTouchCharStats(QChar(ch), correct, wrong) );
		//kdDebug() << m_charStats.size() << endl;
	}

	return true;
}
// ----------------------------------------------------------------------------

void KTouchSessionStats::write(QDomDocument& doc, QDomElement& root) const {
	QDomElement session = doc.createElement("SessionStats");
	root.appendChild(session);
	session.setAttribute("Time", m_elapsedTime);
	session.setAttribute("Chars", m_totalChars);
	session.setAttribute("Corrects", m_correctChars);
	session.setAttribute("Words", m_words);
	// add time
	QDomElement e = doc.createElement("Time");
	QDomText tn = doc.createTextNode(m_timeRecorded.toString(Qt::ISODate));
	e.appendChild(tn);
	session.appendChild(e);
	// add levels
	QString level_nums;
	for (std::set<unsigned int>::const_iterator it = m_levelNums.begin(); it != m_levelNums.end(); ++it)
		level_nums += QString( " %1").arg(*it);
	e = doc.createElement("LevelNums");
	tn = doc.createTextNode(level_nums);
	e.appendChild(tn);
	session.appendChild(e);
	// add char data
	QString char_data;
    // we append for each missed char the char-unicode and the counters
	for (std::set<KTouchCharStats>::const_iterator it=m_charStats.begin(); it!=m_charStats.end(); ++it)
		char_data += QString(" %1 %2 %3").arg(it->m_char.unicode())
				.arg(it->m_correctCount).arg(it->m_wrongCount);
	e = doc.createElement("CharStats");
	tn = doc.createTextNode(char_data);
	e.appendChild(tn);
	session.appendChild(e);
}
// ----------------------------------------------------------------------------

void KTouchSessionStats::addCorrectChar(QChar key) {
	++m_correctChars;
	++m_totalChars;
    // we only count non-space characters
	if (key!=QChar(' ')) {
		std::set<KTouchCharStats>::iterator it = m_charStats.find( KTouchCharStats(key,0,0) );
		if (it==m_charStats.end())
			m_charStats.insert( KTouchCharStats(key,1,0) );
		else
			++(const_cast<KTouchCharStats&>(*it).m_correctCount);
        // this const_cast is necessary because the current gcc 3.2 has a bug in the
        // implementation of set::find(). The non-const overload is missing and find() always
        // returns a const object. Maybe in the next release this will be fixed. Until then
        // the const_cast helps.
	}
}
// ----------------------------------------------------------------------------

void KTouchSessionStats::removeCorrectChar() {
	--m_correctChars;
}
// ----------------------------------------------------------------------------

void KTouchSessionStats::addWrongChar(QChar key) {
	++m_totalChars;
	if (key==QChar(8) || key==QChar(' ')) 
		return; // don't remember wrong backspaces or spaces
	std::set<KTouchCharStats>::iterator it = m_charStats.find( KTouchCharStats(key,0,0) );
	if (it==m_charStats.end())
		m_charStats.insert( KTouchCharStats(key,0,1) );
	else
		++(const_cast<KTouchCharStats&>(*it).m_wrongCount);
        // this const_cast is necessary because the current gcc 3.2 has a bug in the
        // implementation of set::find(). The non-const overload is missing and find() always
        // returns a const object. Maybe in the next release this will be fixed. Until then
        // the const_cast helps.
}
// ----------------------------------------------------------------------------



// *** KTouchLectureStats ***

void KTouchLectureStats::clear() {
	m_lectureTitle = QString::null;
	m_lectureURL = QString::null;
	m_levelStats.clear();
	m_sessionStats.clear();
}
// ----------------------------------------------------------------------------

bool KTouchLectureStats::read(QDomNode in) {
	clear();
	// read the URL
	QDomNode n = in.namedItem("URL");
	if (!n.isNull()) m_lectureURL = n.firstChild().nodeValue();
	else return false; // no URL, statistics useless
	// read the Title
	n = in.namedItem("Title");
	if (!n.isNull()) m_lectureTitle = n.firstChild().nodeValue();
	// read the level stats
	n = in.namedItem("AllLevelStats");
	if (!n.isNull()) {
		// loop over all childs and read levels
		QDomNode levelNode = n.firstChild();
		while (!levelNode.isNull()) {
			//kdDebug() << "[KTouchLectureStats::read]  Reading level stats" << endl;
			KTouchLevelStats tmp;
			if (tmp.read(levelNode))  m_levelStats.push_back(tmp);
			levelNode = levelNode.nextSibling();
		}
	}
	// read the session stats
	n = in.namedItem("AllSessionStats");
	if (!n.isNull()) {
		// loop over all childs and read sessions
		QDomNode sessionNode = n.firstChild();
		while (!sessionNode.isNull()) {
			//kdDebug() << "[KTouchLectureStats::read]  Reading session stats" << endl;
			KTouchSessionStats tmp;
			if (tmp.read(sessionNode))  m_sessionStats.push_back(tmp);
			sessionNode = sessionNode.nextSibling();
		}
	}
	return true;
}
// ----------------------------------------------------------------------------

void KTouchLectureStats::write(QDomDocument& doc, QDomElement& root) const {
	if (m_lectureURL.isEmpty()) return; // Error here!
	QDomElement lecture = doc.createElement("LectureStats");
	root.appendChild(lecture);
	// store lecture URL
	QDomElement urlElement = doc.createElement("URL");
	QDomText urlText = doc.createTextNode(m_lectureURL.url());
	urlElement.appendChild(urlText);
	lecture.appendChild(urlElement);
	// store title
	QString title = m_lectureTitle;
	if (title.isEmpty()) 
		title = m_lectureURL.fileName();
	QDomElement titleElement = doc.createElement("Title");
	QDomText titleText = doc.createTextNode(title);
	titleElement.appendChild(titleText);
	lecture.appendChild(titleElement);
	// store level stats
	QDomElement levelStatsElement = doc.createElement("AllLevelStats");
	lecture.appendChild(levelStatsElement);
	for (QValueVector<KTouchLevelStats>::ConstIterator it = m_levelStats.begin(); 
		it != m_levelStats.end(); ++it)
	{
		it->write(doc, levelStatsElement);
	}
	// store session stats
	QDomElement sessionStatsElement = doc.createElement("AllSessionStats");
	lecture.appendChild(sessionStatsElement);
	for (QValueVector<KTouchSessionStats>::ConstIterator it = m_sessionStats.begin(); 
			it != m_sessionStats.end(); ++it)
	{
		it->write(doc, sessionStatsElement);
	}
}
// ----------------------------------------------------------------------------



// *** KTouchStatisticsData ***

void KTouchStatisticsData::clear() {
	m_lectureStats.clear();
}
// ----------------------------------------------------------------------------

bool KTouchStatisticsData::read(QWidget * window, const KURL& url) {
	if (url.isEmpty()) return false;
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
		
#ifdef COMPRESSED_XML_STATISTICS
		QByteArray array;
		array = infile.readAll();
		array = qUncompress(array);
		doc.setContent( array );
#else
		doc.setContent( &infile );
#endif // COMPRESSED_XML_STATISTICS
		result = readStats(doc);
	}
    KIO::NetAccess::removeTempFile(target);
    return result;
}
// ----------------------------------------------------------------------------

bool KTouchStatisticsData::write(QWidget * window, const KURL& url) const {
	// create the XML document
	QDomDocument doc;
	writeStats(doc);

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
    }
	
#ifdef COMPRESSED_XML_STATISTICS
	QByteArray array;
	QTextStream out(array, IO_WriteOnly);
	out << doc.toString();
	array = qCompress(array);
	outfile.writeBlock(array);
#else
	QTextStream out( &outfile );
    out << doc.toString();
#endif // COMPRESSED_XML_STATISTICS
    outfile.close();

    // if we have a temporary file, we still need to upload it
    if (temp) {
        KIO::NetAccess::upload(tmpFile, url, window);
        delete temp;
    }
    return true;
}
// ----------------------------------------------------------------------------

bool KTouchStatisticsData::readStats(QDomDocument& doc) {
	clear(); // clear current data
	// retrieve the KTouchStatistics statistics
	QDomNodeList entries = doc.elementsByTagName("LectureStats");
	bool result = true;
	unsigned int i=0;
	while (result && i < entries.count()) {
		KTouchLectureStats stats;
		result = stats.read(entries.item(i));
		// do we have a valid URL?
		if (stats.m_lectureURL.isEmpty())  result = false;
		if (result)
			m_lectureStats[stats.m_lectureURL] = stats;
		++i;
	}
	return result;
}
// ----------------------------------------------------------------------------

void KTouchStatisticsData::writeStats(QDomDocument& doc) const {
	QDomElement root = doc.createElement( "KTouchStatistics" );
	doc.appendChild(root);
	// Store Lecture statistics
	LectureStatsMap::ConstIterator it = m_lectureStats.begin();
	while (it != m_lectureStats.end()) {
		it.data().write(doc, root);
		++it;
	}
}
// ----------------------------------------------------------------------------

