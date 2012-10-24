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


#ifndef RESOURCEEDITORWIDGET_H
#define RESOURCEEDITORWIDGET_H

#include <QWidget>
#include "ui_resourceeditorwidget.h"

class QAbstractItemView;
class QUndoGroup;
class KAction;
class KMessageWidget;
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
    KAction* m_undeleteAction;
    KAction* m_clearMsgAction;
    MessageType m_currentMessageType;
};

#endif // RESOURCEEDITORWIDGET_H
