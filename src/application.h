/*
 * SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QPointer>

#include "editor/resourceeditor.h"

class QQmlEngine;
class DataIndex;

class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int& argc, char** argv, int flags = ApplicationFlags);
    static DataIndex* dataIndex();
    static void setupDeclarativeBindings(QQmlEngine* qmlEngine);
    static QPointer<ResourceEditor>& resourceEditorRef();
    QStringList& qmlImportPaths();
private:
    void registerQmlTypes();
    void migrateKde4Files();
    DataIndex* m_dataIndex;
    QPointer<ResourceEditor> m_resourceEditorRef;
    QStringList m_qmlImportPaths;
};

#endif // APPLICATION_H
