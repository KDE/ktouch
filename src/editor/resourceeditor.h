/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef RESOURCEEDITOR_H
#define RESOURCEEDITOR_H

#include <KMainWindow>

class QUndoGroup;
class QTimer;
class KAction;
class KActionCollection;
class CategorizedResourceSortFilterProxyModel;
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

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void newResource();
    void deleteResource();
    void importResource();
    void exportResource();
    void onResourceSelected();
    void restoreResourceBackup();
    void clearResourceBackup();
    void save();
    void setUndoText(const QString& text);
    void setRedoText(const QString& text);

private:
    void prepareResourceRestore(Resource* backup);
    Resource* storeResource(Resource* resource, Resource* dataIndexResource = 0);
    void selectDataResource(Resource* resource);
    void selectFirstResource();
    bool importCourse(const QString& path);
    bool importKeyboardLayout(const QString& path);
    DataIndex* m_dataIndex;
    ResourceModel* m_resourceModel;
    CategorizedResourceSortFilterProxyModel* m_categorizedResourceModel;
    Resource* m_currentResource;
    Resource* m_backupResource;
    QUndoGroup* m_undoGroup;
    KActionCollection* m_actionCollection;
    QAction* m_newResourceAction;
    QAction* m_deleteResourceAction;
    QAction* m_undoAction;
    QAction* m_redoAction;
    QAction* m_importResourceAction;
    QAction* m_exportResourceAction;
    ResourceEditorWidget* m_editorWidget;
    QTimer* m_saveTimer;
};

#endif // RESOURCEEDITOR_H
