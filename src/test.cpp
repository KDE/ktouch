#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kdebug.h>
#include <kapplication.h>

#include "testwin.h"

static const char description[] =
    I18N_NOOP("A program that helps you to learn and practice touch typing");

static const char version[] = "1.7.1";

int main(int argc, char **argv)
{
    KAboutData about("ktouch", 0,
                     ki18n("KTouch"),
                     version,
                     ki18n(description),
                     KAboutData::License_GPL,
                     ki18n("Copyright (C) 2000-2007 by Håvard Frøiland and Andreas Nicolai"),
                     KLocalizedString(),
                     "http://edu.kde.org/ktouch",
                     "submit@bugs.kde.org");
    about.addAuthor( ki18n("Andreas Nicolai"), ki18n("Current maintainer and programmer"), "Andreas.Nicolai@gmx.net" );
    about.addAuthor( ki18n("Håvard Frøiland"), ki18n("Original author, project admin"), "haavard@users.sourceforge.net" );
    about.addCredit( ki18n("David Vignoni"), ki18n("Creator of the SVG icon"), "david80v@tin.it");
    about.addCredit( ki18n("Anne-Marie Mahfouf"), ki18n("Lots of patches, fixes, updates"), "annma@kde.org");
    about.addCredit( ki18n("All the creators of training and keyboard files"));
    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add("+[URL]", ki18n( "Training file to open" ));
    KCmdLineArgs::addCmdLineOptions( options );
    KApplication app;
    KTouch *mainWin = 0;

    if (app.isSessionRestored()) {
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

        mainWin->show();
        args->clear();
    }

    return app.exec();
}
