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

#include <QCommandLineParser>

#include <KAboutData>
#include <KLocalizedString>

#include "application.h"
#include "mainwindow.h"
#include "version.h"

int main(int argc, char **argv)
{
    Application app(argc, argv);

    KLocalizedString::setApplicationDomain("ktouch");

    KAboutData about(QStringLiteral("ktouch"),
                i18n("Typewriting Trainer"),
                QStringLiteral(KTOUCH_VERSION_STRING),
                i18n("Learn and practice typewriting"),
                KAboutLicense::GPL,
                i18n(
                    "Copyright (C) 2011-2015 by Sebastian Gottfried\n"
                    "Copyright (C) 2000-2007 by Håvard Frøiland and Andreas Nicolai"
                ),
                QString(),
                QStringLiteral("http://edu.kde.org/ktouch"),
                QStringLiteral("submit@bugs.kde.org"));

    about.addAuthor(i18n("Sebastian Gottfried"), i18n("Current maintainer"), "sebastiangottfried@web.de");
    about.addAuthor(i18n("Andreas Nicolai"), i18n("Former maintainer and programmer"), "Andreas.Nicolai@gmx.net");
    about.addAuthor(i18n("Håvard Frøiland"), i18n("Original author"), "haavard@users.sourceforge.net");

    about.addCredit(i18n("David Vignoni"), i18n("Creator of the SVG icon"), "david80v@tin.it");
    about.addCredit(i18n("Anne-Marie Mahfouf"), i18n("Lots of patches, fixes and updates"), "annma@kde.org");
    about.addCredit(i18n("All the creators of training and keyboard files"));

    about.setOrganizationDomain(QByteArray("kde.org"));

    KAboutData::setApplicationData(about);

    app.setApplicationName(about.componentName());
    app.setApplicationDisplayName(about.displayName());
    app.setOrganizationDomain(about.organizationDomain());
    app.setApplicationVersion(about.version());

    QApplication::setWindowIcon(QIcon::fromTheme(QLatin1String("ktouch")));


    QCommandLineParser parser;
    about.setupCommandLine(&parser);

    parser.addOption(QCommandLineOption(QStringLiteral("resource-editor"), i18n("Launch the course and keyboard layout editor")));

    parser.addOption({{"I", "import-path"}, i18n("Prepend the path to the list of QML import paths"), "path"});

    parser.process(app);

    about.processCommandLine(&parser);

    if (parser.isSet("import-path"))
    {
        foreach (const QString& path, parser.values("import-path"))
        {
            app.qmlImportPaths().append(path);
        }
    }


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
                QPointer<ResourceEditor>& resourceEditorRef = Application::resourceEditorRef();

                if (resourceEditorRef.isNull())
                {
                    resourceEditorRef = QPointer<ResourceEditor>(new ResourceEditor());
                    resourceEditorRef.data()->restore(i);
                }
            }
        }
    }
    else
    {
        if (parser.isSet("resource-editor"))
        {
            QPointer<ResourceEditor>& resourceEditorRef = Application::resourceEditorRef();
            if (resourceEditorRef.isNull())
            {
                resourceEditorRef = QPointer<ResourceEditor>(new ResourceEditor());
            }

            ResourceEditor* resourceEditor = resourceEditorRef.data();

            resourceEditor->show();
        }
        else
        {
            MainWindow *mainWin = 0;
            mainWin = new MainWindow();
            mainWin->show();
        }
    }

    return app.exec();
}
