/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "resourceeditor.h"

#include <QUuid>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QPointer>
#include <QTimer>
#include <QAbstractItemView>
#include <QStandardPaths>
#include <QUndoGroup>

#include <KToolBar>
#include <KActionCollection>
#include <KStandardAction>
#include <KLocalizedString>
#include <KMessageBox>

#include "core/dataindex.h"
#include "core/dataaccess.h"
#include "core/resource.h"
#include "core/course.h"
#include "core/lesson.h"
#include "core/keyboardlayout.h"
#include "core/resourcedataaccess.h"
#include "core/userdataaccess.h"
#include "models/resourcemodel.h"
#include "models/categorizedresourcesortfilterproxymodel.h"
#include "resourceeditorwidget.h"
#include "newresourceassistant.h"
#include "application.h"

ResourceEditor::ResourceEditor(QWidget *parent) :
    KMainWindow(parent),
    m_dataIndex(Application::dataIndex()),
    m_resourceModel(new ResourceModel(this)),
    m_categorizedResourceModel(new CategorizedResourceSortFilterProxyModel(this)),
    m_currentResource(0),
    m_backupResource(0),
    m_undoGroup(new QUndoGroup(this)),
    m_actionCollection(new KActionCollection(this)),
    m_newResourceAction(KStandardAction::openNew(this, SLOT(newResource()), m_actionCollection)),
    m_deleteResourceAction(new QAction(QIcon::fromTheme(QStringLiteral("edit-delete")), i18n("Delete"), this)),
    m_undoAction(KStandardAction::undo(m_undoGroup, SLOT(undo()), m_actionCollection)),
    m_redoAction(KStandardAction::redo(m_undoGroup, SLOT(redo()), m_actionCollection)),
    m_importResourceAction(new QAction(QIcon::fromTheme(QStringLiteral("document-import")), i18n("Import"), this)),
    m_exportResourceAction(new QAction(QIcon::fromTheme(QStringLiteral("document-export")), i18n("Export"), this)),
    m_editorWidget(new ResourceEditorWidget(this)),
    m_saveTimer(new QTimer(this))

{
    m_resourceModel->setDataIndex(m_dataIndex);
    m_categorizedResourceModel->setResourceModel(m_resourceModel);
    m_categorizedResourceModel->setCategorizedModel(true);

    const int unit = fontMetrics().height();
    setMinimumSize(39 * unit, 28 * unit);
    setCaption(i18nc("@title:window", "Course and Keyboard Layout Editor"));

    m_newResourceAction->setToolTip(i18n("Create a new course or keyboard layout"));
    m_deleteResourceAction->setEnabled(false);
    m_deleteResourceAction->setToolTip(i18n("Delete the current selected course or keyboard layout"));
    m_actionCollection->addAction(QStringLiteral("deleteResource"), m_deleteResourceAction);
    connect(m_deleteResourceAction, &QAction::triggered, this, &ResourceEditor::deleteResource);
    m_undoAction->setEnabled(false);
    connect(m_undoGroup, &QUndoGroup::canUndoChanged, m_undoAction, &QAction::setEnabled);
    connect(m_undoGroup, &QUndoGroup::undoTextChanged, this, &ResourceEditor::setUndoText);
    m_redoAction->setEnabled(false);
    connect(m_undoGroup, &QUndoGroup::canRedoChanged, m_redoAction, &QAction::setEnabled);
    connect(m_undoGroup, &QUndoGroup::redoTextChanged, this, &ResourceEditor::setRedoText);
    m_actionCollection->addAction(QStringLiteral("importResource"), m_importResourceAction);
    m_importResourceAction->setToolTip(i18n("Import a course or keyboard layout from a file"));
    connect(m_importResourceAction, &QAction::triggered, this, &ResourceEditor::importResource);
    m_exportResourceAction->setToolTip(i18n("Export the current selected course or keyboard layout to a file"));
    m_actionCollection->addAction(QStringLiteral("exportResource"), m_exportResourceAction);
    connect(m_exportResourceAction, &QAction::triggered, this, &ResourceEditor::exportResource);
    m_exportResourceAction->setEnabled(false);

    toolBar()->addAction(m_newResourceAction);
    toolBar()->addAction(m_deleteResourceAction);
    toolBar()->addSeparator();
    toolBar()->addAction(m_undoAction);
    toolBar()->addAction(m_redoAction);
    toolBar()->addSeparator();
    toolBar()->addAction(m_importResourceAction);
    toolBar()->addAction(m_exportResourceAction);

    setCentralWidget(m_editorWidget);

    m_editorWidget->setResourceModel(m_resourceModel);
    m_editorWidget->setUndoGroup(m_undoGroup);

    QAbstractItemView* const resourceView = m_editorWidget->resourceView();
    resourceView->setModel(m_categorizedResourceModel);
    connect(resourceView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ResourceEditor::onResourceSelected);

    selectFirstResource();

    connect(m_editorWidget, &ResourceEditorWidget::resourceRestorationRequested, this, &ResourceEditor::restoreResourceBackup);
    connect(m_editorWidget, &ResourceEditorWidget::resourceRestorationDismissed, this, &ResourceEditor::clearResourceBackup);

    connect(m_saveTimer, &QTimer::timeout, this, &ResourceEditor::save);
    m_saveTimer->setInterval(60000);
}

