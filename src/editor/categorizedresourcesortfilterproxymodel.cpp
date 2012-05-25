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


#include "categorizedresourcesortfilterproxymodel.h"

#include "KStringHandler"

CategorizedResourceSortFilterProxyModel::CategorizedResourceSortFilterProxyModel(QObject *parent) :
    KCategorizedSortFilterProxyModel(parent),
    m_resourceTypeFilter(ResourceModel::CourseItem | ResourceModel::KeyboardLayoutItem)
{
}

ResourceModel::ResourceItemTypes CategorizedResourceSortFilterProxyModel::resourceTypeFilter() const
{
    return m_resourceTypeFilter;
}

void CategorizedResourceSortFilterProxyModel::setResourceTypeFilter(ResourceModel::ResourceItemTypes types)
{
    if (types != m_resourceTypeFilter)
    {
        m_resourceTypeFilter = types;
        invalidateFilter();
    }
}

void CategorizedResourceSortFilterProxyModel::setSourceModel(QAbstractItemModel* sourceModel)
{
    QSortFilterProxyModel::setSourceModel(sourceModel);
    sort(0);
}

bool CategorizedResourceSortFilterProxyModel::subSortLessThan(const QModelIndex& left, const QModelIndex& right) const
{
    const QString leftStr = sourceModel()->data(left, Qt::DisplayRole).toString();
    const QString rightStr = sourceModel()->data(right, Qt::DisplayRole).toString();
    return KStringHandler::naturalCompare(leftStr, rightStr, Qt::CaseInsensitive) < 0;
}

bool CategorizedResourceSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    const QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    const int resourceType = sourceModel()->data(index, ResourceModel::ResourceTypeRole).toInt();
    return m_resourceTypeFilter & resourceType;
}
