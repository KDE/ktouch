/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
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
