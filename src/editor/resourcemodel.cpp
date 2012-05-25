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

#include <KLocale>
#include <KCategorizedSortFilterProxyModel>

#include "../core/dataindex.h"

ResourceModel::ResourceModel(DataIndex* dataIndex, QObject* parent) :
    QAbstractListModel(parent),
    m_dataIndex(dataIndex)
{
}

DataIndex* ResourceModel::dataIndex() const
{
    return m_dataIndex;
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

QVariant ResourceModel::courseData(int row, int role) const
{
    if (role == KCategorizedSortFilterProxyModel::CategorySortRole)
        return QVariant(0);
    if (role == KCategorizedSortFilterProxyModel::CategoryDisplayRole)
        return QVariant(i18n("Courses"));
    if (role == Qt::DisplayRole)
        return QVariant(m_dataIndex->course(row)->title());
    if (role == ResourceModel::ResourceTypeRole)
        return QVariant(ResourceModel::CourseItem);
    if (role == ResourceModel::DataRole)
        return QVariant::fromValue<QObject*>(m_dataIndex->course(row));
    if (role == ResourceModel::IndexRole)
        return QVariant(row);
    return QVariant();
}

QVariant ResourceModel::keyboardLayoutData(int row, int role) const
{
    if (role == KCategorizedSortFilterProxyModel::CategorySortRole)
        return QVariant(1);
    if (role == KCategorizedSortFilterProxyModel::CategoryDisplayRole)
        return QVariant(i18n("Keyboard layouts"));
    if (role == Qt::DisplayRole)
        return QVariant(m_dataIndex->keyboardLayout(row)->title());
    if (role == ResourceModel::ResourceTypeRole)
        return QVariant(ResourceModel::KeyboardLayoutItem);
    if (role == ResourceModel::DataRole)
        return QVariant::fromValue<QObject*>(m_dataIndex->keyboardLayout(row));
    if (role == ResourceModel::IndexRole)
        return QVariant(row);
    return QVariant();
}
