/*
 *  SPDX-FileCopyrightText: 2016 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "ktouchcontext.h"

#include <QMenu>
#include <QPointer>
#include <QDialogButtonBox>
#include <QQuickView>

#include "application.h"
#include "colorsconfigwidget.h"
#include "editor/resourceeditor.h"
#include "customlessoneditordialog.h"
#include "preferences.h"
#include "trainingconfigwidget.h"
#include "core/lesson.h"

#include <kxmlgui_version.h>
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
#endif



const QString keyboardKCMName = QStringLiteral("kcm_keyboard");

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
    connect(m_XEventNotifier, &XEventNotifier::layoutChanged, this, &KTouchContext::keyboardLayoutNameChanged);
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
    return "unknown";
#endif
}

DataIndex* KTouchContext::dataIndex()
{
    return Application::dataIndex();
}

bool KTouchContext::keyboardKCMAvailable()
{
    return testKCMAvailibility(keyboardKCMName);
}

void KTouchContext::showMenu(int xPos, int yPos)
{
    m_menu->popup(m_view->mapToGlobal(QPoint(xPos, yPos)));
}

Lesson* KTouchContext::createLesson()
{
    return new Lesson();
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
    if (KConfigDialog::showDialog(QStringLiteral("preferences")))
    {
        return;
    }

    KConfigDialog* dialog = new KConfigDialog(m_mainWindow, QStringLiteral("preferences"), Preferences::self());
    dialog->setFaceType(KPageDialog::List);
    dialog->setModal(true);
    dialog->addPage(new TrainingConfigWidget(), i18n("Training"), QStringLiteral("chronometer"), i18n("Training Settings"));
    dialog->addPage(new ColorsConfigWidget(), i18n("Colors"), QStringLiteral("preferences-desktop-color"), i18n("Color Settings"));
    dialog->show();
}

void KTouchContext::configureShortcuts()
{
#if KXMLGUI_VERSION >= QT_VERSION_CHECK(5, 84, 0)
    KShortcutsDialog::showDialog(m_actionCollection, KShortcutsEditor::LetterShortcutsDisallowed, m_mainWindow);
#else
    KShortcutsDialog::configure(m_actionCollection, KShortcutsEditor::LetterShortcutsDisallowed, m_mainWindow);
#endif
}

void KTouchContext::showKeyboardKCM()
{
    QPointer<KCMultiDialog> kcm = new KCMultiDialog(m_mainWindow);

    kcm->setWindowTitle(i18nc("@title:window", "Configure Keyboard"));
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
    m_actionCollection->addAction(QStringLiteral("editor"), editorAction);
    m_menu->addAction(editorAction);
    m_menu->addSeparator();
    m_menu->addAction(KStandardAction::preferences(this, SLOT(showConfigDialog()), m_actionCollection));
    m_menu->addAction(KStandardAction::keyBindings(this, SLOT(configureShortcuts()), m_actionCollection));

    if (testKCMAvailibility(keyboardKCMName))
    {
        QAction* configureKeyboardAction = new QAction(i18n("Configure Keyboard..."), this);
        m_menu->addAction(configureKeyboardAction);
        connect(configureKeyboardAction, &QAction::triggered, this, &KTouchContext::showKeyboardKCM);
    }

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

    return service->hasServiceType(QStringLiteral("KCModule")) && !service->noDisplay();
}
