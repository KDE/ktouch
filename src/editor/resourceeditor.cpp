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
#include <QListView>

#include <KToolBar>
#include <KAction>
#include <KActionCollection>
#include <KStandardAction>
#include <KIcon>
#include <KLocale>

ResourceEditor::ResourceEditor(QWidget *parent) :
    KMainWindow(parent),
    m_actionCollection(new KActionCollection(this)),
    m_newResourceAction(KStandardAction::openNew(this, SLOT(newResource()), m_actionCollection)),
    m_deleteResourceAction(new KAction(KIcon("edit-delete"), i18n("Delete"), this)),
    m_undoAction(KStandardAction::undo(this, SLOT(undo()), m_actionCollection)),
    m_redoAction(KStandardAction::redo(this, SLOT(redo()), m_actionCollection)),
    m_importResourceAction(new KAction(KIcon("document-import"), i18n("Import"), this)),
    m_exportResourceAction(new KAction(KIcon("document-export"), i18n("Export"), this))

{
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

    QListView* resourceListView = new QListView(splitter);
    resourceListView->setMinimumWidth(200);
    QWidget* placeHolder = new QWidget(splitter);
    placeHolder->setMinimumWidth(400);
    placeHolder->setMinimumHeight(500);

    splitter->addWidget(resourceListView);
    splitter->addWidget(placeHolder);
    splitter->setChildrenCollapsible(false);
    splitter->adjustSize();

    setCentralWidget(splitter);
}

void ResourceEditor::newResource()
{
}

void ResourceEditor::deleteResource()
{
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
