/*
 *  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
                    "Copyright (C) 2011-2012 by Sebastian Gottfried\n"
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
    options.add("opengl", ki18n("use a QGLWidget for the viewport"));
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
        mainWin->setUseOpenGLViewport(args->isSet("opengl"));
        mainWin->show();
        args->clear();
    }

    return app.exec();
}
