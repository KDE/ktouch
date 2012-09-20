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


#ifndef CATEGORIZEDRESOURCESORTFILTERPROXYMODEL_H
#define CATEGORIZEDRESOURCESORTFILTERPROXYMODEL_H

#include "KCategorizedSortFilterProxyModel"

#include "models/resourcemodel.h"

class CategorizedResourceSortFilterProxyModel : public KCategorizedSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(ResourceModel::ResourceItemTypes resourceTypeFilter READ resourceTypeFilter WRITE setResourceTypeFilter NOTIFY resourceTypeFilterChanged)
    Q_PROPERTY(QString keyboardLayoutNameFilter READ keyboardLayoutNameFilter WRITE setKeyboardLayoutNameFilter NOTIFY keyboardLayoutNameFilterChanged)
    Q_PROPERTY(ResourceModel* resourceModel READ resourceModel WRITE setResourceModel NOTIFY resourceModelChanged)
public:
    explicit CategorizedResourceSortFilterProxyModel(QObject* parent = 0);
    ResourceModel::ResourceItemTypes resourceTypeFilter() const;
    void setResourceTypeFilter(ResourceModel::ResourceItemTypes types);
    QString keyboardLayoutNameFilter() const;
    void setKeyboardLayoutNameFilter(const QString& name);
    ResourceModel* resourceModel() const;
    void setResourceModel(ResourceModel* resourceModel);
signals:
    void resourceTypeFilterChanged();
    void keyboardLayoutNameFilterChanged();
    void resourceModelChanged();
protected:
    bool subSortLessThan(const QModelIndex& left, const QModelIndex& right) const;
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;
private:
    ResourceModel::ResourceItemTypes m_resourceTypeFilter;
    QString m_keyboardLayoutNameFilter;
    ResourceModel* m_resourceModel;
};

#endif // CATEGORIZEDRESOURCESORTFILTERPROXYMODEL_H
