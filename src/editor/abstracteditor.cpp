/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
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

AbstractEditor::~AbstractEditor()
{
    delete m_undoStacks;
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
