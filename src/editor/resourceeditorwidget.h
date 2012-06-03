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


#ifndef RESOURCEEDITORWIDGET_H
#define RESOURCEEDITORWIDGET_H

#include <QWidget>
#include "ui_resourceeditorwidget.h"

class QAbstractItemView;
class KMessageWidget;

class ResourceEditorWidget : public QWidget, private Ui::ResourceEditorWidget
{
    Q_OBJECT
public:
    explicit ResourceEditorWidget(QWidget* parent = 0);
    KMessageWidget* messageWidget() const;
    QAbstractItemView* resourceView() const;
};

#endif // RESOURCEEDITORWIDGET_H
