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

#include "resourcemodel.h"

#include <QSignalMapper>

#include <KLocale>
#include <KIcon>
#include <KCategorizedSortFilterProxyModel>

ResourceModel::ResourceModel(QObject* parent) :
    QAbstractListModel(parent),
    m_dataIndex(0),
    m_signalMapper(new QSignalMapper(this))
{
    connect(m_signalMapper, SIGNAL(mapped(int)), SLOT(emitDataChanged(int)));
    QHash<int,QByteArray> roleNames = this->roleNames();
    roleNames.insert(ResourceModel::KeyboardLayoutNameRole, "keyboardLayoutName");
    roleNames.insert(ResourceModel::PathRole, "path");
    roleNames.insert(ResourceModel::DataRole, "dataRole");
    setRoleNames(roleNames);
}

DataIndex* ResourceModel::dataIndex() const
{
    return m_dataIndex;
}

void ResourceModel::setDataIndex(DataIndex* dataIndex)
{
    if (dataIndex != m_dataIndex)
    {
        beginResetModel();

        if (m_dataIndex)
        {
            m_dataIndex->disconnect(this);
        }

        m_dataIndex = dataIndex;

        if (m_dataIndex)
        {
            connect(m_dataIndex, SIGNAL(courseAboutToBeAdded(DataIndexCourse*,int)), SLOT(onCourseAboutToBeAdded(DataIndexCourse*,int)));
            connect(m_dataIndex, SIGNAL(courseAdded()), SLOT(onResourceAdded()));
            connect(m_dataIndex, SIGNAL(coursesAboutToBeRemoved(int,int)), SLOT(onCoursesAboutToBeRemoved(int,int)));
            connect(m_dataIndex, SIGNAL(coursesRemoved()), SLOT(onResourceRemoved()));
            connect(m_dataIndex, SIGNAL(keyboardLayoutAboutToBeAdded(DataIndexKeyboardLayout*,int)), SLOT(onKeyboardLayoutAboutToBeAdded(DataIndexKeyboardLayout*,int)));
            connect(m_dataIndex, SIGNAL(keyboardLayoutAdded()), SLOT(onResourceAdded()));
            connect(m_dataIndex, SIGNAL(keyboardLayoutsAboutToBeRemoved(int,int)), SLOT(onKeyboardLayoutsAboutToBeRemoved(int,int)));
            connect(m_dataIndex, SIGNAL(keyboardLayoutsRemoved()), SLOT(onResourceRemoved()));
        }

        for (int i = 0; i < dataIndex->courseCount(); i++)
        {
            connectToCourse(dataIndex->course(i));
        }

        for (int i = 0; i < dataIndex->keyboardLayoutCount(); i++)
        {
            connectToKeyboardLayout(dataIndex->keyboardLayout(i));
        }

        updateMappings();

        endResetModel();

        emit dataIndexChanged();
    }
}

Qt::ItemFlags ResourceModel::flags(const QModelIndex& index) const
{
    return QAbstractListModel::flags(index);
}

QVariant ResourceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() > m_dataIndex->courseCount() + m_dataIndex->keyboardLayoutCount())
        return QVariant();

    if (index.row() < m_dataIndex->courseCount())
        return courseData(index.row(), role);

    return keyboardLayoutData(index.row() - m_dataIndex->courseCount(), role);
}

QVariant ResourceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Vertical)
        return QVariant(section + 1);
    return QVariant(i18n("Title"));
}

int ResourceModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_dataIndex->courseCount() + m_dataIndex->keyboardLayoutCount();
}

void ResourceModel::onCourseAboutToBeAdded(DataIndexCourse* course, int index)
{
    connectToCourse(course);
    beginInsertRows(QModelIndex(), index, index);
}

void ResourceModel::onCoursesAboutToBeRemoved(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
}

void ResourceModel::onKeyboardLayoutAboutToBeAdded(DataIndexKeyboardLayout* keyboardLayout, int index)
{
    const int offset = m_dataIndex->courseCount();
    connectToKeyboardLayout(keyboardLayout);
    beginInsertRows(QModelIndex(), index + offset, index + offset);
}

void ResourceModel::onKeyboardLayoutsAboutToBeRemoved(int first, int last)
{
    const int offset = m_dataIndex->courseCount();
    beginRemoveRows(QModelIndex(), first + offset, last + offset);
}

void ResourceModel::onResourceAdded()
{
    updateMappings();
    endInsertRows();
}

void ResourceModel::onResourceRemoved()
{
    updateMappings();
    endRemoveRows();
}

void ResourceModel::emitDataChanged(int row)
{
    const QModelIndex modelIndex = index(row);
    emit dataChanged(modelIndex, modelIndex);
}

