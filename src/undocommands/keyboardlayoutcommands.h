/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef KEYBOARDLAYOUTCOMMANDS_H
#define KEYBOARDLAYOUTCOMMANDS_H

#include <QUndoStack>
#include <QRect>

#include "core/specialkey.h"
#include "core/keychar.h"

class KeyboardLayout;

class SetKeyboardLayoutTitleCommand : public QUndoCommand
{
public:
    explicit SetKeyboardLayoutTitleCommand(KeyboardLayout* layout, const QString& newTitle, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    int id() const override;
    bool mergeWith(const QUndoCommand* other) override;
private:
    KeyboardLayout* m_layout;
    QString m_oldTitle;
    QString m_newTitle;
};

class SetKeyboardLayoutNameCommand : public QUndoCommand
{
public:
    explicit SetKeyboardLayoutNameCommand(KeyboardLayout* layout, const QString& newName, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    int id() const override;
    bool mergeWith(const QUndoCommand* other) override;
private:
    KeyboardLayout* m_layout;
    QString m_oldName;
    QString m_newName;
};

class SetKeyboardLayoutSizeCommand : public QUndoCommand
{
public:
    explicit SetKeyboardLayoutSizeCommand(KeyboardLayout* layout, const QSize& newSize, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    int id() const override;
    bool mergeWith(const QUndoCommand* other) override;
private:
    KeyboardLayout* m_layout;
    QSize m_oldSize;
    QSize m_newSize;
};

class AddKeyCommand : public QUndoCommand
{
public:
    AddKeyCommand(KeyboardLayout* layout, AbstractKey* key, QUndoCommand* parent = nullptr);
    ~AddKeyCommand() override;
    void undo() override;
    void redo() override;
    int id() const override;
    bool mergeWith(const QUndoCommand* other) override;
private:
    KeyboardLayout* m_layout;
    AbstractKey* m_backupKey;
};

class RemoveKeyCommand : public QUndoCommand
{
public:
    RemoveKeyCommand(KeyboardLayout* layout, int keyIndex, QUndoCommand* parent = nullptr);
    ~RemoveKeyCommand();
    void undo() override;
    void redo() override;
    int id() const override;
    bool mergeWith(const QUndoCommand* other) override;
private:
    KeyboardLayout* m_layout;
    int m_keyIndex;
    AbstractKey* m_backupKey;
};

class SetKeyGeometryCommand : public QUndoCommand
{
public:
    explicit SetKeyGeometryCommand(KeyboardLayout* layout, int keyIndex, const QRect& newRect, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    int id() const override;
    bool mergeWith(const QUndoCommand* other) override;
private:
    KeyboardLayout* m_layout;
    int m_keyIndex;
    QRect m_oldRect;
    QRect m_newRect;
};

class SetKeyFingerIndexCommand : public QUndoCommand
{
public:
    explicit SetKeyFingerIndexCommand(KeyboardLayout* layout, int keyIndex, int newFingerIndex, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    int id() const override;
    bool mergeWith(const QUndoCommand* other) override;
private:
    KeyboardLayout* m_layout;
    int m_keyIndex;
    int m_oldFingerIndex;
    int m_newFingerIndex;
};

class SetKeyHasHapticMarkerCommand : public QUndoCommand
{
public:
    explicit SetKeyHasHapticMarkerCommand(KeyboardLayout* layout, int keyIndex, bool newHasHapticMarker, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    int id() const override;
    bool mergeWith(const QUndoCommand* other) override;
private:
    KeyboardLayout* m_layout;
    int m_keyIndex;
    bool m_oldHasHapticMarker;
    bool m_newHasHapticMarker;
};

class AddKeyCharCommand : public QUndoCommand
{
public:
    AddKeyCharCommand(KeyboardLayout* layout, int keyIndex, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    int id() const override;
    bool mergeWith(const QUndoCommand* other) override;
private:
    KeyboardLayout* m_layout;
    int m_keyIndex;
};

class RemoveKeyCharCommand : public QUndoCommand
{
public:
    RemoveKeyCharCommand(KeyboardLayout* layout, int keyIndex, int keyCharIndex, QUndoCommand* parent = nullptr);
    ~RemoveKeyCharCommand();
    void undo() override;
    void redo() override;
    int id() const override;
    bool mergeWith(const QUndoCommand* other) override;
private:
    KeyboardLayout* m_layout;
    int m_keyIndex;
    int m_keyCharIndex;
    KeyChar* m_backupKeyChar;
};

class SetKeyCharValueCommand : public QUndoCommand
{
public:
    explicit SetKeyCharValueCommand(KeyboardLayout* layout, int keyIndex, int keyCharIndex, QChar newValue, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    int id() const override;
    bool mergeWith(const QUndoCommand* other) override;
private:
    KeyboardLayout* m_layout;
    int m_keyIndex;
    int m_keyCharIndex;
    QChar m_oldValue;
    QChar m_newValue;
};

class SetKeyCharModifierCommand : public QUndoCommand
{
public:
    explicit SetKeyCharModifierCommand(KeyboardLayout* layout, int keyIndex, int keyCharIndex, const QString& newModifier, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    int id() const override;
    bool mergeWith(const QUndoCommand* other) override;
private:
    KeyboardLayout* m_layout;
    int m_keyIndex;
    int m_keyCharIndex;
    QString m_oldModifier;
    QString m_newModifier;
};

class SetKeyCharPositionCommand : public QUndoCommand
{
public:
    explicit SetKeyCharPositionCommand(KeyboardLayout* layout, int keyIndex, int keyCharIndex, KeyChar::Position newPosition, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    int id() const override;
    bool mergeWith(const QUndoCommand* other) override;
private:
    KeyboardLayout* m_layout;
    int m_keyIndex;
    int m_keyCharIndex;
    KeyChar::Position m_oldPosition;
    KeyChar::Position m_newPosition;
};

class SetSpecialKeyTypeCommand : public QUndoCommand
{
public:
    explicit SetSpecialKeyTypeCommand(KeyboardLayout* layout, int keyIndex, SpecialKey::Type newType, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    int id() const override;
    bool mergeWith(const QUndoCommand* other) override;
private:
    KeyboardLayout* m_layout;
    int m_keyIndex;
    SpecialKey::Type m_oldType;
    SpecialKey::Type m_newType;
};

class SetSpecialKeyLabelCommand : public QUndoCommand
{
public:
    explicit SetSpecialKeyLabelCommand(KeyboardLayout* layout, int keyIndex, const QString& newLabel, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    int id() const override;
    bool mergeWith(const QUndoCommand* other) override;
private:
    KeyboardLayout* m_layout;
    int m_keyIndex;
    QString m_oldLabel;
    QString m_newLabel;
};

class SetSpecialKeyModifierIdCommand : public QUndoCommand
{
public:
    explicit SetSpecialKeyModifierIdCommand(KeyboardLayout* layout, int keyIndex, const QString& newModifiewId, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    int id() const override;
    bool mergeWith(const QUndoCommand* other) override;
private:
    KeyboardLayout* m_layout;
    int m_keyIndex;
    QString m_oldModifierId;
    QString m_newModifierId;
};

#endif // KEYBOARDLAYOUTCOMMANDS_H
