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

#include <tqfile.h>
#include <tqstringlist.h>
#include <tqdom.h>

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

TQTextStream& operator<<(TQTextStream &out, const KTouchCharStats &ch) {
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

bool KTouchLevelStats::read(TQDomNode in) {
	// read the level number
	TQDomNode n = in.attributes().namedItem("Number");
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
		TQString timestring = n.firstChild().nodeValue();
		if (timestring != TQString::null) 
			m_timeRecorded = TQDateTime::fromString(timestring, Qt::ISODate);
	}
	// read characters
	n = in.namedItem("CharStats");
	if (!n.isNull()) {
		TQString char_data = n.firstChild().nodeValue();
		//kdDebug() << "'" << char_data << "'" << endl;
#ifdef HAVE_SSTREAM
		std::stringstream strm(std::string(char_data.local8Bit()));
#else
		std::istrstream strm(char_data.local8Bit(), char_data.local8Bit().length());
#endif
		int ch, correct, wrong;
		while (strm >> ch >> correct >> wrong)
			m_charStats.insert( KTouchCharStats(TQChar(ch), correct, wrong) );
		//kdDebug() << m_charStats.size() << endl;
	}
	return true;
}
// ----------------------------------------------------------------------------

void KTouchLevelStats::write(TQDomDocument& doc, TQDomElement& root) const {
	TQDomElement level = doc.createElement("LevelStats");
	root.appendChild(level);
	level.setAttribute("Number", m_levelNum);
	level.setAttribute("Time", m_elapsedTime);
	level.setAttribute("Chars", m_totalChars);
	level.setAttribute("Corrects", m_correctChars);
	level.setAttribute("Words", m_words);
	// add time
	TQDomElement e = doc.createElement("Time");
	TQDomText tn = doc.createTextNode(m_timeRecorded.toString(Qt::ISODate));
	e.appendChild(tn);
	level.appendChild(e);
	// add char stats
	TQString char_data;
    // we append for each missed char the char-unicode and the counters
	for (std::set<KTouchCharStats>::const_iterator it=m_charStats.begin(); it!=m_charStats.end(); ++it)
		char_data += TQString(" %1 %2 %3").arg(it->m_char.unicode())
										 .arg(it->m_correctCount).arg(it->m_wrongCount);
	e = doc.createElement("CharStats");
	tn = doc.createTextNode(char_data);
	e.appendChild(tn);
	level.appendChild(e);
}
// ----------------------------------------------------------------------------

