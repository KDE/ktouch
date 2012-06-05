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
