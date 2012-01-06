#include "mainwindow.h"

#include <qdeclarative.h>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QGraphicsDropShadowEffect>
#include <QGLWidget>
#include <QMenu>

#include <kstandarddirs.h>
#include <kdeclarative.h>
#include <kmenu.h>
#include <kcmdlineargs.h>
#include <kactioncollection.h>
#include <kstandardaction.h>
#include <khelpmenu.h>
#include <ktogglefullscreenaction.h>
#include <kshortcutsdialog.h>
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
    m_view(new QDeclarativeView(this)),
    m_actionCollection(new KActionCollection(this)),
    m_menu(new QMenu(this)),
    m_useOpenGLViewport(false)
{
    init();
}

bool MainWindow::useOpenGLViewport() const
{
    return m_useOpenGLViewport;
}

void MainWindow::setUseOpenGLViewport(bool useOpenGLViewport)
{
    if (useOpenGLViewport != m_useOpenGLViewport)
    {
        m_useOpenGLViewport = useOpenGLViewport;
        m_view->setViewport(useOpenGLViewport? new QGLWidget(): new QWidget());
    }
}

void MainWindow::showMenu(int xPos, int yPos)
{
    m_menu->popup(m_view->mapToGlobal(QPoint(xPos, yPos)));
}

void MainWindow::configureShortcuts()
{
    KShortcutsDialog::configure(m_actionCollection, KShortcutsEditor::LetterShortcutsDisallowed, this);
}

void MainWindow::showPreferences()
{
}

void MainWindow::setFullscreen(bool fullScreen)
{
    KToggleFullScreenAction::setFullScreen(this, fullScreen);
}

void MainWindow::init()
{
    m_actionCollection->addAssociatedWidget(this);
    m_menu->addAction(KStandardAction::fullScreen(this, SLOT(setFullscreen(bool)), this, m_actionCollection));
    m_menu->addSeparator();
    m_menu->addAction(KStandardAction::preferences(this, SLOT(showPreferences()), m_actionCollection));
    m_menu->addAction(KStandardAction::keyBindings(this, SLOT(configureShortcuts()), m_actionCollection));
    m_menu->addSeparator();
    KHelpMenu* helpMenu = new KHelpMenu(m_menu, KCmdLineArgs::aboutData(), false, m_actionCollection);
    m_menu->addMenu(helpMenu->menu());

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

    ViewContext* viewContext = new ViewContext(this, this);
    connect(viewContext, SIGNAL(menuRequested(int,int)), SLOT(showMenu(int,int)));

    m_view->setMinimumSize(1000, 700);
    m_view->setStyleSheet("background-color: transparent;");
    m_view->rootContext()->setContextObject(viewContext);
    m_view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    m_view->setSource(QUrl::fromLocalFile(KGlobal::dirs()->findResource("appdata", "qml/main.qml")));
}
