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

#ifndef KEYBOARDLAYOUTCOMBOBOX_H
#define KEYBOARDLAYOUTCOMBOBOX_H

#include "KComboBox"

class ResourceModel;
class CategorizedResourceSortFilterProxyModel;
class DataIndexKeyboardLayout;

class KeyboardLayoutComboBox: public KComboBox
{
    Q_OBJECT
public:
    explicit KeyboardLayoutComboBox(QWidget* parent = 0);
    ResourceModel* resourceModel() const;
    void setResourceModel(ResourceModel* model);
    DataIndexKeyboardLayout* keyboardLayoutAt(int index) const;
    DataIndexKeyboardLayout* selectedKeyboardLayout() const;
    void selectKeyboardLayout(DataIndexKeyboardLayout* layout);
private:
    CategorizedResourceSortFilterProxyModel* m_filteredResourceModel;
    ResourceModel* m_resourceModel;
};

#endif // KEYBOARDLAYOUTCOMBOBOX_H
