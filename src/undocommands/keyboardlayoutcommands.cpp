/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "keyboardlayoutcommands.h"

#include <KLocalizedString>

#include <core/keyboardlayout.h>
#include <core/abstractkey.h>
#include <core/key.h>

SetKeyboardLayoutTitleCommand::SetKeyboardLayoutTitleCommand(KeyboardLayout* layout, const QString& newTitle, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_layout(layout),
    m_oldTitle(layout->title()),
    m_newTitle(newTitle)
{
    setText(i18n("Set keyboard layout title"));
}

void SetKeyboardLayoutTitleCommand::undo()
{
    m_layout->setTitle(m_oldTitle);
}

void SetKeyboardLayoutTitleCommand::redo()
{
    m_layout->setTitle(m_newTitle);
}

int SetKeyboardLayoutTitleCommand::id() const
{
    return 0xa7b18eae;
}

bool SetKeyboardLayoutTitleCommand::mergeWith(const QUndoCommand* other)
{
    const SetKeyboardLayoutTitleCommand* setKeyboardLayoutTitleCommand = static_cast<const SetKeyboardLayoutTitleCommand*>(other);

    if (m_layout != setKeyboardLayoutTitleCommand->m_layout)
        return false;

    m_newTitle = setKeyboardLayoutTitleCommand->m_newTitle;
    return true;
}

SetKeyboardLayoutNameCommand::SetKeyboardLayoutNameCommand(KeyboardLayout* layout, const QString& newName, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_layout(layout),
    m_oldName(layout->name()),
    m_newName(newName)
{
    setText(i18n("Set keyboard layout name"));
}

void SetKeyboardLayoutNameCommand::undo()
{
    m_layout->setName(m_oldName);
}

void SetKeyboardLayoutNameCommand::redo()
{
    m_layout->setName(m_newName);
}

int SetKeyboardLayoutNameCommand::id() const
{
    return 0x24c31e8b;
}

bool SetKeyboardLayoutNameCommand::mergeWith(const QUndoCommand* other)
{
    const SetKeyboardLayoutNameCommand* setKeyboardLayoutNameCommand = static_cast<const SetKeyboardLayoutNameCommand*>(other);

    if (m_layout != setKeyboardLayoutNameCommand->m_layout)
        return false;

    m_newName = setKeyboardLayoutNameCommand->m_newName;
    return true;
}

SetKeyboardLayoutSizeCommand::SetKeyboardLayoutSizeCommand(KeyboardLayout* layout, const QSize& newSize, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_layout(layout),
    m_oldSize(layout->size()),
    m_newSize(newSize)
{
    setText(i18n("Set keyboard layout size"));

    for (int i = 0; i < m_layout->keyCount(); i++)
    {
        AbstractKey* const key = m_layout->key(i);
        QRect rect(key->rect());

        if (key->left() + key->width() > newSize.width())
        {
            rect.moveLeft(newSize.width() - key->width());
        }

        if (key->top() + key->height() > newSize.height())
        {
            rect.moveTop(newSize.height() - key->height());
        }

        if (rect != key->rect())
        {
            new SetKeyGeometryCommand(m_layout, i, rect, this);
        }
    }
}

void SetKeyboardLayoutSizeCommand::undo()
{
    QUndoCommand::undo();
    m_layout->setSize(m_oldSize);
}

void SetKeyboardLayoutSizeCommand::redo()
{
    QUndoCommand::redo();
    m_layout->setSize(m_newSize);
}

int SetKeyboardLayoutSizeCommand::id() const
{
    return 0xaad8b7c8;
}

bool SetKeyboardLayoutSizeCommand::mergeWith(const QUndoCommand* other)
{
    Q_UNUSED(other);
    return false;
}

AddKeyCommand::AddKeyCommand(KeyboardLayout *layout, AbstractKey *key, QUndoCommand *parent) :
    QUndoCommand(parent),
    m_layout(layout),
    m_backupKey(key)
{
    setText(i18n("Add key"));
}

AddKeyCommand::~AddKeyCommand()
{
    delete m_backupKey;
}

void AddKeyCommand::undo()
{
    const int keyIndex = m_layout->keyCount() - 1;
    AbstractKey* abstractKey = m_layout->key(keyIndex);

    if (Key* key = qobject_cast<Key*>(abstractKey))
    {
        Key* backupKey = new Key();
        backupKey->copyFrom(key);
        m_backupKey = backupKey;
    }
    else if (SpecialKey* specialKey = qobject_cast<SpecialKey*>(abstractKey))
    {
        SpecialKey* specialKeyBackup = new SpecialKey();
        specialKey->copyFrom(specialKey);
        m_backupKey = specialKeyBackup;
    }

    m_layout->removeKey(keyIndex);
}

void AddKeyCommand::redo()
{
    Q_ASSERT(m_backupKey);

    m_layout->addKey(m_backupKey);

    m_backupKey = 0;
}

int AddKeyCommand::id() const
{
    return 0xfd2d9a61;
}

bool AddKeyCommand::mergeWith(const QUndoCommand *other)
{
    Q_UNUSED(other);
    return false;
}

RemoveKeyCommand::RemoveKeyCommand(KeyboardLayout* layout, int keyIndex, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_layout(layout),
    m_keyIndex(keyIndex),
    m_backupKey(0)
{
    setText(i18n("Remove key"));
}

RemoveKeyCommand::~RemoveKeyCommand()
{
    delete m_backupKey;
}

void RemoveKeyCommand::undo()
{
    Q_ASSERT(m_backupKey);

    if (m_keyIndex == m_layout->keyCount())
    {
        m_layout->addKey(m_backupKey);
    }
    else
    {
        m_layout->insertKey(m_keyIndex, m_backupKey);
    }

    m_backupKey = 0;
}

void RemoveKeyCommand::redo()
{
    AbstractKey* abstractKey = m_layout->key(m_keyIndex);

    if (Key* key = qobject_cast<Key*>(abstractKey))
    {
        Key* backupKey = new Key();
        backupKey->copyFrom(key);
        m_backupKey = backupKey;
    }
    else if (SpecialKey* specialKey = qobject_cast<SpecialKey*>(abstractKey))
    {
        SpecialKey* specialKeyBackup = new SpecialKey();
        specialKeyBackup->copyFrom(specialKey);
        m_backupKey = specialKeyBackup;
    }

    m_layout->removeKey(m_keyIndex);
}

int RemoveKeyCommand::id() const
{
    return 0xf992f4a7;
}

bool RemoveKeyCommand::mergeWith(const QUndoCommand* other)
{
    Q_UNUSED(other)
    return false;
}

SetKeyGeometryCommand::SetKeyGeometryCommand(KeyboardLayout* layout, int keyIndex, const QRect& newRect, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_layout(layout),
    m_keyIndex(keyIndex),
    m_oldRect(layout->key(keyIndex)->rect()),
    m_newRect(newRect)
{
    setText(i18n("Set key geometry"));
}

void SetKeyGeometryCommand::undo()
{
    m_layout->key(m_keyIndex)->setRect(m_oldRect);
}

void SetKeyGeometryCommand::redo()
{
    m_layout->key(m_keyIndex)->setRect(m_newRect);
}

int SetKeyGeometryCommand::id() const
{
    return 0x7260c67c;
}

bool SetKeyGeometryCommand::mergeWith(const QUndoCommand* other)
{
    const SetKeyGeometryCommand* setKeyGeometryCommand = static_cast<const SetKeyGeometryCommand*>(other);

    if (m_layout != setKeyGeometryCommand->m_layout)
        return false;

    if (m_keyIndex != setKeyGeometryCommand->m_keyIndex)
        return false;

    m_newRect = setKeyGeometryCommand->m_newRect;
    return true;
}

SetKeyFingerIndexCommand::SetKeyFingerIndexCommand(KeyboardLayout* layout, int keyIndex, int newFingerIndex, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_layout(layout),
    m_keyIndex(keyIndex),
    m_newFingerIndex(newFingerIndex)
{
    setText(i18n("Set key finger"));

    Key* key = qobject_cast<Key*>(m_layout->key(m_keyIndex));
    Q_ASSERT(key);
    m_oldFingerIndex = key->fingerIndex();
}

void SetKeyFingerIndexCommand::undo()
{
    Key* key = qobject_cast<Key*>(m_layout->key(m_keyIndex));
    Q_ASSERT(key);
    key->setFingerIndex(m_oldFingerIndex);
}

void SetKeyFingerIndexCommand::redo()
{
    Key* key = qobject_cast<Key*>(m_layout->key(m_keyIndex));
    Q_ASSERT(key);
    key->setFingerIndex(m_newFingerIndex);
}

int SetKeyFingerIndexCommand::id() const
{
    return 0xcecb02ad;
}

bool SetKeyFingerIndexCommand::mergeWith(const QUndoCommand* other)
{
    const SetKeyFingerIndexCommand* setKeyFingerIndexCommand = static_cast<const SetKeyFingerIndexCommand*>(other);

    if (m_layout != setKeyFingerIndexCommand->m_layout)
        return false;

    if (m_keyIndex != setKeyFingerIndexCommand->m_keyIndex)
        return false;

    m_newFingerIndex = setKeyFingerIndexCommand->m_newFingerIndex;
    return true;
}

SetKeyHasHapticMarkerCommand::SetKeyHasHapticMarkerCommand(KeyboardLayout* layout, int keyIndex, bool newHasHapticMarker, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_layout(layout),
    m_keyIndex(keyIndex),
    m_newHasHapticMarker(newHasHapticMarker)
{
    setText(i18n("Set key haptic marker"));

    Key* key = qobject_cast<Key*>(m_layout->key(m_keyIndex));
    Q_ASSERT(key);
    m_oldHasHapticMarker = key->hasHapticMarker();
}

void SetKeyHasHapticMarkerCommand::undo()
{
    Key* key = qobject_cast<Key*>(m_layout->key(m_keyIndex));
    Q_ASSERT(key);
    key->setHasHapticMarker(m_oldHasHapticMarker);
}

void SetKeyHasHapticMarkerCommand::redo()
{
    Key* key = qobject_cast<Key*>(m_layout->key(m_keyIndex));
    Q_ASSERT(key);
    key->setHasHapticMarker(m_newHasHapticMarker);
}

int SetKeyHasHapticMarkerCommand::id() const
{
    return 0xf436020c;
}

bool SetKeyHasHapticMarkerCommand::mergeWith(const QUndoCommand* other)
{
    const SetKeyHasHapticMarkerCommand* setKeyHasHapticMarkerCommand = static_cast<const SetKeyHasHapticMarkerCommand*>(other);

    if (m_layout != setKeyHasHapticMarkerCommand->m_layout)
        return false;

    if (m_keyIndex != setKeyHasHapticMarkerCommand->m_keyIndex)
        return false;

    m_newHasHapticMarker = setKeyHasHapticMarkerCommand->m_newHasHapticMarker;
    return true;
}

AddKeyCharCommand::AddKeyCharCommand(KeyboardLayout* layout, int keyIndex, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_layout(layout),
    m_keyIndex(keyIndex)
{
    setText(i18n("Add key character"));
}

void AddKeyCharCommand::undo()
{
    Key* key = qobject_cast<Key*>(m_layout->key(m_keyIndex));

    Q_ASSERT(key);
    Q_ASSERT(key->keyCharCount() > 0);

    key->removeKeyChar(key->keyCharCount() - 1);
}

void AddKeyCharCommand::redo()
{
    Key* key = qobject_cast<Key*>(m_layout->key(m_keyIndex));

    Q_ASSERT(key);

    KeyChar* keyChar = new KeyChar();
    key->addKeyChar(keyChar);
}

int AddKeyCharCommand::id() const
{
    return 0x9808f018;
}

bool AddKeyCharCommand::mergeWith(const QUndoCommand* other)
{
    Q_UNUSED(other)
    return false;
}

RemoveKeyCharCommand::RemoveKeyCharCommand(KeyboardLayout* layout, int keyIndex, int keyCharIndex, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_layout(layout),
    m_keyIndex(keyIndex),
    m_keyCharIndex(keyCharIndex),
    m_backupKeyChar(0)
{
    setText(i18n("Remove key character"));
}

RemoveKeyCharCommand::~RemoveKeyCharCommand()
{
    delete m_backupKeyChar;
}

void RemoveKeyCharCommand::undo()
{
    Key* key = qobject_cast<Key*>(m_layout->key(m_keyIndex));

    Q_ASSERT(m_backupKeyChar);
    Q_ASSERT(key);

    if (m_keyCharIndex == key->keyCharCount())
    {
        key->addKeyChar(m_backupKeyChar);
    }
    else
    {
        key->insertKeyChar(m_keyCharIndex, m_backupKeyChar);
    }

    m_backupKeyChar = 0;
}

void RemoveKeyCharCommand::redo()
{
    Key* key = qobject_cast<Key*>(m_layout->key(m_keyIndex));

    Q_ASSERT(key);

    m_backupKeyChar = new KeyChar();
    m_backupKeyChar->copyFrom(key->keyChar(m_keyCharIndex));

    key->removeKeyChar(m_keyCharIndex);
}

int RemoveKeyCharCommand::id() const
{
    return 0x4b6252f5;
}

bool RemoveKeyCharCommand::mergeWith(const QUndoCommand* other)
{
    Q_UNUSED(other)
    return false;
}

SetKeyCharValueCommand::SetKeyCharValueCommand(KeyboardLayout* layout, int keyIndex, int keyCharIndex, QChar newValue, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_layout(layout),
    m_keyIndex(keyIndex),
    m_keyCharIndex(keyCharIndex),
    m_newValue(newValue)
{
    setText(i18n("Set key character value"));

    Key* key = qobject_cast<Key*>(m_layout->key(m_keyIndex));
    m_oldValue = key->keyChar(m_keyCharIndex)->value();
}

void SetKeyCharValueCommand::undo()
{
    Key* key = qobject_cast<Key*>(m_layout->key(m_keyIndex));
    key->keyChar(m_keyCharIndex)->setValue(m_oldValue);
}

void SetKeyCharValueCommand::redo()
{
    Key* key = qobject_cast<Key*>(m_layout->key(m_keyIndex));
    key->keyChar(m_keyCharIndex)->setValue(m_newValue);
}

int SetKeyCharValueCommand::id() const
{
    return 0x7a621de0;
}

bool SetKeyCharValueCommand::mergeWith(const QUndoCommand* other)
{
    const SetKeyCharValueCommand* setKeyCharValueCommand = static_cast<const SetKeyCharValueCommand*>(other);

    if (m_layout != setKeyCharValueCommand->m_layout)
        return false;

    if (m_keyIndex != setKeyCharValueCommand->m_keyIndex)
        return false;

    if (m_keyCharIndex != setKeyCharValueCommand->m_keyCharIndex)
        return false;

    m_newValue = setKeyCharValueCommand->m_newValue;
    return true;
}

SetKeyCharModifierCommand::SetKeyCharModifierCommand(KeyboardLayout* layout, int keyIndex, int keyCharIndex, const QString& newModifier, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_layout(layout),
    m_keyIndex(keyIndex),
    m_keyCharIndex(keyCharIndex),
    m_newModifier(newModifier)
{
    setText(i18n("Set key character modifier"));

    Key* key = qobject_cast<Key*>(m_layout->key(m_keyIndex));
    m_oldModifier = key->keyChar(m_keyCharIndex)->modifier();
}

void SetKeyCharModifierCommand::undo()
{
    Key* key = qobject_cast<Key*>(m_layout->key(m_keyIndex));
    key->keyChar(m_keyCharIndex)->setModifier(m_oldModifier);
}

void SetKeyCharModifierCommand::redo()
{
    Key* key = qobject_cast<Key*>(m_layout->key(m_keyIndex));
    key->keyChar(m_keyCharIndex)->setModifier(m_newModifier);
}

int SetKeyCharModifierCommand::id() const
{
    return 0xdd9f24d0;
}

bool SetKeyCharModifierCommand::mergeWith(const QUndoCommand* other)
{
    const SetKeyCharModifierCommand* setKeyCharModifierCommand = static_cast<const SetKeyCharModifierCommand*>(other);

    if (m_layout != setKeyCharModifierCommand->m_layout)
        return false;

    if (m_keyIndex != setKeyCharModifierCommand->m_keyIndex)
        return false;

    if (m_keyCharIndex != setKeyCharModifierCommand->m_keyCharIndex)
        return false;

    m_newModifier = setKeyCharModifierCommand->m_newModifier;
    return true;
}

SetKeyCharPositionCommand::SetKeyCharPositionCommand(KeyboardLayout* layout, int keyIndex, int keyCharIndex, KeyChar::Position newPosition, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_layout(layout),
    m_keyIndex(keyIndex),
    m_keyCharIndex(keyCharIndex),
    m_newPosition(newPosition)
{
    setText(i18n("Set key character position"));

    Key* key = qobject_cast<Key*>(m_layout->key(m_keyIndex));
    m_oldPosition = key->keyChar(m_keyCharIndex)->position();
}

void SetKeyCharPositionCommand::undo()
{
    Key* key = qobject_cast<Key*>(m_layout->key(m_keyIndex));
    key->keyChar(m_keyCharIndex)->setPosition(m_oldPosition);
}

void SetKeyCharPositionCommand::redo()
{
    Key* key = qobject_cast<Key*>(m_layout->key(m_keyIndex));
    key->keyChar(m_keyCharIndex)->setPosition(m_newPosition);
}

int SetKeyCharPositionCommand::id() const
{
    return 0x50192aaa;
}

bool SetKeyCharPositionCommand::mergeWith(const QUndoCommand* other)
{
    const SetKeyCharPositionCommand* setKeyCharPositionCommand = static_cast<const SetKeyCharPositionCommand*>(other);

    if (m_layout != setKeyCharPositionCommand->m_layout)
        return false;

    if (m_keyIndex != setKeyCharPositionCommand->m_keyIndex)
        return false;

    if (m_keyCharIndex != setKeyCharPositionCommand->m_keyCharIndex)
        return false;

    m_newPosition = setKeyCharPositionCommand->m_newPosition;
    return true;
}

SetSpecialKeyTypeCommand::SetSpecialKeyTypeCommand(KeyboardLayout* layout, int keyIndex, SpecialKey::Type newType, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_layout(layout),
    m_keyIndex(keyIndex),
    m_newType(newType)
{
    setText(i18n("Set special key type"));

    SpecialKey* key = qobject_cast<SpecialKey*>(m_layout->key(m_keyIndex));
    m_oldType = key->type();
}

void SetSpecialKeyTypeCommand::undo()
{
    SpecialKey* key = qobject_cast<SpecialKey*>(m_layout->key(m_keyIndex));
    key->setType(m_oldType);
}

void SetSpecialKeyTypeCommand::redo()
{
    SpecialKey* key = qobject_cast<SpecialKey*>(m_layout->key(m_keyIndex));
    key->setType(m_newType);
}

int SetSpecialKeyTypeCommand::id() const
{
    return 0xf1ce4bee;
}

bool SetSpecialKeyTypeCommand::mergeWith(const QUndoCommand* other)
{
    const SetSpecialKeyTypeCommand* setSpecialKeyTypeCommand = static_cast<const SetSpecialKeyTypeCommand*>(other);

    if (m_layout != setSpecialKeyTypeCommand->m_layout)
        return false;

    if (m_keyIndex != setSpecialKeyTypeCommand->m_keyIndex)
        return false;

    m_newType = setSpecialKeyTypeCommand->m_newType;
    return true;
}

SetSpecialKeyLabelCommand::SetSpecialKeyLabelCommand(KeyboardLayout* layout, int keyIndex, const QString& newLabel, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_layout(layout),
    m_keyIndex(keyIndex),
    m_newLabel(newLabel)
{
    setText(i18n("Set special key label"));

    SpecialKey* key = qobject_cast<SpecialKey*>(m_layout->key(m_keyIndex));
    m_oldLabel = key->label();
}

void SetSpecialKeyLabelCommand::undo()
{
    SpecialKey* key = qobject_cast<SpecialKey*>(m_layout->key(m_keyIndex));
    key->setLabel(m_oldLabel);
}

void SetSpecialKeyLabelCommand::redo()
{
    SpecialKey* key = qobject_cast<SpecialKey*>(m_layout->key(m_keyIndex));
    key->setLabel(m_newLabel);
}

int SetSpecialKeyLabelCommand::id() const
{
    return 0x6a1ddec9;
}

bool SetSpecialKeyLabelCommand::mergeWith(const QUndoCommand* other)
{
    const SetSpecialKeyLabelCommand* setSpecialKeyLabelCommand = static_cast<const SetSpecialKeyLabelCommand*>(other);

    if (m_layout != setSpecialKeyLabelCommand->m_layout)
        return false;

    if (m_keyIndex != setSpecialKeyLabelCommand->m_keyIndex)
        return false;

    m_newLabel = setSpecialKeyLabelCommand->m_newLabel;
    return true;
}

SetSpecialKeyModifierIdCommand::SetSpecialKeyModifierIdCommand(KeyboardLayout* layout, int keyIndex, const QString& newModifiewId, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_layout(layout),
    m_keyIndex(keyIndex),
    m_newModifierId(newModifiewId)
{
    setText(i18n("Set special key modifier ID"));

    SpecialKey* key = qobject_cast<SpecialKey*>(m_layout->key(m_keyIndex));
    m_oldModifierId = key->modifierId();
}

void SetSpecialKeyModifierIdCommand::undo()
{
    SpecialKey* key = qobject_cast<SpecialKey*>(m_layout->key(m_keyIndex));
    key->setModifierId(m_oldModifierId);
}

void SetSpecialKeyModifierIdCommand::redo()
{
    SpecialKey* key = qobject_cast<SpecialKey*>(m_layout->key(m_keyIndex));
    key->setModifierId(m_newModifierId);
}

int SetSpecialKeyModifierIdCommand::id() const
{
    return 0xafabebaf;
}

bool SetSpecialKeyModifierIdCommand::mergeWith(const QUndoCommand* other)
{
    const SetSpecialKeyModifierIdCommand* setSpecialKeyModifierIdCommand = static_cast<const SetSpecialKeyModifierIdCommand*>(other);

    if (m_layout != setSpecialKeyModifierIdCommand->m_layout)
        return false;

    if (m_keyIndex != setSpecialKeyModifierIdCommand->m_keyIndex)
        return false;

    m_newModifierId = setSpecialKeyModifierIdCommand->m_newModifierId;
    return true;
}
