/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "resourcetypeswidget.h"

#include <QFileDialog>
#include <QStandardItemModel>

#include <KLocalizedString>

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

    connect(m_typesView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &ResourceTypesWidget::currentRowChanged);
}

void ResourceTypesWidget::currentRowChanged(const QModelIndex& current)
{
    const QStandardItem* item = m_typesModel->item(current.row());
    emit typeSelected(static_cast<ResourceModel::ResourceItemType>(item->data().toInt()));
}
