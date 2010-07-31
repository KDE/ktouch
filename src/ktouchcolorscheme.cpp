/***************************************************************************
 *   ktouchcolorscheme.cpp                                                 *
 *   ---------------------                                                 *
 *   Copyright (C) 2003-2006 by Andreas Nicolai                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouchcolorscheme.h"

#include <tqfile.h>
#include <tqtextstream.h>
#include <tqdom.h>

#include <kdebug.h>
#include <klocale.h>
#include <ktempfile.h>
#include <kio/netaccess.h>

#define WRITE_COLOR(z,x,y) 	e = doc.createElement(x); \
	n = doc.createTextNode( (y).name() ); \
	e.appendChild(n); \
	z.appendChild(e);

#define READ_COLOR(x,y)   	c = n.namedItem(x); \
	if (!c.isNull())  y = TQColor(c.firstChild().nodeValue());

TQValueVector<KTouchColorScheme>	KTouchColorScheme::m_colorSchemes;

void KTouchColorScheme::clear() {
	// the default scheme is the classic one
    m_teacherTextColor = TQColor("#000032");
    m_teacherBackground = TQColor("#BEBEFF");
    m_studentTextColor = TQColor("#003200");
    m_studentBackground = TQColor("#9FFF9F");
    m_errorTextColor = Qt::white;
    m_errorBackground = TQColor("#AC0000");

    m_frame = Qt::black;
    m_background[0] = TQColor(255,238,  7);     m_background[4] = TQColor(247,138,247);
    m_background[1] = TQColor( 14,164,239);     m_background[5] = TQColor(158,255,155);
    m_background[2] = TQColor(158,255,155);     m_background[6] = TQColor( 14,164,239);
    m_background[3] = TQColor(252,138,138);     m_background[7] = TQColor(255,238,  7);
    m_text = Qt::black;
    m_backgroundH = Qt::darkBlue;
    m_textH = Qt::white;
    m_cBackground = Qt::gray;
    m_cText = Qt::black;
    m_cBackgroundH = Qt::white;
    m_cTextH = Qt::black;

	m_default = false;
}
// ----------------------------------------------------------------------------

bool KTouchColorScheme::read(const TQDomNode& node) {
//	kdDebug() << "[KTouchColorScheme::read]  " << endl;

	TQDomNode name = node.namedItem("Name");
	if (name.isNull())  m_name = i18n("untitled color scheme");
	else				m_name = name.firstChild().nodeValue();

//	kdDebug() << "  Reading scheme = " << m_name << endl;
	
	TQDomNode n = node.namedItem("LineColors");
	if (!n.isNull()) {
		TQDomNode c;
		READ_COLOR("TeacherTextColor", m_teacherTextColor);
		READ_COLOR("TeacherBackground", m_teacherBackground);
		READ_COLOR("StudentTextColor", m_studentTextColor);
		READ_COLOR("StudentBackground", m_studentBackground);
		READ_COLOR("ErrorTextColor", m_errorTextColor);
		READ_COLOR("ErrorBackground", m_errorBackground);
	}

	n = node.namedItem("KeyboardColors");
	if (!n.isNull()) {
		TQDomNode c;
		READ_COLOR("KeyFrame", m_frame);
		for (unsigned int i=0; i<8; ++i) {
			READ_COLOR(TQString("KeyBack_%1").arg(i), m_background[i]);
		}
		READ_COLOR("KeyText", m_text);
		READ_COLOR("KeyBackHigh", m_backgroundH);
		READ_COLOR("KeyTextHigh", m_textH);
		READ_COLOR("KeyBackControl", m_cBackground);
		READ_COLOR("KeyTextControl", m_cText);
		READ_COLOR("KeyBackControlHigh", m_cBackgroundH);
		READ_COLOR("KeyTextControlHigh", m_cTextH);
	}
	return true;
}
// ----------------------------------------------------------------------------

void KTouchColorScheme::write(TQDomDocument& doc, TQDomElement& root) const {
	//kdDebug() << "[KTouchColorScheme::write]  " << endl;
	TQDomElement element = doc.createElement("KTouchColorScheme");
	// append name of color scheme
	TQDomElement name = doc.createElement("Name");
	TQDomText name_text;
	if (m_name.isEmpty())	name_text = doc.createTextNode( i18n("untitled color scheme") );
	else					name_text = doc.createTextNode(m_name);
	name.appendChild(name_text);
	element.appendChild(name);
	// store slide line colors
	TQDomElement line_colors = doc.createElement("LineColors");
	TQDomElement e;
	TQDomText n;

	WRITE_COLOR(line_colors, "TeacherTextColor", m_teacherTextColor);
	WRITE_COLOR(line_colors, "TeacherBackground", m_teacherBackground);
	WRITE_COLOR(line_colors, "StudentTextColor", m_studentTextColor);
	WRITE_COLOR(line_colors, "StudentBackground", m_studentBackground);
	WRITE_COLOR(line_colors, "ErrorTextColor", m_errorTextColor);
	WRITE_COLOR(line_colors, "ErrorBackground", m_errorBackground);

	element.appendChild(line_colors);
    // done with slide line colors

	// write key drawing colors
	TQDomElement key_colors = doc.createElement("KeyboardColors");

	WRITE_COLOR(key_colors, "KeyFrame", m_frame);
	for (unsigned int i=0; i<8; ++i) {
		WRITE_COLOR(key_colors, TQString("KeyBack_%1").arg(i), m_background[i]);
	}
	WRITE_COLOR(key_colors, "KeyText", m_text);
	WRITE_COLOR(key_colors, "KeyBackHigh", m_backgroundH);
	WRITE_COLOR(key_colors, "KeyTextHigh", m_textH);
	WRITE_COLOR(key_colors, "KeyBackControl", m_cBackground);
	WRITE_COLOR(key_colors, "KeyTextControl", m_cText);
	WRITE_COLOR(key_colors, "KeyBackControlHigh", m_cBackgroundH);
	WRITE_COLOR(key_colors, "KeyTextControlHigh", m_cTextH);

	element.appendChild(key_colors);


	root.appendChild(element);
}
// ----------------------------------------------------------------------------

bool KTouchColorScheme::readList(TQWidget * window, const KURL& url) {
	if (url.isEmpty()) return false;
	//kdDebug() << "[KTouchColorScheme::readList]  " << endl;
	//kdDebug() << "  url = '" << url.url() << "'" << endl;
    TQString tmp_file;
    bool result = KIO::NetAccess::download(url, tmp_file, window);
    if (result) {
        // Ok, that was successful, try to parse the XML doc now
        TQFile infile(tmp_file);
        if ( !infile.open( IO_ReadOnly ) ) {
    		KIO::NetAccess::removeTempFile(tmp_file);
            return false;   // Bugger it... couldn't open it...
		}
		TQDomDocument doc;
		doc.setContent( &infile );
		// read the document
		TQDomNodeList schemes = doc.elementsByTagName("KTouchColorScheme");
		int num = schemes.count();
		if (num > 0) {
			//kdDebug() << "  reading " << schemes.count() << " color maps..." << endl;
			for (int i=0; i<num; ++i) {
				KTouchColorScheme scheme;
				if (scheme.read(schemes.item(i)))
					m_colorSchemes.append(scheme);
			}
		}
	
    }
    KIO::NetAccess::removeTempFile(tmp_file);

    return result;
}
// ----------------------------------------------------------------------------

bool KTouchColorScheme::writeList(TQWidget * window, const KURL& url) {
	// create the XML document and root node
	TQDomDocument doc;
    TQDomElement root = doc.createElement( "KTouchColorSchemeCollection" );
    doc.appendChild(root);
	// write all the non-default color schemes
	for (TQValueVector<KTouchColorScheme>::const_iterator it = m_colorSchemes.constBegin();
		it != m_colorSchemes.constEnd(); ++it)
	{
		if (!it->m_default)  it->write(doc, root);
	}

	// and save it
    TQString tmp_file;
    KTempFile *temp=0;
    if (url.isLocalFile())
        tmp_file=url.path();         // for local files the path is sufficient
    else {
        temp=new KTempFile;         // for remote files create a temporary file first
        temp->setAutoDelete(true);  // so we don't have to delete the file ourselves
        tmp_file=temp->name();
    }

    TQFile outfile(tmp_file);
    if ( !outfile.open( IO_WriteOnly ) ) {
        if (temp)  delete temp;
        kdDebug() << "Error creating tmp file '"+tmp_file+"' for writing color map!" << endl;
        return false;
    }
	
    TQTextStream out( &outfile );
    out << doc.toString();
    outfile.close();
    // if we have a temporary file, we still need to upload it
    if (temp) {
        KIO::NetAccess::upload(tmp_file, url, window);
        delete temp;
    }
    return true;
}
// ----------------------------------------------------------------------------

void KTouchColorScheme::createDefaults() {
	m_colorSchemes.clear();
    KTouchColorScheme color;
	color.m_default = true;

    color.m_name = i18n("Black & White");
    color.m_teacherTextColor = Qt::black;
    color.m_teacherBackground = Qt::white;
    color.m_studentTextColor = Qt::black;
    color.m_studentBackground = Qt::white;
    color.m_errorTextColor = Qt::white;
    color.m_errorBackground = Qt::black;
    color.m_frame = Qt::black;
    for (int i=0; i<8; ++i)
        color.m_background[i] = Qt::white;
    color.m_text = Qt::black;
    color.m_backgroundH = Qt::black;
    color.m_textH = Qt::white;
    color.m_cBackground = Qt::gray;
    color.m_cText = Qt::black;
    color.m_cBackgroundH = Qt::white;
    color.m_cTextH = Qt::black;
    m_colorSchemes.push_back(color);

    color.m_name = i18n("Classic");
    color.m_teacherTextColor = TQColor("#000032");
    color.m_teacherBackground = TQColor("#BEBEFF");
    color.m_studentTextColor = TQColor("#003200");
    color.m_studentBackground = TQColor("#9FFF9F");
    color.m_errorTextColor = Qt::white;
    color.m_errorBackground = TQColor("#AC0000");
    color.m_frame = Qt::black;
    color.m_background[0] = TQColor(255,238,  7);     color.m_background[4] = TQColor(247,138,247);
    color.m_background[1] = TQColor( 14,164,239);     color.m_background[5] = TQColor(158,255,155);
    color.m_background[2] = TQColor(158,255,155);     color.m_background[6] = TQColor( 14,164,239);
    color.m_background[3] = TQColor(252,138,138);     color.m_background[7] = TQColor(255,238,  7);
    color.m_text = Qt::black;
    color.m_backgroundH = Qt::darkBlue;
    color.m_textH = Qt::white;
    color.m_cBackground = Qt::gray;
    color.m_cText = Qt::black;
    color.m_cBackgroundH = Qt::white;
    color.m_cTextH = Qt::black;
    m_colorSchemes.push_back(color);

    color.m_name = i18n("Deep Blue");
    color.m_teacherTextColor = Qt::white;
    color.m_teacherBackground = TQColor(  0, 39, 80);
    color.m_studentTextColor = Qt::white;
    color.m_studentBackground = TQColor( 39, 59,127);
    color.m_errorTextColor = TQColor("#B5CFFF");
    color.m_errorBackground = TQColor("#640000");
    color.m_frame = TQColor(220,220,220);
    color.m_background[0] = TQColor(  0, 39, 80);     color.m_background[4] = TQColor( 24, 19, 72);
    color.m_background[1] = TQColor( 39, 59,127);     color.m_background[5] = TQColor(  8, 44,124);
    color.m_background[2] = TQColor(  4, 39, 53);     color.m_background[6] = TQColor( 10, 82,158);
    color.m_background[3] = TQColor( 40, 32,121);     color.m_background[7] = TQColor( 43, 60,124);
    color.m_text = Qt::white;
    color.m_backgroundH = TQColor(125,180,255);
    color.m_textH = Qt::darkBlue;
    color.m_cBackground = Qt::black;
    color.m_cText = Qt::white;
    color.m_cBackgroundH = TQColor(111,121,73);
    color.m_cTextH = Qt::white;
    m_colorSchemes.push_back(color);

    color.m_name = i18n("Stripy");
    color.m_teacherTextColor = Qt::white;
    color.m_teacherBackground = TQColor( 39, 70, 227);
    color.m_studentTextColor = Qt::white;
    color.m_studentBackground = TQColor( 39, 70, 127);
    color.m_errorTextColor = Qt::white;
    color.m_errorBackground = Qt::darkGray;
    color.m_frame = Qt::black;
    for (int i=0; i<8; i=i+2)
        color.m_background[i] = TQColor( 39, 70, 127);
	for (int i=1; i<8; i=i+2)
        color.m_background[i] = Qt::darkGray;
    color.m_text = Qt::black;
    color.m_backgroundH = TQColor( 39, 70, 227);
    color.m_textH = Qt::white;
    color.m_cBackground = Qt::gray;
    color.m_cText = Qt::black;
    color.m_cBackgroundH = TQColor( 39, 70, 227);
    color.m_cTextH = Qt::black;
    m_colorSchemes.push_back(color);
}
// ----------------------------------------------------------------------------

