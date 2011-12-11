#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kapplication.h>

#include "mainwindow.h"

static const char description[] =
    I18N_NOOP("Learn and practice typewriting");

static const char version[] = "2.0.0";

int main(int argc, char **argv)
{
    KAboutData about("ktouch", 0,
                ki18n("Typewriting Trainer"),
                version,
                ki18n(description),
                KAboutData::License_GPL,
                ki18n(
                    "Copyright (C) 2011 by Sebastian Gottfried\n"
                    "Copyright (C) 2000-2007 by Håvard Frøiland and Andreas Nicolai"
                ),
                KLocalizedString(),
                "http://edu.kde.org/ktouch",
                "submit@bugs.kde.org");

    about.addAuthor(ki18n("Sebastian Gottfried"), ki18n("Current maintainer and programmer"), "sebastiangottfried@web.de");
    about.addAuthor(ki18n("Andreas Nicolai"), ki18n("Former maintainer and programmer"), "Andreas.Nicolai@gmx.net" );
    about.addAuthor(ki18n("Håvard Frøiland"), ki18n("Original author, project admin"), "haavard@users.sourceforge.net");

    about.addCredit(ki18n("David Vignoni"), ki18n("Creator of the SVG icon"), "david80v@tin.it");
    about.addCredit(ki18n("Anne-Marie Mahfouf"), ki18n("Lots of patches, fixes, updates"), "annma@kde.org");
    about.addCredit(ki18n("All the creators of training and keyboard files"));

    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineOptions options;
    //options.add("+[URL]", ki18n("Training file to open"));
    KCmdLineArgs::addCmdLineOptions(options);

    KApplication app;
    MainWindow *mainWin = 0;

    if (app.isSessionRestored())
    {
        RESTORE(MainWindow);
    }
    else
    {
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        mainWin = new MainWindow();
        mainWin->show();
        args->clear();
    }

    return app.exec();
}
