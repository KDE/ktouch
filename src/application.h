/*
 * Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QSharedPointer>

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
    static QSharedPointer<ResourceEditor>& resourceEditorRef();
    QStringList& qmlImportPaths();
private:
    void registerQmlTypes();
    void migrateKde4Files();
    DataIndex* m_dataIndex;
    QSharedPointer<ResourceEditor> m_resourceEditorRef;
    QStringList m_qmlImportPaths;
};

#endif // APPLICATION_H
