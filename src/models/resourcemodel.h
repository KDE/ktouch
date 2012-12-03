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

#ifndef RESOURCEMODEL_H
#define RESOURCEMODEL_H

#include <QAbstractListModel>

#include "core/dataindex.h"

class QIcon;
class QSignalMapper;
class DataIndex;

class ResourceModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(DataIndex* dataIndex READ dataIndex WRITE setDataIndex NOTIFY dataIndexChanged)
    Q_ENUMS(ResourceItemTypes)
public:
    enum ResourceItemType {
        None = 0x0,
        CourseItem = 0x1,
        KeyboardLayoutItem = 0x2
    };
    Q_DECLARE_FLAGS(ResourceItemTypes, ResourceItemType)
    enum AdditionalRoles {
        ResourceTypeRole = Qt::UserRole + 1,
        DataRole,
        KeyboardLayoutNameRole,
        PathRole,
        IndexRole,
        SourceRole
    };
    explicit ResourceModel( QObject* parent = 0);
    DataIndex* dataIndex() const;
    void setDataIndex(DataIndex* dataIndex);
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent) const;

signals:
    void dataIndexChanged();

private slots:
    void onCourseAboutToBeAdded(DataIndexCourse* course, int index);
    void onCoursesAboutToBeRemoved(int first, int last);
    void onKeyboardLayoutAboutToBeAdded(DataIndexKeyboardLayout* keyboardLayout, int index);
    void onKeyboardLayoutsAboutToBeRemoved(int first, int last);
    void onResourceAdded();
    void onResourceRemoved();
    void emitDataChanged(int row);

private:
    QVariant courseData(int row, int role) const;
    QVariant keyboardLayoutData(int row, int role) const;
    void connectToCourse(DataIndexCourse* course);
    void connectToKeyboardLayout(DataIndexKeyboardLayout* keyboardLayout);
    void updateMappings();
    QIcon resourceIcon(DataIndex::Source source) const;
    DataIndex* m_dataIndex;
    QSignalMapper* m_signalMapper;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ResourceModel::ResourceItemTypes)

#endif // RESOURCEMODEL_H
