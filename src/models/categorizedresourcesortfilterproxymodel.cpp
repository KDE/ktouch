/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */


#include "categorizedresourcesortfilterproxymodel.h"

#include <QCollator>

CategorizedResourceSortFilterProxyModel::CategorizedResourceSortFilterProxyModel(QObject *parent) :
    KCategorizedSortFilterProxyModel(parent),
    m_resourceTypeFilter(ResourceModel::CourseItem | ResourceModel::KeyboardLayoutItem),
    m_invertedKeyboardLayoutNameFilter(false),
    m_resourceModel(0)
{
    setDynamicSortFilter(true);
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
        invalidate();
        sort(0);
        emit resourceTypeFilterChanged();
    }
}

QString CategorizedResourceSortFilterProxyModel::keyboardLayoutNameFilter() const
{
    return m_keyboardLayoutNameFilter;
}

void CategorizedResourceSortFilterProxyModel::setKeyboardLayoutNameFilter(const QString &name)
{
    if (name != m_keyboardLayoutNameFilter)
    {
        m_keyboardLayoutNameFilter = name;
        invalidateFilter();
        invalidate();
        sort(0);
        emit keyboardLayoutNameFilterChanged();
    }
}

bool CategorizedResourceSortFilterProxyModel::invertedKeyboardLayoutNameFilter() const
{
    return m_invertedKeyboardLayoutNameFilter;
}

void CategorizedResourceSortFilterProxyModel::setInvertedKeyboardLayoutNameFilter(bool inverted)
{
    if (inverted != m_invertedKeyboardLayoutNameFilter)
    {
        m_invertedKeyboardLayoutNameFilter = inverted;
        invalidateFilter();
        invalidate();
        sort(0);
        emit invertedKeyboardLayoutNameFilterChanged();
    }
}


ResourceModel* CategorizedResourceSortFilterProxyModel::resourceModel() const
{
    return m_resourceModel;
}

void CategorizedResourceSortFilterProxyModel::setResourceModel(ResourceModel* resourceModel)
{
    if (resourceModel != m_resourceModel)
    {
        m_resourceModel = resourceModel;
        setSourceModel(m_resourceModel);
        sort(0);
        emit resourceModelChanged();
    }
}

bool CategorizedResourceSortFilterProxyModel::subSortLessThan(const QModelIndex& left, const QModelIndex& right) const
{
    const QString leftStr = sourceModel()->data(left, Qt::DisplayRole).toString();
    const QString rightStr = sourceModel()->data(right, Qt::DisplayRole).toString();
    QCollator locater;
    locater.setCaseSensitivity(Qt::CaseInsensitive);

    const int difference = locater.compare(leftStr, rightStr);

    if (difference == 0)
    {
        return left.row() < right.row();
    }

    return  difference < 0;
}

bool CategorizedResourceSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    const QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    const int resourceType = sourceModel()->data(index, ResourceModel::ResourceTypeRole).toInt();

    if ((m_resourceTypeFilter & resourceType) == 0)
        return false;

    if (m_keyboardLayoutNameFilter.isEmpty())
        return true;

    const QString name = sourceModel()->data(index, ResourceModel::KeyboardLayoutNameRole).toString();

    return m_invertedKeyboardLayoutNameFilter ^ (name == m_keyboardLayoutNameFilter);
}
