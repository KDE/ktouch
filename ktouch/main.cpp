 /***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Tue Mar 13 15:33:49 EST 2001
    copyright            : (C) 2001 by Haavard Froeiland
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

#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>

#include "ktouch.h"

static const char *description =
	I18N_NOOP("KTouch");
// INSERT A DESCRIPTION FOR YOUR APPLICATION HERE


static KCmdLineOptions options[] =
{
  { 0, 0, 0 }
  // INSERT YOUR COMMANDLINE OPTIONS HERE
};

int main(int argc, char *argv[])
{

  KAboutData aboutData( "ktouch", I18N_NOOP("KTouch"),
    VERSION, description, KAboutData::License_GPL,
    "(c) 2001, Haavard Froeiland", 0, 0, "haavard@users.sourceforge.net");
  aboutData.addAuthor("Haavard Froeiland",0, "haavard@users.sourceforge.net");
  KCmdLineArgs::init( argc, argv, &aboutData );
	  KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

  KApplication a;
  KTouch *ktouch = new KTouch();
  a.setMainWidget(ktouch);
  ktouch->show();  

  return a.exec();
}

