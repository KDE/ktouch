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
