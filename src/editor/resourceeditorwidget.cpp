/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "resourceeditorwidget.h"

#include <QAction>

#include <KCategoryDrawer>
#include <KLocalizedString>
#include <KStandardAction>

#include "core/dataindex.h"

ResourceEditorWidget::ResourceEditorWidget(QWidget* parent) :
    QWidget(parent),
    Ui::ResourceEditorWidget(),
    m_undeleteAction(new QAction(QIcon::fromTheme(QStringLiteral("edit-undo")), i18n("Restore"), this)),
    m_clearMsgAction(new QAction(QIcon::fromTheme(QStringLiteral("window-close")), i18n("Dismiss"), this))
{
    setupUi(this);

    m_messageWidget->hide();
    m_messageWidget->setCloseButtonVisible(false);

    m_resourceView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_resourceView->setCategoryDrawer(new KCategoryDrawer(m_resourceView));
    m_resourceView->setMouseTracking(true);
    m_resourceView->setVerticalScrollMode(QListView::ScrollPerPixel);
    m_resourceView->setMinimumWidth(200);

    m_splitter->setStretchFactor(1, 1);

    connect(m_clearMsgAction, &QAction::triggered, this, &ResourceEditorWidget::clearMessage);
    connect(m_undeleteAction, &QAction::triggered, this, &ResourceEditorWidget::requestResourceRestoration);
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
