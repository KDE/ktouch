/***************************************************************************
 *   ktouch.cpp                                                            *
 *   ----------                                                            *
 *   Copyright (C) 2000 by Håvard Frøiland, 2004 by Andreas Nicolai        *
 *   ghorwin@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "ktouch.h"
#include <kaboutdata.h>
#include <kcmdlineargs.h>

#include <kdebug.h>

static const char description[] =
    I18N_NOOP("A program that helps you to learn and practice touch typing");

static const char version[] = "1.5.3";

static KCmdLineOptions options[] = {
    { "+[URL]", I18N_NOOP( "Training file to open" ), 0 },
    KCmdLineLastOption
};

int main(int argc, char **argv)
{
    KAboutData about("ktouch",
                     I18N_NOOP("KTouch"),
                     version,
                     description,
                     KAboutData::License_GPL,
                     "Copyright (C) 2000-2006 by Håvard Frøiland and Andreas Nicolai",
                     0,
                     "http://edu.kde.org/ktouch",
                     "submit@bugs.kde.org");
    about.addAuthor( "Andreas Nicolai", I18N_NOOP("Current maintainer and programmer"), "Andreas.Nicolai@gmx.net" );
    about.addAuthor( "Håvard Frøiland", I18N_NOOP("Original author, project admin"), "haavard@users.sourceforge.net" );
    about.addCredit( "David Vignoni", I18N_NOOP("Creator of the SVG icon"), "david80v@tin.it");
    about.addCredit( "Anne-Marie Mahfouf", I18N_NOOP("Lots of patches, fixes, updates"), "annma@kde.org");
    about.addCredit( "All the creators of training and keyboard files", 0, 0);
    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions( options );
    KApplication app;
    KTouch *mainWin = 0;

    if (app.isRestored()) {
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

    return app.exec();
}
