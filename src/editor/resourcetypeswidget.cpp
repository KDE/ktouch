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

#include "resourcetypeswidget.h"

#include <QStandardItemModel>

ResourceTypesWidget::ResourceTypesWidget(QWidget* parent) :
    QWidget(parent),
    Ui::ResourceTypesWidget(),
    m_typesModel(new QStandardItemModel(this))
{
    setupUi(this);

    QStandardItem* courseItem = new QStandardItem(i18n("Course"));
    courseItem->setData(ResourceModel::CourseItem);
    courseItem->setEditable(false);

    QStandardItem* keyboardLayoutItem = new QStandardItem(i18n("Keyboard layout"));
    keyboardLayoutItem->setData(ResourceModel::KeyboardLayoutItem);
    keyboardLayoutItem->setEditable(false);

    m_typesModel->appendRow(courseItem);
    m_typesModel->appendRow(keyboardLayoutItem);

    m_typesView->setModel(m_typesModel);

    connect(m_typesView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), SLOT(currentRowChanged(QModelIndex)));
}

void ResourceTypesWidget::currentRowChanged(const QModelIndex& current)
{
    const QStandardItem* item = m_typesModel->item(current.row());
    emit typeSelected(static_cast<ResourceModel::ResourceItemType>(item->data().toInt()));
}
