/*
 * Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "application.h"

#include <QDir>
#include <QFile>
#include <QQmlEngine>
#include <QQmlContext>
#include <QStandardPaths>

#include <Kdelibs4ConfigMigrator>
#include <Kdelibs4Migration>
#include <KDeclarative/KDeclarative>

#include "bindings/utils.h"
#include "bindings/stringformatter.h"
#include "declarativeitems/griditem.h"
#include "declarativeitems/lessonpainter.h"
#include "declarativeitems/preferencesproxy.h"
#include "declarativeitems/scalebackgrounditem.h"
#include "declarativeitems/traininglinecore.h"
#include "core/keyboardlayout.h"
#include "core/key.h"
#include "core/specialkey.h"
#include "core/keychar.h"
#include "core/course.h"
#include "core/lesson.h"
#include "core/profile.h"
#include "core/trainingstats.h"
#include "core/dataindex.h"
#include "core/dataaccess.h"
#include "core/profiledataaccess.h"
#include "models/resourcemodel.h"
#include "models/lessonmodel.h"
#include "models/categorizedresourcesortfilterproxymodel.h"
#include "models/learningprogressmodel.h"
#include "models/errorsmodel.h"


Application::Application(int& argc, char** argv, int flags):
    QApplication(argc, argv, flags),
    m_dataIndex(new DataIndex(this))
{
    registerQmlTypes();
    migrateKde4Files();

    DataAccess dataAccess;
    dataAccess.loadDataIndex(m_dataIndex);
}

DataIndex* Application::dataIndex()
{
    Application* app = qobject_cast<Application*>(QCoreApplication::instance());

    return app->m_dataIndex;
}

QPointer<ResourceEditor>& Application::resourceEditorRef()
{
    Application* app = qobject_cast<Application*>(QCoreApplication::instance());

    return app->m_resourceEditorRef;
}

void Application::setupDeclarativeBindings(QQmlEngine* qmlEngine)
{
    KDeclarative::KDeclarative kDeclarative;
    kDeclarative.setDeclarativeEngine(qmlEngine);
    kDeclarative.setupBindings();

    Application* app = static_cast<Application*>(Application::instance());
    foreach (const QString& path, app->m_qmlImportPaths)
    {
        qmlEngine->addImportPath(path);
    }

    QQmlContext* rootContext = qmlEngine->rootContext();

    rootContext->setContextProperty("utils", new Utils());
    rootContext->setContextProperty("strFormatter", new StringFormatter());
}

QStringList& Application::qmlImportPaths()
{
    return m_qmlImportPaths;
}

void Application::registerQmlTypes()
{
    qmlRegisterType<KeyboardLayout>("ktouch", 1, 0, "KeyboardLayout");
    qmlRegisterType<AbstractKey>("ktouch", 1, 0, "AbstractKey");
    qmlRegisterType<Key>("ktouch", 1, 0, "Key");
    qmlRegisterType<SpecialKey>("ktouch", 1, 0, "SpecialKey");
    qmlRegisterType<KeyChar>("ktouch", 1, 0, "KeyChar");
    qmlRegisterType<Course>("ktouch", 1, 0, "Course");
    qmlRegisterType<Lesson>("ktouch", 1, 0, "Lesson");
    qmlRegisterType<TrainingStats>("ktouch", 1, 0, "TrainingStats");
    qmlRegisterType<Profile>("ktouch", 1, 0, "Profile");
    qmlRegisterType<DataIndex>("ktouch", 1, 0, "DataIndex");
    qmlRegisterType<DataIndexCourse>("ktouch", 1, 0, "DataIndexCourse");
    qmlRegisterType<DataIndexKeyboardLayout>("ktouch", 1, 0, "DataIndexKeyboardLayout");
    qmlRegisterType<PreferencesProxy>("ktouch", 1, 0, "Preferences");
    qmlRegisterType<DataAccess>("ktouch", 1, 0, "DataAccess");
    qmlRegisterType<ProfileDataAccess>("ktouch", 1, 0, "ProfileDataAccess");

    qmlRegisterType<ResourceModel>("ktouch", 1, 0, "ResourceModel");
    qmlRegisterType<LessonModel>("ktouch", 1, 0, "LessonModel");
    qmlRegisterType<CategorizedResourceSortFilterProxyModel>("ktouch", 1, 0, "CategorizedResourceSortFilterProxyModel");
    qmlRegisterType<LearningProgressModel>("ktouch", 1, 0, "LearningProgressModel");
    qmlRegisterType<ErrorsModel>("ktouch", 1, 0, "ErrorsModel");

    qmlRegisterType<GridItem>("ktouch", 1, 0 , "Grid");
    qmlRegisterType<ScaleBackgroundItem>("ktouch", 1, 0, "ScaleBackgroundItem");
    qmlRegisterType<LessonPainter>("ktouch", 1, 0, "LessonPainter");
    qmlRegisterType<TrainingLineCore>("ktouch", 1, 0, "TrainingLineCore");
}

void Application::migrateKde4Files()
{
    QStringList configFiles;
    configFiles << QLatin1String("ktouchrc");
    Kdelibs4ConfigMigrator confMigrator(QLatin1String("ktouch"));
    confMigrator.setConfigFiles(configFiles);
    confMigrator.migrate();

    Kdelibs4Migration migration;
    const QDir dataDir = QDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    if (!dataDir.exists())
    {
        dataDir.mkpath(dataDir.path());
    }
    const QString dbPath = dataDir.filePath("profiles.db");
    const QString oldDbPath = migration.locateLocal("data", QStringLiteral("ktouch/profiles.db"));
    if (!QFile(dbPath).exists() && !oldDbPath.isEmpty())
    {
        QFile(oldDbPath).copy(dbPath);
    }

}
