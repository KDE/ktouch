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

#ifndef KEYBOARDLAYOUTEDITOR_H
#define KEYBOARDLAYOUTEDITOR_H

#include "editor/abstracteditor.h"
#include "ui_keyboardlayouteditor.h"

class DataIndexKeyboardLayout;
class KeyboardLayout;
class AbstractKey;

class KeyboardLayoutEditor : public AbstractEditor, private Ui::KeyboardLayoutEditor
{
    Q_OBJECT
    Q_PROPERTY(KeyboardLayout* keyboardLayout READ keyboardLayout NOTIFY keyboardLayoutChanged)
    Q_PROPERTY(AbstractKey* selectedKey READ selectedKey WRITE setSelectedKey NOTIFY selectedKeyChanged)
public:
    explicit KeyboardLayoutEditor(QWidget* parent = 0);

    void openKeyboardLayout(DataIndexKeyboardLayout* dataIndexKeyboardLayout);
    void clearUndoStackForKeyboardLayout(DataIndexKeyboardLayout* dataIndexKeyboardLayout);
    void save();

    KeyboardLayout* keyboardLayout() const;
    AbstractKey* selectedKey() const;
    void setSelectedKey(AbstractKey* key);
signals:
    void keyboardLayoutChanged();
    void selectedKeyChanged();
private slots:
    void clearSelection();
private:
    DataIndexKeyboardLayout* m_dataIndexKeyboardLayout;
    KeyboardLayout* m_keyboardLayout;
    AbstractKey* m_selectedKey;
};

#endif // KEYBOARDLAYOUTEDITOR_H
