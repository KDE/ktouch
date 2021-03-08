/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef KEYBOARDLAYOUTPROPERTIESWIDGET_H
#define KEYBOARDLAYOUTPROPERTIESWIDGET_H

#include <QWidget>
#include "ui_keyboardlayoutpropertieswidget.h"

class QUndoStack;
class KeyboardLayout;
class AbstractKey;
class CharactersModel;
class CharactersViewDelegate;

class KeyboardLayoutPropertiesWidget : public QWidget, private Ui::KeyboardLayoutPropertiesWidget
{
    Q_OBJECT
public:
    explicit KeyboardLayoutPropertiesWidget(QWidget* parent = 0);
    void setKeyboardLayout(KeyboardLayout* layout);
    void setUndoStack(QUndoStack* undoStack);
    void setSelectedKey(int index);
    void setReadOnly(bool readOnly);
private slots:
    void setKeyboardLayoutTitle(const QString& title);
    void setKeyboardLayoutName(const QString& name);
    void setKeyboardLayoutSize(const QSize& size);
    void setKeyGeometry(const QRect& rect);
    void setKeyFingerIndex(int fingerIndex);
    void setKeyHasHapticMarker(bool hasHapticMarker);
    void addCharacter();
    void removeCharacter();
    void setSpecialKeyType(int type);
    void setSpecialKeyLabel(const QString& label);
    void setSpecialKeyModifierId(const QString& id);
    void updateKeyboardLayoutTitle();
    void updateKeyboardLayoutName();
    void updateKeyboardLayoutWidth();
    void updateKeyboardLayoutHeight();
    void updateKeyLeft();
    void updateKeyTop();
    void updateKeyWidth();
    void updateKeyHeight();
    void resetKeyGeometry(AbstractKey* key);
    void updateKeyFingerIndex();
    void updateKeyHasHapticMarker();
    void updateSpecialKeyType();
    void updateSpecialKeyLabel();
    void updateSpecialKeyModifierId();
    void onKeyboardLayoutWidthChanged(int width);
    void onKeyboardLayoutHeightChanged(int height);
    void onKeyLeftChanged(int left);
    void onKeyTopChanged(int top);
    void onKeyWidthChanged(int width);
    void onKeyHeightChanged(int height);
    void onFingerIndexChanged(int fingerIndex);
    void onCharacterSelected();
    void onSpecialKeyTypeChanged(int type);
private:
    KeyboardLayout* m_keyboardLayout;
    int m_selectedKeyIndex;
    AbstractKey* m_selectedKey;
    bool m_readOnly;
    QUndoStack* m_undoStack;
    CharactersModel* m_charactersModel;
    CharactersViewDelegate* m_charactersViewDelegate;
};

#endif // KEYBOARDLAYOUTPROPERTIESWIDGET_H