ResourceEditor::~ResourceEditor()
{
    if (m_backupResource)
    {
        delete m_backupResource;
    }
}

void ResourceEditor::closeEvent(QCloseEvent* event)
{
    save();
    KMainWindow::closeEvent(event);
}

void ResourceEditor::newResource()
{
    QPointer<NewResourceAssistant> assistant = new NewResourceAssistant(m_resourceModel, this);

    if (assistant->exec() == QDialog::Accepted)
    {
        save();
        Resource* resource = assistant->createResource();
        if (Resource* dataIndexResource = storeResource(resource))
        {
            m_editorWidget->clearUndoStackForResource(dataIndexResource);
            selectDataResource(dataIndexResource);
        }
        delete resource;
    }

    delete assistant;
}

void ResourceEditor::deleteResource()
{
    Q_ASSERT(m_currentResource);

    save();

    // HACK: disable categorization temporarily as mitigation against kdelibs bug 303228
    m_categorizedResourceModel->setCategorizedModel(false);

    DataAccess dataAccess;
    UserDataAccess userDataAccess;

    if (DataIndexCourse* course = qobject_cast<DataIndexCourse*>(m_currentResource))
    {
        for (int i = 0; i < m_dataIndex->courseCount(); i++)
        {
            if (m_dataIndex->course(i) == course)
            {
                Course* backup = new Course();
                if (!dataAccess.loadCourse(m_dataIndex->course(i), backup))
                {
                    KMessageBox::error(this, i18n("Error while opening course"));
                    return;
                }
                if (!userDataAccess.deleteCourse(backup))
                {
                    delete backup;
                    KMessageBox::error(this, i18n("Error while deleting course"));
                    return;
                }
                prepareResourceRestore(backup);
                m_dataIndex->removeCourse(i);
            }
        }
    }
    else if (DataIndexKeyboardLayout* keyboardLayout = qobject_cast<DataIndexKeyboardLayout*>(m_currentResource))
    {
        for (int i = 0; i < m_dataIndex->keyboardLayoutCount(); i++)
        {
            if (m_dataIndex->keyboardLayout(i) == keyboardLayout)
            {
                KeyboardLayout* backup = new KeyboardLayout();
                if (!dataAccess.loadKeyboardLayout(m_dataIndex->keyboardLayout(i), backup))
                {
                    KMessageBox::error(this, i18n("Error while opening keyboard layout"));
                    return;
                }
                if (!userDataAccess.deleteKeyboardLayout(backup))
                {
                    KMessageBox::error(this, i18n("Error while deleting keyboard layout"));
                    return;
                }
                prepareResourceRestore(backup);
                m_dataIndex->removeKeyboardLayout(i);
            }
        }
    }

    // HACK: reactivate categorization again
    m_categorizedResourceModel->setCategorizedModel(true);

    selectFirstResource();
}

void ResourceEditor::importResource()
{
    const QString path = QFileDialog::getOpenFileName(this, QString(), QString(), i18n("XML files (*.xml)"));

    if (!path.isNull())
    {
        save();

        if (importCourse(path))
            return;

        if (importKeyboardLayout(path))
            return;

        KMessageBox::error(this, i18n("The selected file could not be imported."));
    }
}

