/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */


#include "keyboardlayoutcombobox.h"

#include "models/categorizedresourcesortfilterproxymodel.h"


KeyboardLayoutComboBox::KeyboardLayoutComboBox(QWidget* parent) :
    KComboBox(parent),
    m_filteredResourceModel(new CategorizedResourceSortFilterProxyModel(this)),
    m_resourceModel(0)
{
    m_filteredResourceModel->setResourceTypeFilter(ResourceModel::KeyboardLayoutItem);
    setModel(m_filteredResourceModel);
}

ResourceModel* KeyboardLayoutComboBox::resourceModel() const
{
    return m_resourceModel;
}

void KeyboardLayoutComboBox::setResourceModel(ResourceModel* model)
{
    if (model != m_resourceModel)
    {
        m_resourceModel = model;
        m_filteredResourceModel->setResourceModel(model);
    }
}

DataIndexKeyboardLayout* KeyboardLayoutComboBox::keyboardLayoutAt(int index) const
{
    const QModelIndex modelIndex = model()->index(index, 0);
    const QVariant variant = model()->data(modelIndex, ResourceModel::DataRole);
    QObject* const object = qvariant_cast<QObject*>(variant);
    DataIndexKeyboardLayout* const layout = qobject_cast<DataIndexKeyboardLayout*>(object);

    return layout;

}

DataIndexKeyboardLayout* KeyboardLayoutComboBox::selectedKeyboardLayout() const
{
    if (currentIndex() == -1)
        return 0;


    return keyboardLayoutAt(currentIndex());
}

void KeyboardLayoutComboBox::selectKeyboardLayout(DataIndexKeyboardLayout* layout)
{
    for (int i = 0; i < model()->rowCount(); i++)
    {
        if (keyboardLayoutAt(i) == layout)
        {
            setCurrentIndex(i);
            break;
        }
    }
}
