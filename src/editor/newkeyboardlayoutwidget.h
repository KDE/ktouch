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

#ifndef NEWKEYBOARDLAYOUTWIDGET_H
#define NEWKEYBOARDLAYOUTWIDGET_H

#include <QWidget>
#include "ui_newkeyboardlayoutwidget.h"

class ResourceModel;

class NewKeyboardLayoutWidget : public QWidget, private Ui::NewKeyboardLayoutWidget
{
    Q_OBJECT
public:
    explicit NewKeyboardLayoutWidget(ResourceModel* resourceModel, QWidget* parent = 0);
    QString name() const;
    QString title() const;
    bool isValid() const;
signals:
    void isValidChanged();
private slots:
    void pasteCurrentName();
    void checkName();
private:
    bool m_nameIsValid;
    ResourceModel* m_resourceModel;
};

#endif // NEWKEYBOARDLAYOUTWIDGET_H
