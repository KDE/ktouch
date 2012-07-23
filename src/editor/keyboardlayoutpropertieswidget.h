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

#ifndef KEYBOARDLAYOUTPROPERTIESWIDGET_H
#define KEYBOARDLAYOUTPROPERTIESWIDGET_H

#include <QWidget>
#include "ui_keyboardlayoutpropertieswidget.h"

class QUndoStack;
class KeyboardLayout;
class AbstractKey;

class KeyboardLayoutPropertiesWidget : public QWidget, private Ui::KeyboardLayoutPropertiesWidget
{
    Q_OBJECT
public:
    KeyboardLayoutPropertiesWidget(QWidget* parent = 0);
    void setKeyboardLayout(KeyboardLayout* layout);
    void setUndoStack(QUndoStack* undoStack);
    void setSelectedKey(AbstractKey* key);
    void setReadOnly(bool readOnly);
private slots:
    void updateFont();
    void setKeyboardLayoutTitle(const QString& title);
    void setKeyboardLayoutName(const QString& name);
    void setKeyboardLayoutSize(const QSize &size);
    void updateKeyboardLayoutTitle();
    void updateKeyboardLayoutName();
    void updateKeyboardLayoutWidth();
    void updateKeyboardLayoutHeight();
    void onKeyboardLayoutWidthChanged(int width);
    void onKeyboardLayoutHeightChanged(int height);
private:
    KeyboardLayout* m_keyboardLayout;
    AbstractKey* m_selectedKey;
    QUndoStack* m_undoStack;
};

#endif // KEYBOARDLAYOUTPROPERTIESWIDGET_H