void ResourceEditor::exportResource()
{
    Q_ASSERT(m_currentResource);

    DataAccess dataAccess;
    ResourceDataAccess resourceDataAccess;

    save();

    if (DataIndexCourse* dataIndexCourse = qobject_cast<DataIndexCourse*>(m_currentResource))
    {
        for (int i = 0; i < m_dataIndex->courseCount(); i++)
        {
            if (m_dataIndex->course(i) == dataIndexCourse)
            {
                Course* course = new Course();

                if (!dataAccess.loadCourse(m_dataIndex->course(i), course))
                {
                    KMessageBox::error(this, i18n("Error while opening course"));
                    delete course;
                    return;
                }

                const QString initialFileName(QStringLiteral("%1.xml").arg(course->keyboardLayoutName()));
                const QString path(QFileDialog::getSaveFileName(this, QString(), initialFileName, i18n("XML files (*.xml)")));

                if (!path.isNull())
                {
                    if (!resourceDataAccess.storeCourse(path, course))
                    {
                        KMessageBox::error(this, i18n("Error while saving course"));
                        delete course;
                        return;
                    }
                }
                delete course;
            }
        }
    }
    else if (DataIndexKeyboardLayout* dataIndexkeyboardLayout = qobject_cast<DataIndexKeyboardLayout*>(m_currentResource))
    {
        for (int i = 0; i < m_dataIndex->keyboardLayoutCount(); i++)
        {
            if (m_dataIndex->keyboardLayout(i) == dataIndexkeyboardLayout)
            {
                KeyboardLayout* keyboardlayout = new KeyboardLayout();

                if (!dataAccess.loadKeyboardLayout(m_dataIndex->keyboardLayout(i), keyboardlayout))
                {
                    KMessageBox::error(this, i18n("Error while opening keyboard layout"));
                    delete keyboardlayout;
                    return;
                }

                const QString initialFileName(QStringLiteral("%1.xml").arg(keyboardlayout->name()));
                const QString path(QFileDialog::getSaveFileName(this, QString(), initialFileName, i18n("XML files (*.xml)")));

                if (!path.isNull())
                {
                    if (!resourceDataAccess.storeKeyboardLayout(path, keyboardlayout))
                    {
                        KMessageBox::error(this, i18n("Error while saving keyboard layout"));
                        delete keyboardlayout;
                        return;
                    }
                }
                delete keyboardlayout;
            }
        }
    }
}

void ResourceEditor::onResourceSelected()
{
    QAbstractItemView* const resourceView = m_editorWidget->resourceView();

    save();

    if (resourceView->selectionModel()->hasSelection())
    {
        QModelIndex current = resourceView->selectionModel()->selectedIndexes().first();
        const QVariant variant = resourceView->model()->data(current, ResourceModel::DataRole);
        QObject* const obj = variant.value<QObject*>();
        m_currentResource = qobject_cast<Resource*>(obj);
        const DataIndex::Source source = static_cast<DataIndex::Source>(resourceView->model()->data(current, ResourceModel::SourceRole).toInt());

        Q_ASSERT(m_currentResource);

        m_deleteResourceAction->setEnabled(source == DataIndex::UserResource);
        m_exportResourceAction->setEnabled(true);
        m_editorWidget->openResource(m_currentResource);
    }
    else
    {
        m_currentResource = 0;
        m_deleteResourceAction->setEnabled(false);
        m_exportResourceAction->setEnabled(false);
    }
}

void ResourceEditor::restoreResourceBackup()
{
    Q_ASSERT(m_backupResource);

    save();

    if (Resource* dataIndexResource = storeResource(m_backupResource))
    {
        selectDataResource(dataIndexResource);
    }

    clearResourceBackup();
}

void ResourceEditor::clearResourceBackup()
{
    Q_ASSERT(m_backupResource);

    delete m_backupResource;
    m_backupResource = 0;
}

void ResourceEditor::save()
{
    if (m_undoGroup->activeStack() && !m_undoGroup->activeStack()->isClean())
    {
        m_editorWidget->save();
    }

    m_saveTimer->start();
}

void ResourceEditor::setUndoText(const QString& text)
{
    m_undoAction->setToolTip(text);
}

void ResourceEditor::setRedoText(const QString& text)
{
    m_redoAction->setToolTip(text);
}

