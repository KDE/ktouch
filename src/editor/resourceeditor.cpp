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

#include "resourceeditor.h"

#include <QSplitter>
#include <QTableView>
#include <QUuid>

#include <KToolBar>
#include <KAction>
#include <KActionCollection>
#include <KStandardAction>
#include <KIcon>
#include <KLocale>
#include <KCategorizedSortFilterProxyModel>
#include <KCategorizedView>
#include <KCategoryDrawer>
#include <KMessageBox>

#include "core/dataindex.h"
#include "core/dataaccess.h"
#include "core/resource.h"
#include "core/course.h"
#include "core/keyboardlayout.h"
#include "resourcemodel.h"
#include "categorizedresourcesortfilterproxymodel.h"
#include "newresourceassistant.h"

ResourceEditor::ResourceEditor(QWidget *parent) :
    KMainWindow(parent),
    m_dataIndex(new DataIndex(this)),
    m_resourceModel(new ResourceModel(m_dataIndex, this)),
    m_currentResource(0),
    m_actionCollection(new KActionCollection(this)),
    m_newResourceAction(KStandardAction::openNew(this, SLOT(newResource()), m_actionCollection)),
    m_deleteResourceAction(new KAction(KIcon("edit-delete"), i18n("Delete"), this)),
    m_undoAction(KStandardAction::undo(this, SLOT(undo()), m_actionCollection)),
    m_redoAction(KStandardAction::redo(this, SLOT(redo()), m_actionCollection)),
    m_importResourceAction(new KAction(KIcon("document-import"), i18n("Import"), this)),
    m_exportResourceAction(new KAction(KIcon("document-export"), i18n("Export"), this))

{
    DataAccess dataAccess;
    dataAccess.loadDataIndex(m_dataIndex);
    CategorizedResourceSortFilterProxyModel* categorizedResourceModel = new CategorizedResourceSortFilterProxyModel(this);
    categorizedResourceModel->setSourceModel(m_resourceModel);
    categorizedResourceModel->setCategorizedModel(true);

    setMinimumSize(700, 500);
    setCaption(i18n("Course and Keyboard Layout Editor"));

    m_deleteResourceAction->setEnabled(false);
    m_actionCollection->addAction("deleteResource", m_deleteResourceAction);
    connect(m_deleteResourceAction, SIGNAL(triggered()), SLOT(deleteResource()));
    m_undoAction->setEnabled(false);
    m_redoAction->setEnabled(false);
    m_actionCollection->addAction("importResource", m_importResourceAction);
    connect(m_importResourceAction, SIGNAL(triggered()), SLOT(importResource()));
    m_actionCollection->addAction("exportResource", m_exportResourceAction);
    connect(m_exportResourceAction, SIGNAL(triggered()), SLOT(exportResource()));
    m_exportResourceAction->setEnabled(false);

    toolBar()->addAction(m_newResourceAction);
    toolBar()->addAction(m_deleteResourceAction);
    toolBar()->addSeparator();
    toolBar()->addAction(m_undoAction);
    toolBar()->addAction(m_redoAction);
    toolBar()->addSeparator();
    toolBar()->addAction(m_importResourceAction);
    toolBar()->addAction(m_exportResourceAction);

    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    m_resourceView = new KCategorizedView(splitter);
    m_resourceView->setCategoryDrawer(new KCategoryDrawerV3(m_resourceView));
    m_resourceView->setMouseTracking(true);
    m_resourceView->setVerticalScrollMode(QListView::ScrollPerPixel);
    m_resourceView->setModel(categorizedResourceModel);
    m_resourceView->setMinimumWidth(200);
    connect(m_resourceView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), SLOT(onResourceSelected()));

    QWidget* placeHolder = new QWidget(splitter);
    placeHolder->setMinimumWidth(400);
    placeHolder->setMinimumHeight(500);

    splitter->addWidget(m_resourceView);
    splitter->addWidget(placeHolder);
    splitter->setChildrenCollapsible(false);

    setCentralWidget(splitter);

    if (m_resourceView->model()->rowCount() > 0)
    {
        m_resourceView->selectionModel()->select(m_resourceView->model()->index(0, 0), QItemSelectionModel::SelectCurrent);
    }
}

