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

#include "keyboardlayoutcommands.h"

#include <core/keyboardlayout.h>
#include <core/abstractkey.h>

SetKeyboardLayoutTitleCommand::SetKeyboardLayoutTitleCommand(KeyboardLayout* layout, const QString& newTitle, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_layout(layout),
    m_oldTitle(layout->title()),
    m_newTitle(newTitle)
{
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

SetKeyGeometryCommand::SetKeyGeometryCommand(KeyboardLayout* layout, int keyIndex, const QRect& newRect, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_layout(layout),
    m_keyIndex(keyIndex),
    m_oldRect(layout->key(keyIndex)->rect()),
    m_newRect(newRect)
{
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
