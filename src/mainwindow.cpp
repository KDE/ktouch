#include "mainwindow.h"

#include <qdeclarative.h>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QGraphicsDropShadowEffect>

#include <kstandarddirs.h>
#include <kdeclarative.h>
#include <plasma/theme.h>

#include "core/keyboardlayout.h"
#include "core/key.h"
#include "core/specialkey.h"
#include "core/keychar.h"
#include "core/course.h"
#include "core/lesson.h"
#include "core/lessonline.h"
#include "core/trainingstats.h"
#include "core/dataindex.h"
#include "core/dataaccess.h"
#include "viewcontext.h"

MainWindow::MainWindow(QWidget* parent):
    KMainWindow(parent),
    m_view(new QDeclarativeView(this))
{
    init();
}

void MainWindow::init()
{
    Plasma::Theme* theme = Plasma::Theme::defaultTheme();
    theme->setUseGlobalSettings(false);
    theme->setThemeName("ktouch");

    qmlRegisterType<QGraphicsDropShadowEffect>("Effects",1,0,"DropShadow");
    qmlRegisterType<KeyboardLayout>("ktouch", 1, 0, "KeyboardLayout");
    qmlRegisterType<AbstractKey>("ktouch", 1, 0, "AbstractKey");
    qmlRegisterType<Key>("ktouch", 1, 0, "Key");
    qmlRegisterType<SpecialKey>("ktouch", 1, 0, "SpecialKey");
    qmlRegisterType<KeyChar>("ktouch", 1, 0, "KeyChar");
    qmlRegisterType<Course>("ktouch", 1, 0, "Course");
    qmlRegisterType<Lesson>("ktouch", 1, 0, "Lesson");
    qmlRegisterType<LessonLine>("ktouch", 1, 0, "LessonLine");
    qmlRegisterType<TrainingStats>("ktouch", 1, 0, "TrainingStats");
    qmlRegisterType<DataIndex>("ktouch", 1, 0, "DataIndex");
    qmlRegisterType<DataIndexCourse>("ktouch", 1, 0, "DataIndexCourse");
    qmlRegisterType<DataIndexKeyboardLayout>("ktouch", 1, 0, "DataIndexKeyboardLayout");
    qmlRegisterType<DataAccess>("ktouch", 1, 0, "DataAccess");

    KDeclarative kDeclarative;
    kDeclarative.setDeclarativeEngine(m_view->engine());
    kDeclarative.initialize();
    kDeclarative.setupBindings();

    setCentralWidget(m_view);

    m_view->setMinimumSize(1000, 700);
    m_view->setStyleSheet("background-color: transparent;");
    m_view->rootContext()->setContextObject(new ViewContext(this, this));
    m_view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    m_view->setSource(QUrl::fromLocalFile(KGlobal::dirs()->findResource("appdata", "qml/main.qml")));
}
