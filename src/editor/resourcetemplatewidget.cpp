/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */


#include "resourcetemplatewidget.h"

#include <KLocalizedString>

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

    connect(m_useTemplateCheckbox, &QAbstractButton::clicked, this, &ResourceTemplateWidget::isValidChanged);
    connect(m_templatesView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ResourceTemplateWidget::isValidChanged);
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

