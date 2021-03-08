/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef ABSTRACTEDITOR_H
#define ABSTRACTEDITOR_H

#include <QWidget>

class QUndoGroup;
class QUndoStack;

class AbstractEditor : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractEditor(QWidget* parent = 0);
    ~AbstractEditor();
    QUndoGroup* undoGroup() const;
    void setUndoGroup(QUndoGroup* undoGroup);
protected:
    void initUndoStack(const QString& undoStackId);
    void clearUndoStack(const QString& undoStackId);
    QUndoStack* currentUndoStack() const;
private:
    QUndoGroup* m_undoGroup;
    QMap<QString,QUndoStack*>* m_undoStacks;
    QUndoStack* m_currentUndoStack;
};

#endif // ABSTRACTEDITOR_H
