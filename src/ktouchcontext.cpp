/*
 *  Copyright 2016  Sebastian Gottfried <sebastian.gottfried@posteo.de>
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

#include "ktouchcontext.h"

#include <QMenu>
#include <QPointer>
#include <QDialogButtonBox>
#include <QMenu>
#include <QQuickView>

#include "application.h"
#include "colorsconfigwidget.h"
#include "editor/resourceeditor.h"
#include "customlessoneditordialog.h"
#include "preferences.h"
#include "trainingconfigwidget.h"

#include <KActionCollection>
#include <KStandardAction>
#include <KHelpMenu>
#include <KToggleFullScreenAction>
#include <KShortcutsDialog>
#include <KConfigDialog>
#include <KCMultiDialog>
#include <KLocalizedString>

#ifdef KTOUCH_BUILD_WITH_X11
#include "x11_helper.h"
#else
#include "keyboardlayoutmenu.h"
#endif



const QString keyboardKCMName = "kcm_keyboard";

KTouchContext::KTouchContext(KMainWindow* mainWindow, QQuickView* view, QObject *parent) :
    QObject(parent),
    m_actionCollection(new KActionCollection(this)),
    m_menu(new QMenu(mainWindow)),
    m_mainWindow(mainWindow),
    m_view(view)
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

KTouchContext::~KTouchContext()
{
#ifdef KTOUCH_BUILD_WITH_X11
    m_XEventNotifier->stop();
    delete m_XEventNotifier;
#endif
}

QString KTouchContext::keyboardLayoutName() const
{
#ifdef KTOUCH_BUILD_WITH_X11
    return X11Helper::getCurrentLayout().toString();
#else
    return m_keyboardLayoutMenu->keyboardLayoutName();
#endif
}

DataIndex* KTouchContext::dataIndex()
{
    return Application::dataIndex();
}

void KTouchContext::showMenu(int xPos, int yPos)
{
    m_menu->popup(m_view->mapToGlobal(QPoint(xPos, yPos)));
}

void KTouchContext::showResourceEditor()
{
    QPointer<ResourceEditor>& resourceEditorRef = Application::resourceEditorRef();
    if (resourceEditorRef.isNull())
    {
        resourceEditorRef = QPointer<ResourceEditor>(new ResourceEditor());
    }

    ResourceEditor* resourceEditor = resourceEditorRef.data();

    resourceEditor->show();
    resourceEditor->activateWindow();
}

bool KTouchContext::showCustomLessonDialog(Lesson* lesson, KeyboardLayout* keyboardLayout)
{
    CustomLessonEditorDialog* dialog = new CustomLessonEditorDialog(m_mainWindow);

    dialog->setLesson(lesson);
    dialog->setKeyboardLayout(keyboardLayout);

    bool result = dialog->exec() == QDialog::Accepted;

    delete dialog;

    return result;
}

void KTouchContext::showConfigDialog()
{
    if (KConfigDialog::showDialog("preferences"))
    {
        return;
    }

    KConfigDialog* dialog = new KConfigDialog(m_mainWindow, "preferences", Preferences::self());
    dialog->setFaceType(KPageDialog::List);
    dialog->setModal(true);
    dialog->addPage(new TrainingConfigWidget(), i18n("Training"), "chronometer", i18n("Training Settings"));
    dialog->addPage(new ColorsConfigWidget(), i18n("Colors"), "preferences-desktop-color", i18n("Color Settings"));
    dialog->show();
}

void KTouchContext::configureShortcuts()
{
    KShortcutsDialog::configure(m_actionCollection, KShortcutsEditor::LetterShortcutsDisallowed, m_mainWindow);
}

void KTouchContext::configureKeyboard()
{
    QPointer<KCMultiDialog> kcm = new KCMultiDialog(m_mainWindow);

    kcm->setWindowTitle(i18n("Configure Keyboard"));
    kcm->addModule(keyboardKCMName);
    kcm->exec();

    delete kcm;
}

void KTouchContext::setFullscreen(bool fullScreen)
{
    KToggleFullScreenAction::setFullScreen(m_mainWindow, fullScreen);
}

void KTouchContext::init()
{
    m_actionCollection->addAssociatedWidget(m_mainWindow);
    m_menu->addAction(KStandardAction::fullScreen(this, SLOT(setFullscreen(bool)), m_mainWindow, m_actionCollection));
    m_menu->addSeparator();
    QAction* editorAction = new QAction(i18n("Course and Keyboard Layout Editor..."), this);
    connect(editorAction, &QAction::triggered, this, &KTouchContext::showResourceEditor);
    m_actionCollection->addAction("editor", editorAction);
    m_menu->addAction(editorAction);
    m_menu->addSeparator();
    m_menu->addAction(KStandardAction::preferences(this, SLOT(showConfigDialog()), m_actionCollection));
    m_menu->addAction(KStandardAction::keyBindings(this, SLOT(configureShortcuts()), m_actionCollection));


#ifdef KTOUCH_BUILD_WITH_X11
    if (testKCMAvailibility(keyboardKCMName))
    {
        QAction* configureKeyboardAction = new QAction(i18n("Configure Keyboard..."), this);
        m_menu->addAction(configureKeyboardAction);
        connect(configureKeyboardAction, &QAction::triggered, this, &KTouchContext::configureKeyboard);
    }
#else
    m_menu->addMenu(m_keyboardLayoutMenu);
#endif

    m_menu->addSeparator();
    KHelpMenu* helpMenu = new KHelpMenu(m_mainWindow);
    m_menu->addMenu(helpMenu->menu());
}

bool KTouchContext::testKCMAvailibility(const QString& name)
{
    KService::Ptr service = KService::serviceByStorageId(name + ".desktop");

    if (!service)
    {
        return false;
    }

    return service->hasServiceType("KCModule") && !service->noDisplay();
}
