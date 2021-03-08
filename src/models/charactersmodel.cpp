/*
 * SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "charactersmodel.h"

#include <KLocalizedString>

#include "core/keyboardlayout.h"
#include "core/key.h"
#include "core/keychar.h"
#include "undocommands/keyboardlayoutcommands.h"

CharactersModel::CharactersModel(QObject *parent) :
    QAbstractTableModel(parent),
    m_keyboardLayout(nullptr),
    m_keyIndex(-1),
    m_key(nullptr),
    m_undoStack(nullptr)
{
}

KeyboardLayout* CharactersModel::keyboardLayout() const
{
    return m_keyboardLayout;
}

void CharactersModel::setKeyboardLayout(KeyboardLayout* keyboardLayout)
{
    if (keyboardLayout != m_keyboardLayout)
    {
        m_keyboardLayout = keyboardLayout;
        setKeyIndex(-1);
        emit keyboardLayoutChanged();
    }
}

int CharactersModel::keyIndex() const
{
    return m_keyIndex;
}

void CharactersModel::setKeyIndex(int keyIndex)
{
    if (!m_keyboardLayout)
        return;

    Key* key = nullptr;

    if (keyIndex != -1)
    {
        key = qobject_cast<Key*>(m_keyboardLayout->key(keyIndex));

        if (!key)
            keyIndex = -1;
    }

    if (keyIndex != m_keyIndex)
    {
        beginResetModel();

        if (m_key)
        {
            m_key->disconnect(this);

            for (int i = 0; i < m_key->keyCharCount(); i++)
            {
                KeyChar* keyChar = m_key->keyChar(i);
                keyChar->disconnect(this);
            }
        }

        m_keyIndex = keyIndex;
        m_key = key;

        if (m_key)
        {
            connect(m_key, &Key::keyCharAboutToBeAdded, this, &CharactersModel::onKeyCharAboutToBeAdded);
            connect(m_key, &Key::keyCharAdded, this, &CharactersModel::onKeyCharAdded);
            connect(m_key, &Key::keyCharsAboutToBeRemoved, this, &CharactersModel::onKeyCharsAboutToBeRemoved);
            connect(m_key, &Key::keyCharsRemoved, this, &CharactersModel::onKeyCharsRemoved);

            for (int i = 0; i < m_key->keyCharCount(); i++)
            {
                KeyChar* keyChar = m_key->keyChar(i);
                connect(keyChar, &KeyChar::valueChanged, this, [=] { emitCharacterChanged(i); });
                connect(keyChar, &KeyChar::modifierChanged, this, [=] { emitCharacterChanged(i); });
                connect(keyChar, &KeyChar::positionChanged, this, [=] { emitCharacterChanged(i); });
            }
        }

        endResetModel();
    }
}

QUndoStack* CharactersModel::undoStack() const
{
    return m_undoStack;
}

void CharactersModel::setUndoStack(QUndoStack *undoStack)
{
    m_undoStack = undoStack;
}

Qt::ItemFlags CharactersModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant CharactersModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_key->keyCharCount())
        return QVariant();

    KeyChar* keyChar = m_key->keyChar(index.row());

    switch (index.column())
    {
    case 0:
        return characterData(keyChar, role);
    case 1:
        return modifierIdData(keyChar, role);
    case 2:
        return positionData(keyChar, role);
    default:
        return QVariant();
    }
}

bool CharactersModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid())
        return false;

    if (index.row() >= m_key->keyCharCount())
        return false;

    if (role != Qt::EditRole)
        return false;

    if (!m_undoStack)
        return false;

    KeyChar* keyChar = m_key->keyChar(index.row());
    QUndoCommand* cmd = nullptr;

    switch (index.column())
    {
    case 0:
        if (value.toString().length() != 1)
            return false;
        if (value.toString().at(0) == keyChar->value())
            return false;
        cmd = new SetKeyCharValueCommand(m_keyboardLayout, m_keyIndex, index.row(), value.toString().at(0));
        undoStack()->push(cmd);
        return true;
    case 1:
        if (value.toString() == keyChar->modifier())
            return false;
        cmd = new SetKeyCharModifierCommand(m_keyboardLayout, m_keyIndex, index.row(), value.toString());
        undoStack()->push(cmd);
        return true;
    case 2:
        if (static_cast<KeyChar::Position>(value.toInt()) == keyChar->position())
            return false;
        cmd = new SetKeyCharPositionCommand(m_keyboardLayout, m_keyIndex, index.row(), static_cast<KeyChar::Position>(value.toInt()));
        undoStack()->push(cmd);
        return true;
    }

    return false;
}

QVariant CharactersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Vertical)
        return QVariant(section + 1);

    switch (section)
    {
    case 0:
        return QVariant(i18n("Character"));
    case 1:
        return QVariant(i18n("Modifier ID"));
    case 2:
        return QVariant(i18n("Position"));
    default:
        return QVariant();
    }
}

int CharactersModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return 3;
}

int CharactersModel::rowCount(const QModelIndex& parent) const
{
    if (!m_key)
        return 0;

    if (parent.isValid())
        return 0;

    return m_key->keyCharCount();
}

void CharactersModel::onKeyCharAboutToBeAdded(KeyChar* keyChar, int index)
{
    connect(keyChar, &KeyChar::valueChanged, this, [=] { emitCharacterChanged(index); });
    connect(keyChar, &KeyChar::modifierChanged, this, [=] { emitCharacterChanged(index); });
    connect(keyChar, &KeyChar::positionChanged, this, [=] { emitCharacterChanged(index); });
    beginInsertRows(QModelIndex(), index, index);
}

void CharactersModel::onKeyCharAdded()
{
    updateMappings();
    endInsertRows();
}

void CharactersModel::onKeyCharsAboutToBeRemoved(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
}

void CharactersModel::onKeyCharsRemoved()
{
    endRemoveRows();
}

void CharactersModel::emitCharacterChanged(int row)
{
    emit dataChanged(index(row, 0), index(row, columnCount() - 1));
}

void CharactersModel::updateMappings()
{
    for (int i = 0; i < m_key->keyCharCount(); i++)
    {
        disconnect(m_key->keyChar(i), &KeyChar::valueChanged, this, nullptr);
        disconnect(m_key->keyChar(i), &KeyChar::modifierChanged, this, nullptr);
        disconnect(m_key->keyChar(i), &KeyChar::positionChanged, this, nullptr);
        connect(m_key->keyChar(i), &KeyChar::valueChanged, this, [=] { emitCharacterChanged(i); });
        connect(m_key->keyChar(i), &KeyChar::modifierChanged, this, [=] { emitCharacterChanged(i); });
        connect(m_key->keyChar(i), &KeyChar::positionChanged, this, [=] { emitCharacterChanged(i); });
    }
}

QVariant CharactersModel::characterData(KeyChar* keyChar, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return keyChar->value();
    default:
        return QVariant();
    }
}

QVariant CharactersModel::modifierIdData(KeyChar *keyChar, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return keyChar->modifier();
    default:
        return QVariant();
    }
}

QVariant CharactersModel::positionData(KeyChar *keyChar, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
        switch (keyChar->position())
        {
        case KeyChar::TopLeft:
            return i18n("Top left");
        case KeyChar::TopRight:
            return i18n("Top right");
        case KeyChar::BottomLeft:
            return i18n("Bottom left");
        case KeyChar::BottomRight:
            return i18n("Bottom right");
        case KeyChar::Hidden:
            return i18n("Hidden");
        }
        return QVariant();
    case Qt::EditRole:
        return QVariant(keyChar->position());
    default:
        return QVariant();
    }
}
