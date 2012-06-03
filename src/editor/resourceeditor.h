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

#ifndef EDITORMAINWINDOW_H
#define EDITORMAINWINDOW_H

#include <KMainWindow>

class KAction;
class KActionCollection;
class KCategorizedView;
class DataIndex;
class ResourceModel;
class Resource;
class ResourceEditorWidget;

class ResourceEditor : public KMainWindow
{
    Q_OBJECT
public:
    explicit ResourceEditor(QWidget* parent = 0);
    virtual ~ResourceEditor();

private slots:
    void newResource();
    void deleteResource();
    void undo();
    void redo();
    void importResource();
    void exportResource();
    void onResourceSelected();
    void restoreResourceBackup();
    void clearResourceBackup();

private:
    void prepareResourceRestore(Resource* backup);
    void addResource(Resource* resource);
    DataIndex* m_dataIndex;
    ResourceModel* m_resourceModel;
    Resource* m_currentResource;
    Resource* m_backupResource;
    KActionCollection* m_actionCollection;
    KAction* m_newResourceAction;
    KAction* m_deleteResourceAction;
    KAction* m_undoAction;
    KAction* m_redoAction;
    KAction* m_importResourceAction;
    KAction* m_exportResourceAction;
    ResourceEditorWidget* m_editorWidget;
};

#endif // EDITORMAINWINDOW_H
