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

#include <stdlib.h>
#include <stdio.h>

#include <config.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>

#include "ktouchapp.h"

static const char *description =
	I18N_NOOP("KTouch");

static KCmdLineOptions options[] =
{
	{ 0, 0, 0 }
};

int main(int argc, char *argv[])
{
	KAboutData about( "ktouch",
		I18N_NOOP("KTouch"),
		VERSION,
		I18N_NOOP("A touch-typing trainer"),
		KAboutData::License_GPL,
		"Copyright (c) 2001, Haavard Froeiland", 0, 0, "haavard@users.sourceforge.net");
	about.addAuthor("Haavard Froeiland",0, "haavard@users.sourceforge.net");

	KCmdLineArgs::init(argc, argv, &about);
	KCmdLineArgs::addCmdLineOptions( options );
	KUniqueApplication::addCmdLineOptions();

	if (!KTouchApp::start())
	{
		fprintf(stderr, "KTouch is already running!\n");
		exit(0);
	}
	KTouchApp app;
	app.exec();
}

