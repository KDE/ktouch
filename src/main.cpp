/***************************************************************************
 *   ktouch.cpp                                                            *
 *   ----------                                                            *
 *   Copyright (C) 2000 by Håvard Frøiland, 2003 by Andreas Nicolai        *
 *   haavard@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouch.h"
#include <qobject.h>
#include <qtimer.h>
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

//#include "ktouchsplash.h"

static const char *description =
    I18N_NOOP("A program that helps you to learn and practice touch typing.");

static const char *version = "0.1";

static KCmdLineOptions options[] = {
    { "+[URL]", I18N_NOOP( "Training file to open." ), 0 }
};

int main(int argc, char **argv)
{
    KAboutData about("ktouch",
                     I18N_NOOP("KTouch"),
                     version,
                     description,
                     KAboutData::License_GPL,
                     "Copyright (C) 2000 by Haavard Froeiland, 2003 by Andreas Nicolai",
                     0,
                     "http://edu.kde.org/ktouch",
                     "submit@bugs.kde.org");
    about.addAuthor( "Haavard Froeiland", 0, "haavard@users.sourceforge.net" );
    about.addAuthor( "Andreas Nicolai", 0, "Andreas.Nicolai@gmx.net" );
    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions( options );
    KApplication app;
    KTouch *mainWin = 0;

    if (app.isRestored())
    {
        RESTORE(KTouch);
    }
    else
    {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        // TODO: check the command line for a training file and set this as the new to open training file

        /* TODO: turn splash screen on in release
        KTouchSplash splash;            // create the splash screen
        splash.setHideEnabled( true );  // allow user to click the splash screen away
        splash.show();                  // show it
        QTimer splashTimer;
        splashTimer.connect(&splashTimer, SIGNAL(timeout()), &splash, SLOT(hide()));
        splashTimer.start(2000, true);  // start singleshot timer to hide the splashscreen
        */

        mainWin = new KTouch();
        app.setMainWidget( mainWin );

        mainWin->show();
        args->clear();
    }

    int ret = app.exec();

    delete mainWin;
    return ret;
}
