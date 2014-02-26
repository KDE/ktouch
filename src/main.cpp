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

#include <KAboutData>
#include <KCmdLineArgs>
#include <KLocale>

#include "application.h"
#include "mainwindow.h"

#include "KDebug"

static const char description[] =
    I18N_NOOP("Learn and practice typewriting");

static const char version[] = "2.3.0";

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

    about.addAuthor(ki18n("Sebastian Gottfried"), ki18n("Current maintainer"), "sebastiangottfried@web.de");
    about.addAuthor(ki18n("Andreas Nicolai"), ki18n("Former maintainer and programmer"), "Andreas.Nicolai@gmx.net");
    about.addAuthor(ki18n("Håvard Frøiland"), ki18n("Original author"), "haavard@users.sourceforge.net");

    about.addCredit(ki18n("David Vignoni"), ki18n("Creator of the SVG icon"), "david80v@tin.it");
    about.addCredit(ki18n("Anne-Marie Mahfouf"), ki18n("Lots of patches, fixes and updates"), "annma@kde.org");
    about.addCredit(ki18n("All the creators of training and keyboard files"));

    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineOptions options;
    options.add("opengl", ki18n("Use OpenGL for rendering (experimental)"));
    options.add("resource-editor", ki18n("Launch the course and keyboard layout editor"));
    KCmdLineArgs::addCmdLineOptions(options);

    KApplication::setGraphicsSystem("raster");

    Application app;

    if (app.isSessionRestored())
    {
        for (int i = 1; KMainWindow::canBeRestored(i); i++)
        {
            const QString name = KMainWindow::classNameOfToplevel(i);

            if (name == "MainWindow")
            {
                (new MainWindow)->restore(i);
            }
            else if (name == "ResourceEditor")
            {
                QWeakPointer<ResourceEditor>& resourceEditorRef = Application::resourceEditorRef();

                if (resourceEditorRef.isNull())
                {
                    resourceEditorRef = new ResourceEditor();
                    resourceEditorRef.data()->restore(i);
                }
            }
        }
    }
    else
    {
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

        if (args->isSet("resource-editor"))
        {
            QWeakPointer<ResourceEditor>& resourceEditorRef = Application::resourceEditorRef();
            if (resourceEditorRef.isNull())
            {
                resourceEditorRef = new ResourceEditor();
            }

            ResourceEditor* resourceEditor = resourceEditorRef.data();

            resourceEditor->show();
        }
        else
        {
            MainWindow *mainWin = 0;
            mainWin = new MainWindow();
            mainWin->setUseOpenGLViewport(args->isSet("opengl"));
            mainWin->show();
        }
        args->clear();
    }

    return app.exec();
}
