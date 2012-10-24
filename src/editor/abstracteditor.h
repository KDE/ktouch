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

#ifndef ABSTRACTEDITOR_H
#define ABSTRACTEDITOR_H

#include <QWidget>

class QUndoGroup;
class QUndoStack;
class ResourceModel;

class AbstractEditor : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractEditor(QWidget* parent = 0);
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
