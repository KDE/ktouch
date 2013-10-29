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

#include "editor/abstracteditor.h"
#include "ui_courseeditor.h"

#include "undocommands/coursecommands.h"

class QUndoStack;
class DataIndexCourse;
class Course;
class Lesson;
class LessonModel;
class LessonTextHighlighter;

class CourseEditor : public AbstractEditor, private Ui::CourseEditor
{
    Q_OBJECT
public:
    explicit CourseEditor(QWidget* parent = 0);
    void setResourceModel(ResourceModel* model);
    void openCourse(DataIndexCourse* dataIndexCourse);
    void clearUndoStackForCourse(DataIndexCourse* course);
    void save();
private slots:
    void setTitle(const QString& newTitle);
    void setKeyboardLayoutName(const QString& newName);
    void setDescription(const QString& newDescription);
    void addLesson();
    void removeLesson();
    void moveLessonUp();
    void moveLessonDown();
    void setLessonTitle(const QString& newTitle);
    void setLessonNewCharacters(const QString& newCharacters);
    void setLessonText(const QString& text);
    void updateTitle();
    void updateKeyboardLayoutName();
    void updateDescription();
    void updateLessonTitle();
    void updateLessonNewCharacters();
    void updateLessonText();
    void updateLessonCharacters();
    void onKeyboardLayoutChosen();
    void onDescriptionChanged();
    void onLessonTextChanged();
    void selectLesson(int index);
    void onLessonSelected();
private:
    void setIsReadOnly(bool readOnly);
    DataIndexCourse* m_dataIndexCourse;
    Course* m_course;
    int m_currentLessonIndex;
    Lesson* m_currentLesson;
    LessonModel* m_lessonModel;
    bool m_readOnly;
};

#endif // COURSEEDITOR_H