void ResourceEditor::prepareResourceRestore(Resource* backup)
{
    QString msg;

    if (Course* course = qobject_cast<Course*>(backup))
    {
        msg = i18n("Course <b>%1</b> deleted", course->title().toHtmlEscaped());
    }
    else if (KeyboardLayout* keyboardLayout = qobject_cast<KeyboardLayout*>(backup))
    {
        msg = i18n("Keyboard layout <b>%1</b> deleted", keyboardLayout->title().toHtmlEscaped());
    }

    m_editorWidget->showMessage(ResourceEditorWidget::ResourceDeletedMsg, msg);

    if (m_backupResource)
    {
        delete m_backupResource;
    }

    m_backupResource = backup;
}

Resource* ResourceEditor::storeResource(Resource* resource, Resource* dataIndexResource)
{
    // FIXME: Is all this path mangling necessary?
    UserDataAccess userDataAccess;

    if (Course* course = qobject_cast<Course*>(resource))
    {
        DataIndexCourse* dataIndexCourse = dataIndexResource == 0?
            new DataIndexCourse():
            dynamic_cast<DataIndexCourse*>(dataIndexResource);

        QDir dir = QDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
        dir.mkpath(QStringLiteral("courses"));
        dir.cd(QStringLiteral("courses"));

        QString path = dataIndexResource == 0?
            dir.filePath(QStringLiteral("%1.xml").arg(course->id())):
            dataIndexCourse->path();

        if (!userDataAccess.storeCourse(course))
        {
            KMessageBox::error(this, i18n("Error while saving course to disk."));
            return 0;
        }

        dataIndexCourse->setSource(DataIndex::UserResource);
        dataIndexCourse->setTitle(course->title());
        dataIndexCourse->setDescription(course->description());
        dataIndexCourse->setKeyboardLayoutName(course->keyboardLayoutName());
        dataIndexCourse->setId(course->id());
        dataIndexCourse->setPath(path);

        if (dataIndexResource == 0)
        {
            m_dataIndex->addCourse(dataIndexCourse);
        }

        dataIndexResource = dataIndexCourse;
    }
    else if (KeyboardLayout* keyboardLayout = qobject_cast<KeyboardLayout*>(resource))
    {
        DataIndexKeyboardLayout* dataIndexKeyboardLayout = dataIndexResource == 0?
            new DataIndexKeyboardLayout():
            qobject_cast<DataIndexKeyboardLayout*>(dataIndexResource);

        QDir dir = QDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
        dir.mkpath(QStringLiteral("keyboardlayouts"));
        dir.cd(QStringLiteral("keyboardlayouts"));

        QString path = dataIndexResource == 0?
            dir.filePath(QStringLiteral("%1.xml").arg(keyboardLayout->id())):
            dataIndexKeyboardLayout->path();

        if (!userDataAccess.storeKeyboardLayout(keyboardLayout))
        {
            KMessageBox::error(this, i18n("Error while saving keyboard layout to disk."));
            return 0;
        }

        dataIndexKeyboardLayout->setSource(DataIndex::UserResource);
        dataIndexKeyboardLayout->setName(keyboardLayout->name());
        dataIndexKeyboardLayout->setTitle(keyboardLayout->title());
        dataIndexKeyboardLayout->setId(keyboardLayout->id());
        dataIndexKeyboardLayout->setPath(path);

        if (dataIndexResource == 0)
        {
            m_dataIndex->addKeyboardLayout(dataIndexKeyboardLayout);
        }

        dataIndexResource = dataIndexKeyboardLayout;
    }

    return dataIndexResource;
}


void ResourceEditor::selectDataResource(Resource* resource)
{
    QAbstractItemView* const resourceView = m_editorWidget->resourceView();
    QAbstractItemModel* const model = resourceView->model();

    resourceView->selectionModel()->clearSelection();

    for (int i = 0; i < model->rowCount(); i++)
    {
        const QModelIndex index = model->index(i, 0);
        const QVariant var = model->data(index, ResourceModel::DataRole);
        QObject* obj = var.value<QObject*>();

        if (obj == resource)
        {
            resourceView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
            break;
        }
    }
}

void ResourceEditor::selectFirstResource()
{
    QAbstractItemView* const resourceView = m_editorWidget->resourceView();

    if (resourceView->model()->rowCount() > 0)
    {
        resourceView->selectionModel()->select(resourceView->model()->index(0, 0), QItemSelectionModel::ClearAndSelect);
    }
}