void KTouchLevelStats::addCorrectChar(TQChar key) {
	++m_correctChars;
	++m_totalChars;
    // we only count non-space characters
	if (key!=TQChar(' ')) {
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

void KTouchLevelStats::addWrongChar(TQChar key) {
	++m_totalChars;
	if (key==TQChar(8) || key==TQChar(' ')) 
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

bool KTouchSessionStats::read(TQDomNode in) {
	// read time
	TQDomNode n = in.attributes().namedItem("Time");
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
		TQString timestring = n.firstChild().nodeValue();
		if (timestring != TQString::null) 
			m_timeRecorded = TQDateTime::fromString(timestring, Qt::ISODate);
	}
	// read level numbers
	n = in.namedItem("LevelNums");
	if (!n.isNull()) {
		TQString str = n.firstChild().nodeValue();
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
		TQString char_data = n.firstChild().nodeValue();
		//kdDebug() << "'" << char_data << "'" << endl;
#ifdef HAVE_SSTREAM
		std::stringstream strm(std::string(char_data.local8Bit()));
#else
		std::istrstream strm(char_data.local8Bit(), char_data.local8Bit().length());
#endif
		int ch, correct, wrong;
		while (strm >> ch >> correct >> wrong)
			m_charStats.insert( KTouchCharStats(TQChar(ch), correct, wrong) );
		//kdDebug() << m_charStats.size() << endl;
	}

	return true;
}
// ----------------------------------------------------------------------------

void KTouchSessionStats::write(TQDomDocument& doc, TQDomElement& root) const {
	TQDomElement session = doc.createElement("SessionStats");
	root.appendChild(session);
	session.setAttribute("Time", m_elapsedTime);
	session.setAttribute("Chars", m_totalChars);
	session.setAttribute("Corrects", m_correctChars);
	session.setAttribute("Words", m_words);
	// add time
	TQDomElement e = doc.createElement("Time");
	TQDomText tn = doc.createTextNode(m_timeRecorded.toString(Qt::ISODate));
	e.appendChild(tn);
	session.appendChild(e);
	// add levels
	TQString level_nums;
	for (std::set<unsigned int>::const_iterator it = m_levelNums.begin(); it != m_levelNums.end(); ++it)
		level_nums += TQString( " %1").arg(*it);
	e = doc.createElement("LevelNums");
	tn = doc.createTextNode(level_nums);
	e.appendChild(tn);
	session.appendChild(e);
	// add char data
	TQString char_data;
    // we append for each missed char the char-unicode and the counters
	for (std::set<KTouchCharStats>::const_iterator it=m_charStats.begin(); it!=m_charStats.end(); ++it)
		char_data += TQString(" %1 %2 %3").arg(it->m_char.unicode())
				.arg(it->m_correctCount).arg(it->m_wrongCount);
	e = doc.createElement("CharStats");
	tn = doc.createTextNode(char_data);
	e.appendChild(tn);
	session.appendChild(e);
}
// ----------------------------------------------------------------------------

void KTouchSessionStats::addCorrectChar(TQChar key) {
	++m_correctChars;
	++m_totalChars;
    // we only count non-space characters
	if (key!=TQChar(' ')) {
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

void KTouchSessionStats::addWrongChar(TQChar key) {
	++m_totalChars;
	if (key==TQChar(8) || key==TQChar(' ')) 
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
	m_lectureTitle = TQString::null;
	m_lectureURL = TQString::null;
	m_levelStats.clear();
	m_sessionStats.clear();
}
// ----------------------------------------------------------------------------

bool KTouchLectureStats::read(TQDomNode in) {
	clear();
	// read the URL
	TQDomNode n = in.namedItem("URL");
	if (!n.isNull()) m_lectureURL = n.firstChild().nodeValue();
	else return false; // no URL, statistics useless
	// read the Title
	n = in.namedItem("Title");
	if (!n.isNull()) m_lectureTitle = n.firstChild().nodeValue();
	// read the level stats
	n = in.namedItem("AllLevelStats");
	if (!n.isNull()) {
		// loop over all childs and read levels
		TQDomNode levelNode = n.firstChild();
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
		TQDomNode sessionNode = n.firstChild();
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

void KTouchLectureStats::write(TQDomDocument& doc, TQDomElement& root) const {
	if (m_lectureURL.isEmpty()) return; // Error here!
	TQDomElement lecture = doc.createElement("LectureStats");
	root.appendChild(lecture);
	// store lecture URL
	TQDomElement urlElement = doc.createElement("URL");
	TQDomText urlText = doc.createTextNode(m_lectureURL.url());
	urlElement.appendChild(urlText);
	lecture.appendChild(urlElement);
	// store title
	TQString title = m_lectureTitle;
	if (title.isEmpty()) 
		title = m_lectureURL.fileName();
	TQDomElement titleElement = doc.createElement("Title");
	TQDomText titleText = doc.createTextNode(title);
	titleElement.appendChild(titleText);
	lecture.appendChild(titleElement);
	// store level stats
	TQDomElement levelStatsElement = doc.createElement("AllLevelStats");
	lecture.appendChild(levelStatsElement);
	for (TQValueVector<KTouchLevelStats>::ConstIterator it = m_levelStats.begin(); 
		it != m_levelStats.end(); ++it)
	{
		it->write(doc, levelStatsElement);
	}
	// store session stats
	TQDomElement sessionStatsElement = doc.createElement("AllSessionStats");
	lecture.appendChild(sessionStatsElement);
	for (TQValueVector<KTouchSessionStats>::ConstIterator it = m_sessionStats.begin(); 
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

bool KTouchStatisticsData::read(TQWidget * window, const KURL& url) {
	if (url.isEmpty()) return false;
    // Ok, first download the contents as usual using the KIO lib
    // File is only downloaded if not local, otherwise it's just opened
    TQString target;
    bool result = false;
    if (KIO::NetAccess::download(url, target, window)) {
        // Ok, that was successful, store the lectureURL and read the file
        TQFile infile(target);
        if ( !infile.open( IO_ReadOnly ) ) {
    		KIO::NetAccess::removeTempFile(target);
            return false;   // Bugger it... couldn't open it...
		}
		TQDomDocument doc;
		
#ifdef COMPRESSED_XML_STATISTICS
		TQByteArray array;
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

bool KTouchStatisticsData::write(TQWidget * window, const KURL& url) const {
	// create the XML document
	TQDomDocument doc;
	writeStats(doc);

	// and save it
    TQString tmpFile;
    KTempFile *temp=0;
    if (url.isLocalFile())
        tmpFile=url.path();         // for local files the path is sufficient
    else {
        temp=new KTempFile;         // for remote files create a temporary file first
        temp->setAutoDelete(true);  // so we don't have to delete the file ourselves
        tmpFile=temp->name();
    }

    TQFile outfile(tmpFile);
    if ( !outfile.open( IO_WriteOnly ) ) {
        if (temp)  delete temp;
        // kdDebug() << "Error creating lecture file!" << endl;
        return false;
    }
	
#ifdef COMPRESSED_XML_STATISTICS
	TQByteArray array;
	TQTextStream out(array, IO_WriteOnly);
	out << doc.toString();
	array = qCompress(array);
	outfile.writeBlock(array);
#else
	TQTextStream out( &outfile );
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

bool KTouchStatisticsData::readStats(TQDomDocument& doc) {
	clear(); // clear current data
	// retrieve the KTouchStatistics statistics
	TQDomNodeList entries = doc.elementsByTagName("LectureStats");
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

void KTouchStatisticsData::writeStats(TQDomDocument& doc) const {
	TQDomElement root = doc.createElement( "KTouchStatistics" );
	doc.appendChild(root);
	// Store Lecture statistics
	LectureStatsMap::ConstIterator it = m_lectureStats.begin();
	while (it != m_lectureStats.end()) {
		it.data().write(doc, root);
		++it;
	}
}
// ----------------------------------------------------------------------------

