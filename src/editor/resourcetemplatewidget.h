/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */


#ifndef RESOURCETEMPLATEWIDGET_H
#define RESOURCETEMPLATEWIDGET_H

#include <QWidget>
#include "ui_resourcetemplatewidget.h"

#include "models/resourcemodel.h"

class Resource;
class CategorizedResourceSortFilterProxyModel;

class ResourceTemplateWidget : public QWidget, private Ui::ResourceTemplateWidget
{
    Q_OBJECT
public:
    explicit ResourceTemplateWidget(ResourceModel* resourceModel, QWidget* parent = 0);
    ResourceModel::ResourceItemType templateType() const;
    void setTemplateType(ResourceModel::ResourceItemType type);
    bool isValid() const;
    Resource* templateResource() const;
signals:
    void isValidChanged();
private:
    ResourceModel* m_resourceModel;
    ResourceModel::ResourceItemType m_templateType;
    CategorizedResourceSortFilterProxyModel* m_filteredResourcesModel;
};

#endif // RESOURCETEMPLATEWIDGET_H