bool ResourceEditor::importCourse(const QString& path)
{
    ResourceDataAccess resourceDataAccess;
    Course course;

    if (!resourceDataAccess.loadCourse(path, &course))
        return false;

    DataIndexCourse* overwriteDataIndexCourse(0);

    for (int i = 0; i < m_dataIndex->courseCount(); i++)
    {
        DataIndexCourse* const testCourse = m_dataIndex->course(i);

        if (testCourse->source() == DataIndex::BuiltInResource &&  testCourse->id() == course.id())
        {
            switch (KMessageBox::questionYesNo(this, i18n("The selected course is already present as a built-in course."), QString(),
                                               KGuiItem(i18n("Import as new course"), QStringLiteral("dialog-ok")),
                                               KStandardGuiItem::cancel()
            ))
            {
                case KMessageBox::Yes:
                    course.setId(QUuid::createUuid().toString());
                    for (int j = 0; j < course.lessonCount(); j++)
                    {
                        Lesson* const lesson = course.lesson(j);
                        lesson->setId(QUuid::createUuid().toString());
                    }
                    break;
                default:
                    return true;
            }
        }

        if (testCourse->source() == DataIndex::UserResource &&  testCourse->id() == course.id())
        {
            switch (KMessageBox::questionYesNoCancel(this, i18n("The selected course is already present as a user course."), QString(),
                                               KGuiItem(i18n("Import as new course"), QStringLiteral("dialog-ok")),
                                               KStandardGuiItem::overwrite(),
                                               KStandardGuiItem::cancel()
            ))
            {
                case KMessageBox::Yes:
                    course.setId(QUuid::createUuid().toString());
                    for (int j = 0; j < course.lessonCount(); j++)
                    {
                        Lesson* const lesson = course.lesson(j);
                        lesson->setId(QUuid::createUuid().toString());
                    }
                    break;
                case KMessageBox::No:
                    overwriteDataIndexCourse = testCourse;
                    break;
                default:
                    return true;
            }
        }
    }

    if (Resource* dataIndexResource = storeResource(&course, overwriteDataIndexCourse))
    {
        m_editorWidget->clearUndoStackForResource(dataIndexResource);
        selectDataResource(dataIndexResource);
    }

    return true;
}

bool ResourceEditor::importKeyboardLayout(const QString& path)
{
    ResourceDataAccess resourceDataAccess;
    KeyboardLayout keyboardLayout;

    if (!resourceDataAccess.loadKeyboardLayout(path, &keyboardLayout))
        return false;

    DataIndexKeyboardLayout* overwriteDataIndexKeyboardLayout(0);

    for (int i = 0; i < m_dataIndex->keyboardLayoutCount(); i++)
    {
        DataIndexKeyboardLayout* const testKeyboardLayout = m_dataIndex->keyboardLayout(i);

        if (testKeyboardLayout->source() == DataIndex::BuiltInResource &&  testKeyboardLayout->id() == keyboardLayout.id())
        {
            switch (KMessageBox::questionYesNo(this, i18n("The selected keyboard layout is already present as a built-in keyboard layout."), QString(),
                                               KGuiItem(i18n("Import as new keyboard layout"), QStringLiteral("dialog-ok")),
                                               KStandardGuiItem::cancel()
            ))
            {
                case KMessageBox::Yes:
                    keyboardLayout.setId(QUuid::createUuid().toString());
                    break;
                default:
                    return true;
            }
        }

        if (testKeyboardLayout->source() == DataIndex::UserResource &&  testKeyboardLayout->id() == keyboardLayout.id())
        {
            switch (KMessageBox::questionYesNoCancel(this, i18n("The selected keyboard layout is already present as a user keyboard layout."), QString(),
                                               KGuiItem(i18n("Import as new keyboard layout"), QStringLiteral("dialog-ok")),
                                               KStandardGuiItem::overwrite(),
                                               KStandardGuiItem::cancel()
            ))
            {
                case KMessageBox::Yes:
                    keyboardLayout.setId(QUuid::createUuid().toString());
                    break;
                case KMessageBox::No:
                    overwriteDataIndexKeyboardLayout = testKeyboardLayout;
                    break;
                default:
                    return true;
            }
        }
    }

    if (Resource* dataIndexResource = storeResource(&keyboardLayout, overwriteDataIndexKeyboardLayout))
    {
        m_editorWidget->clearUndoStackForResource(dataIndexResource);
        selectDataResource(dataIndexResource);
    }

    return true;
}

