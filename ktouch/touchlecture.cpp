/***************************************************************************
                          touchlecture.cpp  -  description
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

#include "touchlecture.h"
#include "touchlecture.moc"
#include <kdebug.h>


using namespace std;

TouchLecture::TouchLecture()
{
    fileName="";
    level=0;
    pos=1;
    levelHasChanged=false;
};

QString TouchLecture::getName()
{
    return fileName;
}

void TouchLecture::load(QString f)
{
    kdDebug() << "Loading file: " << f << endl;
    QFile file(f);
    if(file.exists())
    {
	if (file.open(IO_ReadOnly) )      // file opened successfully
	{
	    fileName=f;
	    unsigned int maxLevel=0;
	    levelVector.clear();
	    
	    QTextStream t( &file );
	    QString s;
	    levelVector.insert(levelVector.end(),new vector<QString>);
	    while ( !t.eof() )
	    {             // until end of file...
		s = t.readLine().stripWhiteSpace();              // line of text excluding '\n'
		if(s.at(0)!='#') // # means that it's a comment
		{
		    if(s.isEmpty())
		    {
			// we detected a line-space so this might be a new level
			if(!(levelVector[maxLevel]->empty()))
			{
			    levelVector.insert(levelVector.end(),new vector<QString>);
			    maxLevel++;
			}
		    }
		    else
		    {
			// adding the line
			levelVector[maxLevel]->insert(levelVector[maxLevel]->end(),s);
		    }
		}
	    }
	    file.close();
	    if(levelVector[maxLevel]->empty())
	    {
		//levelVector.erase(&levelVector[maxLevel]);
		levelVector.erase(levelVector.begin()+maxLevel);
		maxLevel--;
	    }
	    
	    setLevel(level);
	    pos=1;
	}
	else
	{
	    //i18n needed here. What do I need to include?
	    QMessageBox::information( parent, "Error reading training file",
				      "There was an error when trying to open: "+f );
	}
    }
    else
    {
	//i18n needed here. What do I need to include?
	//QMessageBox::information( parent,"Missing training file",
	//			  "The trainingfile specifide dosn't exists: "+f);
	kdDebug() << "File dosn't exist" << endl;
    }
    kdDebug() << "Loading file end" << endl;
}

int TouchLecture::save()
{
    QFile file(fileName);
    if( file.open(IO_WriteOnly) )
    {  //file opened successfully
	QTextStream t( &file );
	t << "################################## "  << endl;
	t << "#                                # "  << endl;
	t << "#  Trainingfile for KTouch       # "  << endl;
	t << "#             (Håvard Frøiland)  # "  << endl;
	t << "################################## "  << endl;
	
	for(unsigned int i=0;i<levelVector.size();i++)
	{
	    t << endl;
	    t << "################################" << endl;
	    t << "# Level: " << i << endl;
	    t << "#" << endl;
	    vector<QString> levelData=*levelVector[i];
	    for(unsigned int j=0;j<levelData.size();j++)
	    {
		t << levelData[j] << endl;
	    }
	}
    }
    else
	return 1;
    file.close();
    return 0;
}

int TouchLecture::saveAs(QString f)
{
    fileName=f;
    return save();
}

QString TouchLecture::getNextLine()
{
    if (levelVector.empty())
    {
	return "No file loaded";
    }

    if(levelHasChanged)
    {
	setLevel(level);
    }
    
    if(level<(levelVector.size()))
    {
	vector<QString> levelData=*levelVector[level];
	
	if (pos>=(levelData.size()-1))
	{
	    pos=1;
	}
	return levelData[pos++];
    }
    return "Error in getNextLine()";
};


void TouchLecture::levelUp()
{
	kdDebug() << "LevelUp" << endl;
	if(level<(levelVector.size()-1) && !levelHasChanged)
	{
		level++;
		pos=1;
		levelHasChanged=true;
	}
	else if(level==(levelVector.size()-1)) // the user has finished this training file...
	  {
	    kdDebug() << "Finished" << endl;
	    //	    emit pause;

	  }
};

void TouchLecture::levelDown()
{
	kdDebug() << "LevelDown" << endl;
	if(level>0 && !levelHasChanged)
	{
		level--;
		pos=1;
		levelHasChanged=true;
	}
};

void TouchLecture::setLevel(unsigned int l)
{
    if (levelVector.empty())
    {
	kdDebug() <<  "TouchLecture::setLevel   No file loaded" << endl;
	return;
    }

    if(l<(levelVector.size()))
	{
	    level=l;
		emit levelChanged(level);
		{
		    vector<QString> test=*levelVector[level];
			
			emit levelMessage(QString(test[0]));
			
		}
	}
	else
	{
	    level=levelVector.size()-1;
	    setLevel(level);
	}
	levelHasChanged=false;
};

