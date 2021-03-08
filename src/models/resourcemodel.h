/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef RESOURCEMODEL_H
#define RESOURCEMODEL_H

#include <QAbstractListModel>

#include "core/dataindex.h"

class QIcon;
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
    Q_ENUM(AdditionalRoles)
    explicit ResourceModel( QObject* parent = nullptr);
    DataIndex* dataIndex() const;
    void setDataIndex(DataIndex* dataIndex);
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
    QHash<int, QByteArray> roleNames() const override;

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
    void connectToCourse(DataIndexCourse* course, int index);
    void connectToKeyboardLayout(DataIndexKeyboardLayout* keyboardLayout, int index);
    void updateMappings();
    QIcon resourceIcon(DataIndex::Source source) const;
    DataIndex* m_dataIndex;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ResourceModel::ResourceItemTypes)

#endif // RESOURCEMODEL_H
