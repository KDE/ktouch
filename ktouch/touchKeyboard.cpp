/***************************************************************************
                          touchKeyboard.cpp  -  description
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

#include "touchkeyboard.h"
#include "touchkey.h"
#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <string>

TouchKeyboard::TouchKeyboard(QWidget *parent, const char *name)
             : QWidget( parent, name )
{
  dirs = KGlobal::dirs();
  trans=0;
  lastKey=0;
  showAnimation=true;
  for (unsigned int i=0;i<512;i++)
  {
    keyArray[i]=0;
  }
}

void TouchKeyboard::setShowColor(bool show)
{
  TouchKey::setShowColor(show);
  repaint(false);
}

void TouchKeyboard::setShowAnimation(bool show)
{
  showAnimation=show;
}

bool TouchKeyboard::getShowColor()
{
  return TouchKey::getShowColor();
}

bool TouchKeyboard::getShowAnimation()
{
  return showAnimation;
}

void TouchKeyboard::paintEvent( QPaintEvent * )
{
  QPainter painter;
  painter.begin (this);
  painter.translate(trans,0);

  for(int i=0;i<512;i++)
  {
    if(keyArray[i])
    {
      keyArray[i]->paint(&painter);
    }
  }
  painter.end();
}

void TouchKeyboard::newKey(const QChar &nextKey)
{
  QPainter p(this);
  p.translate(trans,0);
  if (keyArray[lastKey])
  {
		keyArray[lastKey.unicode()]->state=false;
	  keyArray[lastKey.unicode()]->paint(&p);
	}
  if (keyArray[nextKey.unicode()])
	{
  	keyArray[nextKey.unicode()]->state=true;
    lastKey=nextKey;

 	  if(showAnimation)
		  keyArray[nextKey.unicode()]->paint(&p);
	}
}

void TouchKeyboard::resizeEvent (QResizeEvent *)
{
  calculateSize();
}

void TouchKeyboard::calculateSize()
{
  float maxW;
  if(width()>(height()*3))
  {
    maxW=(float)height()*3/150;
    trans=(width()-height()*3)/2;
  }
  else
  {
    trans=0;
    maxW=(float)width()/150;
  }

	for (int i=0;i<512;i++)
  {
    if(keyArray[i]) keyArray[i]->setScale(maxW);
  }

  repaint(false);
}

void TouchKeyboard::clearKeyboard()
{
	for (int i=0;i<512;i++)
  {
		if(keyArray[i]!=0)
		{
			delete(keyArray[i]);
		}
	keyArray[i]=0;
  }
	lastKey=0;
  FingerKey::numberOfKeys=0;
}

QString TouchKeyboard::getLanguage()
{
 	return language;
}

void TouchKeyboard::loadKeyboard(QString lang)
{
	const int maxPara=10;
  if(lang.isEmpty())
    lang="en";
  ifstream ifs(dirs->findResource("data","ktouch/keyboard." + lang).latin1(), ios::in);
  if (!ifs) {
    cerr << "Error: unable to open keyboard" << dirs->findResource("data","ktouch/keyboard." + lang).latin1() << endl;
  }
  else
	{
    clearKeyboard();
    language=lang;
		char buffer[128];
		int lineNumber=0;
    string line;
  	
  	while (!ifs.eof()) {
			lineNumber++;
    	ifs.getline(buffer, sizeof(buffer), '\n');
    	if (!(ifs.eof() && strlen(buffer) == 0)){
				if(!(buffer[0]=='#'))
				{
        	line=buffer;
					string separators = " \t";
					unsigned int start=line.find_first_not_of(separators);
					unsigned int end=0;
					string keyBoardVector[maxPara];
					for(int i=0;i<maxPara;i++)
					{
						keyBoardVector[i]="";
					}
					int para=0;
					while(start != string::npos && para<maxPara)
					{
          	end=line.find_first_of(separators, start +1);
						if(end==string::npos)
							end = line.length();
						keyBoardVector[para]=line.substr(start, end-start);
				
						start = line.find_first_not_of(separators, end+1);	 	
						para++;
					}

					// Loads the Finger keys
          if(keyBoardVector[0]=="FingerKey")
					{
						string *s;
						s=new string(keyBoardVector[2]);
        		keyArray[atoi(keyBoardVector[1].c_str())]=new FingerKey(
								s->c_str(),
								atoi(keyBoardVector[3].c_str()),
								atoi(keyBoardVector[4].c_str()));
	  	  	}

					// Loads the control key
					else if(keyBoardVector[0]=="ControlKey")
					{
						string *s;
						s=new string(keyBoardVector[2]);
        		keyArray[atoi(keyBoardVector[1].c_str())]=new ControlKey(
								s->c_str(),
								atoi(keyBoardVector[3].c_str()),
								atoi(keyBoardVector[4].c_str()),
								atoi(keyBoardVector[5].c_str()));
	  	  	}

					// Loads the normal keys
					else if(keyBoardVector[0]=="NormalKey")
					{
  					if(!keyArray[atoi(keyBoardVector[5].c_str())])
						{
							cerr << "Error in line: " << lineNumber << " FingerKey " << keyBoardVector[5] << " key don't exists" << endl;
						}
						
						string *s;
						s=new string(keyBoardVector[2]);

						if(atoi(keyBoardVector[6].c_str())==0)
						{
        			keyArray[atoi(keyBoardVector[1].c_str())]=new NormalKey(
									s->c_str(),
									atoi(keyBoardVector[3].c_str()),
									atoi(keyBoardVector[4].c_str()),
									keyArray[atoi(keyBoardVector[5].c_str())]);
						}
						else
						{
        			keyArray[atoi(keyBoardVector[1].c_str())]=new NormalKey(
									s->c_str(),
									atoi(keyBoardVector[3].c_str()),
									atoi(keyBoardVector[4].c_str()),
									keyArray[atoi(keyBoardVector[5].c_str())],
									atoi(keyBoardVector[6].c_str()));
						}
	  	  	}

					// Loads the Hidden keys
					else if(keyBoardVector[0]=="HiddenKey")
					{
  					if(!keyArray[atoi(keyBoardVector[2].c_str())])
						{
            	cerr << "Error in line: " << lineNumber << " TargetKey " << keyBoardVector[2] << " key don't exists" << endl;
						}

  					if(!keyArray[atoi(keyBoardVector[3].c_str())])
						{
            	cerr << "Error in line: " << lineNumber << " FingerKey " << keyBoardVector[3] << " key don't exists" << endl;
						}

	 					if(!keyArray[atoi(keyBoardVector[4].c_str())])
						{
            	cerr << "Error in line: " << lineNumber << " ControlKey " << keyBoardVector[4] << " key don't exists" << endl;
						}
						
        		keyArray[atoi(keyBoardVector[1].c_str())]=new HiddenKey(
								keyArray[atoi(keyBoardVector[2].c_str())],
								keyArray[atoi(keyBoardVector[3].c_str())],
								keyArray[atoi(keyBoardVector[4].c_str())]);
	  	  	}
				}
			}
		}
	}
  calculateSize();
  repaint();
}


