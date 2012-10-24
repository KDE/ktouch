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

#include "mainwindow.h"

#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QGLWidget>
#include <QMenu>
#include <QVariant>

#include <KStandardDirs>
#include <KMenu>
#include <KCmdLineArgs>
#include <KActionCollection>
#include <KStandardAction>
#include <KHelpMenu>
#include <KToggleFullScreenAction>
#include <KShortcutsDialog>
#include <KConfigDialog>
#include <KCMultiDialog>

#include "editor/resourceeditor.h"
#include "application.h"
#include "preferences.h"
#include "trainingconfigwidget.h"
#include "colorsconfigwidget.h"

#ifdef KTOUCH_BUILD_WITH_X11
#include "x11_helper.h"
#else
#include "keyboardlayoutmenu.h"
#endif

#ifdef KTOUCH_BUILD_WITH_X11
#else
#endif

MainWindow::MainWindow(QWidget* parent):
    KMainWindow(parent),
    m_view(new QDeclarativeView(this)),
    m_actionCollection(new KActionCollection(this)),
    m_menu(new QMenu(this)),
    m_useOpenGLViewport(false)
{
#ifdef KTOUCH_BUILD_WITH_X11
    m_XEventNotifier = new XEventNotifier();
    m_XEventNotifier->start();
    connect(m_XEventNotifier, SIGNAL(layoutChanged()), SIGNAL(keyboardLayoutNameChanged()));
#else
    m_keyboardLayoutMenu = new KeyboardLayoutMenu(this);
    m_keyboardLayoutMenu->setDataIndex(Application::dataIndex());
    connect(m_keyboardLayoutMenu, SIGNAL(keyboardLayoutNameChanged()), SIGNAL(keyboardLayoutNameChanged()));
#endif
    init();
}

MainWindow::~MainWindow()
{
#ifdef KTOUCH_BUILD_WITH_X11
    m_XEventNotifier->stop();
    delete m_XEventNotifier;
#endif
}

QString MainWindow::keyboardLayoutName() const
{
#ifdef KTOUCH_BUILD_WITH_X11
    return X11Helper::getCurrentLayout().toString();
#else
    return m_keyboardLayoutMenu->keyboardLayoutName();
#endif
}

DataIndex *MainWindow::dataIndex()
{
    return Application::dataIndex();
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

void MainWindow::showResourceEditor()
{
    QWeakPointer<ResourceEditor>& resourceEditorRef = Application::resourceEditorRef();
    if (resourceEditorRef.isNull())
    {
        resourceEditorRef = new ResourceEditor();
    }

    ResourceEditor* resourceEditor = resourceEditorRef.data();

    resourceEditor->show();
    resourceEditor->activateWindow();
}


void MainWindow::showConfigDialog()
{
    if (KConfigDialog::showDialog("preferences"))
    {
        return;
    }

    KConfigDialog* dialog = new KConfigDialog(this, "preferences", Preferences::self());
    dialog->setFaceType(KPageDialog::List);
    dialog->setModal(true);
    dialog->addPage(new TrainingConfigWidget(), i18n("Training"), "chronometer", "Training settings");
    dialog->addPage(new ColorsConfigWidget(), i18n("Colors"), "preferences-desktop-color", "Color settings");
    dialog->show();
}

void MainWindow::configureShortcuts()
{
    KShortcutsDialog::configure(m_actionCollection, KShortcutsEditor::LetterShortcutsDisallowed, this);
}

void MainWindow::configureKeyboard()
{
    KCMultiDialog kcm;

    kcm.setWindowTitle(i18n("Configure Keyboard"));
    kcm.addModule("kcm_keyboard");
    kcm.exec();
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
    KAction* editorAction = new KAction(i18n("Course and Keyboard Layout Editor..."), this);
    connect(editorAction, SIGNAL(triggered()), SLOT(showResourceEditor()));
    m_actionCollection->addAction("editor", editorAction);
    m_menu->addAction(editorAction);
    m_menu->addSeparator();
    m_menu->addAction(KStandardAction::preferences(this, SLOT(showConfigDialog()), m_actionCollection));
    m_menu->addAction(KStandardAction::keyBindings(this, SLOT(configureShortcuts()), m_actionCollection));


#ifdef KTOUCH_BUILD_WITH_X11
    KAction* configureKeyboardAction = new KAction(i18n("Configure Keyboard..."), this);
    m_menu->addAction(configureKeyboardAction);
    connect(configureKeyboardAction, SIGNAL(triggered()), SLOT(configureKeyboard()));
#else
    m_menu->addMenu(m_keyboardLayoutMenu);
#endif

    m_menu->addSeparator();
    KHelpMenu* helpMenu = new KHelpMenu(m_menu, KCmdLineArgs::aboutData(), false, m_actionCollection);
    m_menu->addMenu(helpMenu->menu());

    setCentralWidget(m_view);

    Application::setupDeclarativeBindings(m_view->engine());

    m_view->setMinimumSize(1000, 700);
    m_view->setStyleSheet("background-color: transparent;");
    m_view->rootContext()->setContextObject(this);
    m_view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    m_view->setSource(QUrl::fromLocalFile(KGlobal::dirs()->findResource("appdata", "qml/main.qml")));
}
