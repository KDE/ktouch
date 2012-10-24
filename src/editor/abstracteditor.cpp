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

#include "abstracteditor.h"

#include <QMap>
#include <QUndoStack>
#include <QUndoGroup>

AbstractEditor::AbstractEditor(QWidget* parent):
    QWidget(parent),
    m_undoGroup(0),
    m_undoStacks(new QMap<QString,QUndoStack*>),
    m_currentUndoStack(0)
{
}

QUndoGroup* AbstractEditor::undoGroup() const
{
    return m_undoGroup;
}

void AbstractEditor::setUndoGroup(QUndoGroup* undoGroup)
{
    m_undoGroup = undoGroup;
}

void AbstractEditor::initUndoStack(const QString& undoStackId)
{
    if (m_undoStacks->contains(undoStackId))
    {
        m_currentUndoStack = m_undoStacks->value(undoStackId);
    }
    else
    {
        m_currentUndoStack = new QUndoStack(this);
        m_undoStacks->insert(undoStackId, m_currentUndoStack);
        undoGroup()->addStack(m_currentUndoStack);
    }

    undoGroup()->setActiveStack(m_currentUndoStack);
}

void AbstractEditor::clearUndoStack(const QString& undoStackId)
{
    if (m_undoStacks->contains(undoStackId))
    {
        m_undoStacks->value(undoStackId)->clear();
    }
}

QUndoStack* AbstractEditor::currentUndoStack() const
{
    return m_currentUndoStack;
}
