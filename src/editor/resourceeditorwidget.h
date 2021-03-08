/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */


#ifndef RESOURCEEDITORWIDGET_H
#define RESOURCEEDITORWIDGET_H

#include <QWidget>
#include "ui_resourceeditorwidget.h"

class QAbstractItemView;
class QAction;
class QUndoGroup;
class Resource;
class ResourceModel;

class ResourceEditorWidget : public QWidget, private Ui::ResourceEditorWidget
{
    Q_OBJECT
public:
    enum MessageType
    {
        ResourceDeletedMsg
    };
    explicit ResourceEditorWidget(QWidget* parent = 0);
    void setResourceModel(ResourceModel* model);
    void setUndoGroup(QUndoGroup* undoGroup);
    void showMessage(MessageType type, const QString& msg);
    QAbstractItemView* resourceView() const;
    void openResource(Resource* dataIndexResource);
    void clearUndoStackForResource(Resource* dataIndexResource);
    void save();
signals:
    void resourceRestorationRequested();
    void resourceRestorationDismissed();
private slots:
    void requestResourceRestoration();
    void clearMessage();
private:
    QAction* m_undeleteAction;
    QAction* m_clearMsgAction;
    MessageType m_currentMessageType;
};

#endif // RESOURCEEDITORWIDGET_H
