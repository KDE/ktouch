/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <QCommandLineParser>

#include <KAboutData>
#include <KLocalizedString>

#include "application.h"
#include "mainwindow.h"
#include "version.h"

int main(int argc, char **argv)
{
    Application::setAttribute(Qt::AA_EnableHighDpiScaling);
    Application::setAttribute(Qt::AA_UseHighDpiPixmaps);

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
                QStringLiteral("https://edu.kde.org/ktouch"),
                QStringLiteral("submit@bugs.kde.org"));

    about.addAuthor(i18n("Sebastian Gottfried"), i18n("Current maintainer"), QStringLiteral("sebastiangottfried@web.de"));
    about.addAuthor(i18n("Andreas Nicolai"), i18n("Former maintainer and programmer"), QStringLiteral("Andreas.Nicolai@gmx.net"));
    about.addAuthor(i18n("Håvard Frøiland"), i18n("Original author"), QStringLiteral("haavard@users.sourceforge.net"));

    about.addCredit(i18n("David Vignoni"), i18n("Creator of the SVG icon"), QStringLiteral("david80v@tin.it"));
    about.addCredit(i18n("Anne-Marie Mahfouf"), i18n("Lots of patches, fixes and updates"), QStringLiteral("annma@kde.org"));
    about.addCredit(i18n("All the creators of training and keyboard files"));

    about.setOrganizationDomain(QByteArray("kde.org"));

    KAboutData::setApplicationData(about);

    app.setApplicationName(about.componentName());
    app.setApplicationDisplayName(about.displayName());
    app.setOrganizationDomain(about.organizationDomain());
    app.setApplicationVersion(about.version());

    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("ktouch")));


    QCommandLineParser parser;
    about.setupCommandLine(&parser);

    parser.addOption(QCommandLineOption(QStringLiteral("resource-editor"), i18n("Launch the course and keyboard layout editor")));

    parser.addOption({{"I", "import-path"}, i18n("Prepend the path to the list of QML import paths"), QStringLiteral("path")});

    parser.process(app);

    about.processCommandLine(&parser);

    if (parser.isSet(QStringLiteral("import-path")))
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

            if (name == QLatin1String("MainWindow"))
            {
                (new MainWindow)->restore(i);
            }
            else if (name == QLatin1String("ResourceEditor"))
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
        if (parser.isSet(QStringLiteral("resource-editor")))
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
