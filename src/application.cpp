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

#include <qdeclarative.h>
#include <QGraphicsDropShadowEffect>
#include <QScriptValue>
#include <QScriptEngine>
#include <QKeyEvent>

#include <kdeclarative.h>

#include "bindings/utils.h"
#include "bindings/stringformatter.h"
#include "declarativeitems/applicationbackground.h"
#include "declarativeitems/cursorshapearea.h"
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

Application::Application() :
    KApplication(true),
    m_dataIndex(new DataIndex(this))
{
    registerQmlTypes();

    DataAccess dataAccess;
    dataAccess.loadDataIndex(m_dataIndex);
}

DataIndex* Application::dataIndex()
{
    Application* app = qobject_cast<Application*>(QCoreApplication::instance());

    return app->m_dataIndex;
}

QWeakPointer<ResourceEditor>& Application::resourceEditorRef()
{
    Application* app = qobject_cast<Application*>(QCoreApplication::instance());

    return app->m_resourceEditorRef;
}

void Application::setupDeclarativeBindings(QDeclarativeEngine* declarativeEngine)
{
    KDeclarative kDeclarative;
    kDeclarative.setDeclarativeEngine(declarativeEngine);
    kDeclarative.initialize();
    kDeclarative.setupBindings();

    QScriptEngine* engine = kDeclarative.scriptEngine();
    QScriptValue globalObject = engine->globalObject();

    globalObject.setProperty("findImage", engine->newFunction(findImage));
    globalObject.setProperty("getSecondsOfQTime", engine->newFunction(getSecondsOfQTime));
    globalObject.setProperty("getMinutesOfQTime", engine->newFunction(getMinutesOfQTime));
    globalObject.setProperty("uuid", engine->newFunction(uuid));
    globalObject.setProperty("strFormatter", engine->newQObject(new StringFormatter(), QScriptEngine::ScriptOwnership));
}

void Application::registerQmlTypes()
{
    qmlRegisterType<QGraphicsDropShadowEffect>("Effects",1,0,"DropShadow");
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

    qmlRegisterType<ApplicationBackground>("ktouch", 1, 0, "ApplicationBackground");
    qmlRegisterType<CursorShapeArea>("ktouch", 1, 0 , "CursorShapeArea");
    qmlRegisterType<GridItem>("ktouch", 1, 0 , "Grid");
    qmlRegisterType<ScaleBackgroundItem>("ktouch", 1, 0, "ScaleBackgroundItem");
    qmlRegisterType<LessonPainter>("ktouch", 1, 0, "LessonPainter");
    qmlRegisterType<TrainingLineCore>("ktouch", 1, 0, "TrainingLineCore");
}
