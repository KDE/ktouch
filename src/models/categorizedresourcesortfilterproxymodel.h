/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
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
    Q_PROPERTY(bool invertedKeyboardLayoutNameFilter READ invertedKeyboardLayoutNameFilter WRITE setInvertedKeyboardLayoutNameFilter NOTIFY invertedKeyboardLayoutNameFilterChanged)
    Q_PROPERTY(ResourceModel* resourceModel READ resourceModel WRITE setResourceModel NOTIFY resourceModelChanged)
public:
    explicit CategorizedResourceSortFilterProxyModel(QObject* parent = nullptr);
    ResourceModel::ResourceItemTypes resourceTypeFilter() const;
    void setResourceTypeFilter(ResourceModel::ResourceItemTypes types);
    QString keyboardLayoutNameFilter() const;
    void setKeyboardLayoutNameFilter(const QString& name);
    bool invertedKeyboardLayoutNameFilter() const;
    void setInvertedKeyboardLayoutNameFilter(bool inverted);
    ResourceModel* resourceModel() const;
    void setResourceModel(ResourceModel* resourceModel);
signals:
    void resourceTypeFilterChanged();
    void keyboardLayoutNameFilterChanged();
    void invertedKeyboardLayoutNameFilterChanged();
    void resourceModelChanged();
protected:
    bool subSortLessThan(const QModelIndex& left, const QModelIndex& right) const override;
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;
private:
    ResourceModel::ResourceItemTypes m_resourceTypeFilter;
    QString m_keyboardLayoutNameFilter;
    bool m_invertedKeyboardLayoutNameFilter;
    ResourceModel* m_resourceModel;
};

#endif // CATEGORIZEDRESOURCESORTFILTERPROXYMODEL_H
