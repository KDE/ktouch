/***************************************************************************
                          touchlecture.h  -  description
                             -------------------
    begin                : Sun Dec 10 2000
    copyright            : (C) 2000 by Haavard Froeiland
    email                : haavard@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TOUCHLECTURE_H
#define TOUCHLECTURE_H

#include <vector>

#include <qobject.h>
#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qmessagebox.h>
#include <kdebug.h>
#include <qstringlist.h>

class TouchLecture : public QObject
{
	Q_OBJECT

	public:
		TouchLecture();
		QString getNextLine();
		QString getName();

		void load(QString fileName);
		int save();
		int saveAs(QString fileName);
		std::vector<std::vector<QString> *> levelVector;


	public slots:
		void levelUp();
		void levelDown();
		void setLevel(unsigned int level);

	signals:
		void levelChanged(int);
		void levelMessage(const QString&);

	private:
		unsigned int pos;
		unsigned int level;
		QString fileName;
		QWidget *parent;
		bool levelHasChanged;
};

#endif



