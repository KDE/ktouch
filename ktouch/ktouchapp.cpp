/***************************************************************************
                          ktouch.h  -  description
                             -------------------
    begin                : Thu Aug 30 15:33:49 EST 2001
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

#include <unistd.h>

#include <kwin.h>
#include <kdebug.h>
#include <kcmdlineargs.h>
#include <kurl.h>

#include "ktouchapp.h"
#include "ktouch.h"
#include "touchSplash.h"

#include "ktouchapp.moc"

KTouchApp::KTouchApp()
         : KUniqueApplication(),top()
{
}

KTouchApp::~KTouchApp()
{
}

int KTouchApp::newInstance()
{
//	TouchSplash *splash = new TouchSplash();
//	splash->show();
//	splash->raise();
//	splash->setHideEnabled( true );
//	QApplication::flushX();

	if (isRestored())
	{
		// There can only be one main window
		if (KMainWindow::canBeRestored(1)) {
			top = new KTouch;
			top->restore(1);
		}
	}
	else
	{
//		KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

		if (top)
		{
			kdDebug() << "KTouch already running." << endl;
			KWin::setActiveWindow(top->winId());
		}
		else
		{
			top = new KTouch;
			top->show();
		}

	}

//	sleep(2);
//	splash->hide();
//	delete(splash);

	return 0;
}

