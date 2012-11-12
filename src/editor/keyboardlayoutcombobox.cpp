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
