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

#include "newcoursewidget.h"

#include "resourcemodel.h"
#include "categorizedresourcesortfilterproxymodel.h"

NewCourseWidget::NewCourseWidget(ResourceModel* resourceModel, QWidget* parent) :
    QWidget(parent),
    Ui::NewCourseWidget(),
    m_resourceModel(resourceModel)
{
    setupUi(this);

    connect(m_idLineEdit, SIGNAL(textChanged(QString)), SIGNAL(isValidChanged()));
    connect(m_titleLineEdit, SIGNAL(textChanged(QString)), SIGNAL(isValidChanged()));
    connect(m_keyboardLayoutComboBox, SIGNAL(currentIndexChanged(int)), SIGNAL(isValidChanged()));

    CategorizedResourceSortFilterProxyModel* filteredResourcesModel = new CategorizedResourceSortFilterProxyModel(this);
    filteredResourcesModel->setSourceModel(m_resourceModel);
    filteredResourcesModel->setResourceTypeFilter(ResourceModel::KeyboardLayoutItem);

    m_keyboardLayoutComboBox->setModel(filteredResourcesModel);
}

bool NewCourseWidget::isValid() const
{
    if (m_idLineEdit->text().isEmpty())
        return false;

    if (m_titleLineEdit->text().isEmpty())
        return false;

    if (m_keyboardLayoutComboBox->currentIndex() == -1)
        return false;

    return true;
}
