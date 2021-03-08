/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */


#ifndef COURSEEDITOR_H
#define COURSEEDITOR_H

#include "editor/abstracteditor.h"
#include "ui_courseeditor.h"

#include "undocommands/coursecommands.h"

class DataIndexCourse;
class Course;
class Lesson;
class LessonModel;

class CourseEditor : public AbstractEditor, private Ui::CourseEditor
{
    Q_OBJECT
public:
    explicit CourseEditor(QWidget* parent = 0);
    ~CourseEditor();
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
