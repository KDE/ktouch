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
