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

#include "resourceeditorwidget.h"

#include <KCategoryDrawer>
#include <KAction>
#include <KStandardAction>

#include "core/dataindex.h"

ResourceEditorWidget::ResourceEditorWidget(QWidget* parent) :
    QWidget(parent),
    Ui::ResourceEditorWidget(),
    m_undeleteAction(new KAction(KIcon("edit-undo"), i18n("Restore"), this)),
    m_clearMsgAction(new KAction(KIcon("window-close"), i18n("Dismiss"), this))
{
    setupUi(this);

    m_messageWidget->hide();
    m_messageWidget->setCloseButtonVisible(false);

    m_resourceView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_resourceView->setCategoryDrawer(new KCategoryDrawerV3(m_resourceView));
    m_resourceView->setMouseTracking(true);
    m_resourceView->setVerticalScrollMode(QListView::ScrollPerPixel);
    m_resourceView->setMinimumWidth(200);

    m_splitter->setStretchFactor(1, 1);

    connect(m_clearMsgAction, SIGNAL(triggered()), SLOT(clearMessage()));
    connect(m_undeleteAction, SIGNAL(triggered()), SLOT(requestResourceRestoration()));
}

void ResourceEditorWidget::setResourceModel(ResourceModel* model)
{
    m_courseEditor->setResourceModel(model);
}

void ResourceEditorWidget::setUndoGroup(QUndoGroup* undoGroup)
{
    m_courseEditor->setUndoGroup(undoGroup);
    m_keyboardLayoutEditor->setUndoGroup(undoGroup);
}

void ResourceEditorWidget::showMessage(ResourceEditorWidget::MessageType type, const QString& msg)
{
    m_messageWidget->removeAction(m_undeleteAction);
    m_messageWidget->removeAction(m_clearMsgAction);

    switch (type)
    {
    case ResourceEditorWidget::ResourceDeletedMsg:
        m_messageWidget->setMessageType(KMessageWidget::Positive);
        m_messageWidget->addAction(m_undeleteAction);
        m_messageWidget->addAction(m_clearMsgAction);
        break;
    }

    m_currentMessageType = type;

    m_messageWidget->setText(msg);
    m_messageWidget->animatedShow();
}

QAbstractItemView* ResourceEditorWidget::resourceView() const
{
    return m_resourceView;
}

void ResourceEditorWidget::openResource(Resource* dataIndexResource)
{
    if (DataIndexCourse* course = qobject_cast<DataIndexCourse*>(dataIndexResource))
    {
        m_editorStack->setCurrentWidget(m_courseEditor);
        m_courseEditor->openCourse(course);
    }
    else if (DataIndexKeyboardLayout* keyboardLayout = qobject_cast<DataIndexKeyboardLayout*>(dataIndexResource))
    {
        m_editorStack->setCurrentWidget(m_keyboardLayoutEditor);
        m_keyboardLayoutEditor->openKeyboardLayout(keyboardLayout);
    }
}

void ResourceEditorWidget::clearUndoStackForResource(Resource* dataIndexResource)
{
    if (DataIndexCourse* course = qobject_cast<DataIndexCourse*>(dataIndexResource))
    {
        m_courseEditor->clearUndoStackForCourse(course);
    }
    else if (DataIndexKeyboardLayout* keyboardLayout = qobject_cast<DataIndexKeyboardLayout*>(dataIndexResource))
    {
        m_keyboardLayoutEditor->clearUndoStackForKeyboardLayout(keyboardLayout);
    }
}

void ResourceEditorWidget::save()
{
    if (m_editorStack->currentWidget() == m_courseEditor)
    {
        m_courseEditor->save();
    }
    else if (m_editorStack->currentWidget() == m_keyboardLayoutEditor)
    {
        m_keyboardLayoutEditor->save();
    }
}

void ResourceEditorWidget::requestResourceRestoration()
{
    emit(resourceRestorationRequested());
    m_messageWidget->animatedHide();
}

void ResourceEditorWidget::clearMessage()
{
    if (m_currentMessageType == ResourceEditorWidget::ResourceDeletedMsg)
    {
        emit(resourceRestorationDismissed());
    }

    m_messageWidget->animatedHide();
}
