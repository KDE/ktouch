/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef NEWCOURSEWIDGET_H
#define NEWCOURSEWIDGET_H

#include <QWidget>
#include "ui_newcoursewidget.h"

class ResourceModel;

class NewCourseWidget : public QWidget, private Ui::NewCourseWidget
{
    Q_OBJECT
public:
    explicit NewCourseWidget(ResourceModel* resourceModel, QWidget* parent = 0);
    bool isValid() const;
    QString title() const;
    QString keyboardLayoutName() const;
    QString description() const;
signals:
    void isValidChanged() const;
};

#endif // NEWCOURSEWIDGET_H
