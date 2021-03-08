/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
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
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly NOTIFY readOnlyChanged)
    Q_PROPERTY(AbstractKey* selectedKey READ selectedKey WRITE setSelectedKey NOTIFY selectedKeyChanged)
    Q_PROPERTY(int zoomLevel READ zoomLevel WRITE setZoomLevel NOTIFY zoomLevelChanged)
public:
    explicit KeyboardLayoutEditor(QWidget* parent = 0);
    ~KeyboardLayoutEditor();

    void openKeyboardLayout(DataIndexKeyboardLayout* dataIndexKeyboardLayout);
    void clearUndoStackForKeyboardLayout(DataIndexKeyboardLayout* dataIndexKeyboardLayout);
    void save();

    KeyboardLayout* keyboardLayout() const;
    bool readOnly() const;
    void setReadOnly(bool readOnly);
    AbstractKey* selectedKey() const;
    void setSelectedKey(AbstractKey* key);
    int zoomLevel() const;
    Q_SLOT void setZoomLevel(int zoomLevel);

    Q_INVOKABLE void setKeyGeometry(int keyIndex, int top, int left, int width, int height);
signals:
    void keyboardLayoutChanged();
    void readOnlyChanged();
    void selectedKeyChanged();
    void zoomLevelChanged();
private slots:
    void clearSelection();
    void validateSelection();
    void createNewKey();
    void createNewSpecialKey();
    void deleteSelectedKey();
private:
    DataIndexKeyboardLayout* m_dataIndexKeyboardLayout;
    KeyboardLayout* m_keyboardLayout;
    bool m_readOnly;
    AbstractKey* m_selectedKey;
    int m_zoomLevel;
};

#endif // KEYBOARDLAYOUTEDITOR_H
