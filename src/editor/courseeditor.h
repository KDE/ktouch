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


#ifndef COURSEEDITOR_H
#define COURSEEDITOR_H

#include <QWidget>
#include "ui_courseeditor.h"

#include "editor/coursecommands.h"

class QUndoGroup;
class QUndoStack;
class DataIndexCourse;
class Course;
class ResourceModel;
class LessonModel;

class CourseEditor : public QWidget, private Ui::CourseEditor
{
    Q_OBJECT
public:
    explicit CourseEditor(QWidget* parent = 0);
    void setResourceModel(ResourceModel* model);
    void setUndoGroup(QUndoGroup* undoGroup);
    void openCourse(DataIndexCourse* dataIndexCourse);
    void save();
private slots:
    void setTitle(const QString& newTitle);
    void setKeyboardLayoutName(const QString& newName);
    void setDescription(const QString& newDescription);
    void updateTitle();
    void updateKeyboardLayoutName();
    void updateDescription();
    void onKeyboardLayoutChosen();
    void onDescriptionChanged();
private:
    void setIsReadOnly(bool readOnly);
    DataIndexCourse* m_dataIndexCourse;
    Course* m_course;
    LessonModel* m_lessonModel;
    QUndoGroup* m_undoGroup;
    QMap<QString,QUndoStack*>* m_undoStacks;
    QUndoStack* m_currentUndoStack;
};

#endif // COURSEEDITOR_H
