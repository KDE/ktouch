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

#include "ktouchstatisticsdata.h"

// QT includes

#include <QFile>
#include <QTextStream>
#include <QtXml/QDomNode>

// KDE includes

#include <kdebug.h>
#include <klocale.h>
#include <ktemporaryfile.h>
#include <kio/netaccess.h>

#include <sstream>

// remove this define if XML output should not be compressed (for debug purposes only)
// #define COMPRESSED_XML_STATISTICS  << dosn't work for me


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

bool KTouchLevelStats::read(const QDomNode &in) {
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
		if (!timestring.isEmpty()) 
			m_timeRecorded = QDateTime::fromString(timestring, Qt::ISODate);
	}
	// read characters
	n = in.namedItem("CharStats");
	if (!n.isNull()) {
		QString char_data = n.firstChild().nodeValue();
		//kDebug() << "'" << char_data << "'";
		// FIXME : get rid of stringstream here
		std::stringstream strm(std::string(char_data.toLocal8Bit()));
		int ch, correct, wrong;
		while (strm >> ch >> correct >> wrong)
			m_charStats.insert( KTouchCharStats(QChar(ch), correct, wrong) );
		//kDebug() << m_charStats.size();
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
	for (QSet<KTouchCharStats>::const_iterator it=m_charStats.begin(); it!=m_charStats.end(); ++it)
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
		QSet<KTouchCharStats>::iterator it = m_charStats.find( KTouchCharStats(key,0,0) );
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
	QSet<KTouchCharStats>::iterator it = m_charStats.find( KTouchCharStats(key,0,0) );
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

bool KTouchSessionStats::read(const QDomNode &in) {
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
		if (!timestring.isEmpty()) 
			m_timeRecorded = QDateTime::fromString(timestring, Qt::ISODate);
	}
	// read level numbers
	n = in.namedItem("LevelNums");
	if (!n.isNull()) {
		QString str = n.firstChild().nodeValue();
		// FIXME: get rid of stringstream here
		std::stringstream strm(std::string(str.toLocal8Bit()));
		int l;
		while (strm >> l)
			m_levelNums.insert(l);
	}
	// read characters
	n = in.namedItem("CharStats");
	if (!n.isNull()) {
		QString char_data = n.firstChild().nodeValue();
		//kDebug() << "'" << char_data << "'";
		// FIXME: get rid of stringstream here
		std::stringstream strm(std::string(char_data.toLocal8Bit()));
		int ch, correct, wrong;
		while (strm >> ch >> correct >> wrong)
			m_charStats.insert( KTouchCharStats(QChar(ch), correct, wrong) );
		//kDebug() << m_charStats.size();
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
	for (QSet<unsigned int>::const_iterator it = m_levelNums.begin(); it != m_levelNums.end(); ++it)
		level_nums += QString( " %1").arg(*it);
	e = doc.createElement("LevelNums");
	tn = doc.createTextNode(level_nums);
	e.appendChild(tn);
	session.appendChild(e);
	// add char data
	QString char_data;
    // we append for each missed char the char-unicode and the counters
	for (QSet<KTouchCharStats>::const_iterator it=m_charStats.begin(); it!=m_charStats.end(); ++it)
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
		QSet<KTouchCharStats>::iterator it = m_charStats.find( KTouchCharStats(key,0,0) );
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
	QSet<KTouchCharStats>::iterator it = m_charStats.find( KTouchCharStats(key,0,0) );
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
	m_lectureTitle.clear();
	m_lectureURL = "";
	m_levelStats.clear();
	m_sessionStats.clear();
}
// ----------------------------------------------------------------------------

bool KTouchLectureStats::read(const QDomNode &in) {
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
		// loop over all children and read levels
		QDomNode levelNode = n.firstChild();
		while (!levelNode.isNull()) {
			//kDebug() << "[KTouchLectureStats::read]  Reading level stats";
			KTouchLevelStats tmp;
			if (tmp.read(levelNode))  m_levelStats.push_back(tmp);
			levelNode = levelNode.nextSibling();
		}
	}
	// read the session stats
	n = in.namedItem("AllSessionStats");
	if (!n.isNull()) {
		// loop over all children and read sessions
		QDomNode sessionNode = n.firstChild();
		while (!sessionNode.isNull()) {
			//kDebug() << "[KTouchLectureStats::read]  Reading session stats";
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
	for (QList<KTouchLevelStats>::ConstIterator it = m_levelStats.begin(); 
		it != m_levelStats.end(); ++it)
	{
		it->write(doc, levelStatsElement);
	}
	// store session stats
	QDomElement sessionStatsElement = doc.createElement("AllSessionStats");
	lecture.appendChild(sessionStatsElement);
	for (QList<KTouchSessionStats>::ConstIterator it = m_sessionStats.begin(); 
			it != m_sessionStats.end(); ++it)
	{
		it->write(doc, sessionStatsElement);
	}
}
// ----------------------------------------------------------------------------



// *** KTouchStatisticsData ***

void KTouchStatisticsData::clear() {
	m_userName = i18n("Default User");
	m_lectureStats.clear();
}
// ----------------------------------------------------------------------------

// Reads all user statistics into the map 'stats'.
bool KTouchStatisticsData::readUserStats(QWidget * window, const KUrl& url, 
	QMap<QString, KTouchStatisticsData>& stats)
{
//	kdDebug() << "[KTouchStatisticsData::readUserStats]" << endl;
	if (url.isEmpty()) return false;

    // Ok, first download the contents as usual using the KIO lib
    // File is only downloaded if not local, otherwise it's just opened
    QString target;
    bool result = false;
    if (KIO::NetAccess::download(url, target, window)) {
        // Ok, that was successful, store the lectureURL and read the file
        QFile infile(target);
        if ( !infile.open( QIODevice::ReadOnly ) ) {
			// FIXME : what about removing the temp file?
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

//		kdDebug() << "[KTouchStatisticsData::readUserStats]  reading user stats..." << endl;

		// here we read all available statistics store in the file
		QDomNode n = doc.namedItem("KTouchStatisticsData");
		if (!n.isNull()) {
//			kdDebug() << "Entries = " << doc.elementsByTagName("KTouchStatistics").count() << endl;
			n = n.namedItem("KTouchStatistics");
		}
		else
			n = doc.namedItem("KTouchStatistics"); // read in old statistics
		// loop over all entries
		result = true;
		while (result && !n.isNull()) {
			KTouchStatisticsData userstats;
			result = userstats.readStats(n);
			// only add if reading stat data worked
//			kdDebug() << "result = " << result << endl;
			if (result) {
				stats[userstats.m_userName] = userstats;
			}
			n = n.nextSibling();
		}
	}
    KIO::NetAccess::removeTempFile(target);
    return result;
}
// ----------------------------------------------------------------------------

// Writes all user statistics in the map 'stats'.
bool KTouchStatisticsData::writeUserStats(QWidget * window, const KUrl& url, 
	const QMap<QString, KTouchStatisticsData>& stats) 
{
	// create the XML document
	QDomDocument doc;
	// add root node
	QDomElement root = doc.createElement( "KTouchStatisticsData" );
	doc.appendChild(root);

	for (QMap<QString, KTouchStatisticsData>::const_iterator it = stats.constBegin();
		it != stats.constEnd(); ++it) 
	{
		(*it).writeStats(doc, root);
	}

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
        // kdDebug() << "Error creating lecture file!" << endl;
        return false;
    }
	
#ifdef COMPRESSED_XML_STATISTICS
	QByteArray array;
	QTextStream out(array, QIODevice::WriteOnly);
	out << doc.toString();
	array = qCompress(array);
	outfile.write(array);
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

bool KTouchStatisticsData::readStats(const QDomNode &in) {
	clear(); // clear current data

	QDomNode n = in.namedItem("User");
	if (!n.isNull()) 	m_userName = n.firstChild().nodeValue();
	else 				m_userName = i18n("Default User");
	if (m_userName.isEmpty())	m_userName = i18n("Default User");
	if (m_userName.isEmpty())	m_userName = "Default User";

//	kDebug() << "[KTouchStatisticsData::readStats]  reading user stats...";
	kDebug() << "[KTouchStatisticsData::readStats]  User = '" << m_userName << "'";

	// retrieve the KTouchStatistics statistics
	n = in.namedItem("LectureStats");
	bool result = true;
	while (result && !n.isNull()) {
		KTouchLectureStats tmp;
		result = tmp.read(n);
		// do we have a valid URL?
		if (tmp.m_lectureURL.isEmpty())  result = false;
		if (result)
			m_lectureStats[tmp.m_lectureURL] = tmp;
		n = n.nextSibling();
	}

	return result;
}
// ----------------------------------------------------------------------------

void KTouchStatisticsData::writeStats(QDomDocument& doc, QDomElement& root) const {
	kDebug() << "[KTouchStatisticsData::writeStats]  user = '" << m_userName << "'";

	QDomElement user_data = doc.createElement( "KTouchStatistics" );
	root.appendChild(user_data);

	// store user name
	QString username = m_userName;
	if (username.isEmpty()) 	username = i18n("Default User");
	QDomElement userElement = doc.createElement("User");
	QDomText userText = doc.createTextNode(username);
	userElement.appendChild(userText);
	user_data.appendChild(userElement);

	// store lecture statistics
	LectureStatsMap::ConstIterator it = m_lectureStats.begin();
	while (it != m_lectureStats.end()) {
		it.value().write(doc, user_data);
		++it;
	}
}
// ----------------------------------------------------------------------------
