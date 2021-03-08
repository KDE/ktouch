/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef RESOURCETYPESWIDGET_H
#define RESOURCETYPESWIDGET_H

#include <QWidget>
#include "ui_resourcetypeswidget.h"

#include "models/resourcemodel.h"

class QStandardItemModel;

class ResourceTypesWidget : public QWidget, private Ui::ResourceTypesWidget
{
    Q_OBJECT
public:
    explicit ResourceTypesWidget(QWidget* parent = 0);
signals:
    void typeSelected(ResourceModel::ResourceItemType type);
private slots:
    void currentRowChanged(const QModelIndex& current);
private:
    QStandardItemModel* m_typesModel;
};

#endif // RESOURCETYPESWIDGET_H