QVariant ResourceModel::courseData(int row, int role) const
{
    switch(role)
    {
    case KCategorizedSortFilterProxyModel::CategorySortRole:
        return QVariant(0);
    case KCategorizedSortFilterProxyModel::CategoryDisplayRole:
        return QVariant(i18n("Courses"));
    case Qt::DisplayRole:
        return QVariant(m_dataIndex->course(row)->title());
    case Qt::ToolTipRole:
        return QVariant(m_dataIndex->course(row)->description());
    case Qt::DecorationRole:
        return QVariant(resourceIcon(m_dataIndex->course(row)->source()));
    case ResourceModel::ResourceTypeRole:
        return QVariant(ResourceModel::CourseItem);
    case ResourceModel::DataRole:
        return QVariant::fromValue<QObject*>(m_dataIndex->course(row));
    case ResourceModel::KeyboardLayoutNameRole:
        return QVariant(m_dataIndex->course(row)->keyboardLayoutName());
    case ResourceModel::PathRole:
        return QVariant(m_dataIndex->course(row)->path());
    case ResourceModel::IndexRole:
        return QVariant(row);
    case ResourceModel::SourceRole:
        return QVariant(m_dataIndex->course(row)->source());
    default:
        return QVariant();
    }
}

QVariant ResourceModel::keyboardLayoutData(int row, int role) const
{
    switch(role)
    {
    case KCategorizedSortFilterProxyModel::CategorySortRole:
        return QVariant(1);
    case KCategorizedSortFilterProxyModel::CategoryDisplayRole:
        return QVariant(i18n("Keyboard Layouts"));
    case Qt::DisplayRole:
        return QVariant(m_dataIndex->keyboardLayout(row)->title());
    case Qt::ToolTipRole:
        return QVariant(i18n("Name: %1", m_dataIndex->keyboardLayout(row)->name()));
    case Qt::DecorationRole:
        return QVariant(resourceIcon(m_dataIndex->keyboardLayout(row)->source()));
    case ResourceModel::ResourceTypeRole:
        return QVariant(ResourceModel::KeyboardLayoutItem);
    case ResourceModel::DataRole:
        return QVariant::fromValue<QObject*>(m_dataIndex->keyboardLayout(row));
    case ResourceModel::KeyboardLayoutNameRole:
        return QVariant(m_dataIndex->keyboardLayout(row)->name());
    case ResourceModel::PathRole:
        return QVariant(m_dataIndex->keyboardLayout(row)->path());
    case ResourceModel::IndexRole:
        return QVariant(row);
    case ResourceModel::SourceRole:
        return QVariant(m_dataIndex->keyboardLayout(row)->source());
    default:
        return QVariant();
    }
}

void ResourceModel::connectToCourse(DataIndexCourse* course)
{
    connect(course, SIGNAL(titleChanged()), m_signalMapper, SLOT(map()));
    connect(course, SIGNAL(descriptionChanged()), m_signalMapper, SLOT(map()));
    connect(course, SIGNAL(keyboardLayoutNameChanged()), m_signalMapper, SLOT(map()));
    connect(course, SIGNAL(pathChanged()), m_signalMapper, SLOT(map()));
    connect(course, SIGNAL(sourceChanged()), m_signalMapper, SLOT(map()));
}

void ResourceModel::connectToKeyboardLayout(DataIndexKeyboardLayout *keyboardLayout)
{
    connect(keyboardLayout, SIGNAL(titleChanged()), m_signalMapper, SLOT(map()));
    connect(keyboardLayout, SIGNAL(nameChanged()), m_signalMapper, SLOT(map()));
    connect(keyboardLayout, SIGNAL(pathChanged()), m_signalMapper, SLOT(map()));
    connect(keyboardLayout, SIGNAL(sourceChanged()), m_signalMapper, SLOT(map()));
}

void ResourceModel::updateMappings()
{
    for (int i = 0; i < m_dataIndex->courseCount(); i++)
    {
        m_signalMapper->setMapping(m_dataIndex->course(i), i);
    }

    const int offset = m_dataIndex->courseCount();

    for (int i = 0; i < m_dataIndex->keyboardLayoutCount(); i++)
    {
        m_signalMapper->setMapping(m_dataIndex->keyboardLayout(i), i + offset);
    }
}

QIcon ResourceModel::resourceIcon(DataIndex::Source source) const
{
    static QIcon systemIcon = KIcon("computer");
    static QIcon userIcon = KIcon("user-identity");
    return source == DataIndex::BuiltInResource? systemIcon: userIcon;
}