void ResourceEditor::newResource()
{
    NewResourceAssistant assistant(m_resourceModel, this);
    DataAccess dataAccess;

    if (assistant.exec() == QDialog::Accepted)
    {
        Resource* resource = assistant.createResource();

        dataAccess.loadDataIndex(m_dataIndex);

        if (Course* course = qobject_cast<Course*>(resource))
        {
            const QString fileName = QString("%1.xml").arg(course->id());
            QString path = dataAccess.storeUserCourse(fileName, course);
            if (path.isNull())
            {
                KMessageBox::error(this, i18n("Error while saving course to disk."));
                return;
            }

            DataIndexCourse* dataIndexCourse = new DataIndexCourse();

            dataIndexCourse->setSource(DataIndex::UserResource);
            dataIndexCourse->setTitle(course->title());
            dataIndexCourse->setDescription(course->description());
            dataIndexCourse->setKeyboardLayoutName(course->keyboardLayoutName());
            dataIndexCourse->setPath(path);

            m_dataIndex->addCourse(dataIndexCourse);
            if (!dataAccess.storeDataIndex(m_dataIndex))
            {
                KMessageBox::error(this, i18n("Error while saving data index to disk."));
                return;
            }
        }
        else if (KeyboardLayout* keyboardLayout = qobject_cast<KeyboardLayout*>(resource))
        {
            const QString fileName = QString("%1.xml").arg(QUuid::createUuid());
            QString path = dataAccess.storeUserKeyboardLayout(fileName, keyboardLayout);
            if (path.isNull())
            {
                KMessageBox::error(this, i18n("Error while saving keyboard layout to disk."));
                return;
            }

            DataIndexKeyboardLayout* dataIndexKeyboardLayout = new DataIndexKeyboardLayout();

            dataIndexKeyboardLayout->setSource(DataIndex::UserResource);
            dataIndexKeyboardLayout->setName(keyboardLayout->name());
            dataIndexKeyboardLayout->setTitle(keyboardLayout->title());
            dataIndexKeyboardLayout->setPath(path);

            m_dataIndex->addKeyboardLayout(dataIndexKeyboardLayout);
            if (!dataAccess.storeDataIndex(m_dataIndex))
            {
                KMessageBox::error(this, i18n("Error while saving data index to disk."));
                return;
            }
        }
    }
}

void ResourceEditor::deleteResource()
{
    Q_ASSERT(m_currentResource);

    DataAccess dataAccess;

    if (DataIndexCourse* course = qobject_cast<DataIndexCourse*>(m_currentResource))
    {
        for (int i = 0; i < m_dataIndex->courseCount(); i++)
        {
            if (m_dataIndex->course(i) == course)
            {
                QFile file;
                file.setFileName(course->path());
                if (!file.remove())
                {
                    KMessageBox::error(this, i18n("Error while deleting course"));
                    return;
                }
                m_dataIndex->removeCourse(i);
                if (!dataAccess.storeDataIndex(m_dataIndex))
                {
                    KMessageBox::error(this, i18n("Error while saving data index to disk."));
                    return;
                }
            }
        }
    }
    else if (DataIndexKeyboardLayout* keyboardLayout = qobject_cast<DataIndexKeyboardLayout*>(m_currentResource))
    {
        for (int i = 0; i < m_dataIndex->keyboardLayoutCount(); i++)
        {
            if (m_dataIndex->keyboardLayout(i) == keyboardLayout)
            {
                QFile file;
                file.setFileName(keyboardLayout->path());
                if (!file.remove())
                {
                    KMessageBox::error(this, i18n("Error while deleting keyboard layout"));
                    return;
                }
                m_dataIndex->removeKeyboardLayout(i);
                if (!dataAccess.storeDataIndex(m_dataIndex))
                {
                    KMessageBox::error(this, i18n("Error while saving data index to disk."));
                    return;
                }
            }
        }
    }
}

void ResourceEditor::undo()
{
}

void ResourceEditor::redo()
{
}

void ResourceEditor::importResource()
{
}

void ResourceEditor::exportResource()
{
}

void ResourceEditor::onResourceSelected()
{
    const QModelIndex current = m_resourceView->selectionModel()->currentIndex();
    const bool valid = current.isValid() && m_resourceView->selectionModel()->hasSelection();

    if (valid)
    {
        const QVariant variant = m_resourceView->model()->data(current, ResourceModel::DataRole);
        QObject* const obj = variant.value<QObject*>();
        m_currentResource = qobject_cast<Resource*>(obj);
        const DataIndex::Source source = static_cast<DataIndex::Source>(m_resourceView->model()->data(current, ResourceModel::SourceRole).toInt());

        Q_ASSERT(m_currentResource);

        m_deleteResourceAction->setEnabled(source == DataIndex::UserResource);
        m_exportResourceAction->setEnabled(valid);
        // TODO update editor
    }
    else
    {
        m_currentResource = 0;
        m_deleteResourceAction->setEnabled(false);
        m_exportResourceAction->setEnabled(false);
    }
}
