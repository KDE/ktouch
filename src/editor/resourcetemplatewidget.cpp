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


#include "resourcetemplatewidget.h"

#include "core/resource.h"
#include "models/categorizedresourcesortfilterproxymodel.h"

ResourceTemplateWidget::ResourceTemplateWidget(ResourceModel* resourceModel, QWidget* parent) :
    QWidget(parent),
    Ui::ResourceTemplateWidget(),
    m_resourceModel(resourceModel),
    m_templateType(ResourceModel::None),
    m_filteredResourcesModel(new CategorizedResourceSortFilterProxyModel(this))
{
    setupUi(this);

    m_filteredResourcesModel->setResourceModel(resourceModel);
    m_templatesView->setModel(m_filteredResourcesModel);

    connect(m_useTemplateCheckbox, SIGNAL(clicked()), SIGNAL(isValidChanged()));
    connect(m_templatesView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SIGNAL(isValidChanged()));
}

ResourceModel::ResourceItemType ResourceTemplateWidget::templateType() const
{
    return m_templateType;
}

void ResourceTemplateWidget::setTemplateType(ResourceModel::ResourceItemType type)
{
    if (type != m_templateType)
    {
        m_templateType = type;

        switch (m_templateType)
        {
        case ResourceModel::CourseItem:
            m_useTemplateCheckbox->setText(i18n("Use another course as template"));
            break;
        case ResourceModel::KeyboardLayoutItem:
            m_useTemplateCheckbox->setText(i18n("Use another keyboard layout as template"));
            break;
        default:
            break;
        }

        m_filteredResourcesModel->setResourceTypeFilter(m_templateType);
    }
}


bool ResourceTemplateWidget::isValid() const
{
    if (!m_useTemplateCheckbox->isChecked())
        return true;

    return m_templatesView->selectionModel()->hasSelection();
}

Resource* ResourceTemplateWidget::templateResource() const
{
    if (!m_useTemplateCheckbox->isChecked())
        return 0;
    if (!m_templatesView->selectionModel()->hasSelection())
        return 0;
    const QModelIndex index = m_templatesView->selectionModel()->currentIndex();
    QObject* const object = m_filteredResourcesModel->data(index, ResourceModel::DataRole).value<QObject*>();
    return qobject_cast<Resource*>(object);
}

