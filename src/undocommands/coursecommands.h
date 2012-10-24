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


#ifndef COURSECOMMANDS_H
#define COURSECOMMANDS_H

#include <QUndoCommand>

class Course;
class Lesson;

class SetCourseTitleCommand: public QUndoCommand
{
public:
    SetCourseTitleCommand(Course* course, const QString& oldTitle, QUndoCommand* parent = 0);
    void undo();
    void redo();
    bool mergeWith(const QUndoCommand* command);
    int id() const;
private:
    Course* m_course;
    QString m_oldTitle;
    QString m_newTitle;
};

class SetCourseKeyboadLayoutNameCommand: public QUndoCommand
{
public:
    SetCourseKeyboadLayoutNameCommand(Course* course, const QString& oldKeyboardLayoutName, QUndoCommand* parent = 0);
    void undo();
    void redo();
    bool mergeWith(const QUndoCommand* command);
    int id() const;
private:
    Course* m_course;
    QString m_oldKeyboardLayoutName;
    QString m_newKeyboardLayoutName;
};

class SetCourseDescriptionCommand: public QUndoCommand
{
public:
    SetCourseDescriptionCommand(Course* course, const QString& oldDescription, QUndoCommand* parent = 0);
    void undo();
    void redo();
    bool mergeWith(const QUndoCommand* command);
    int id() const;
private:
    Course* m_course;
    QString m_oldDescription;
    QString m_newDescription;
};

class AddLessonCommand: public QUndoCommand
{
public:
    AddLessonCommand(Course* course, int lessonIndex, const QString& lessonTitle, QUndoCommand* parent = 0);
    void undo();
    void redo();
    bool mergeWith(const QUndoCommand* command);
    int id() const;
private:
    Course* m_course;
    int m_lessonIndex;
    QString m_lessonId;
};

class RemoveLessonCommand: public QUndoCommand
{
public:
    RemoveLessonCommand(Course* course, int lessonIndex, QUndoCommand* parent = 0);
    virtual ~RemoveLessonCommand();
    void undo();
    void redo();
    bool mergeWith(const QUndoCommand* command);
    int id() const;
private:
    Course* m_course;
    int m_lessonIndex;
    Lesson* m_backupLesson;
};

class MoveLessonCommand: public QUndoCommand
{
public:
    MoveLessonCommand(Course* course, int oldLessonIndex, int newLessonIndex, QUndoCommand* parent = 0);
    void undo();
    void redo();
    bool mergeWith(const QUndoCommand* command);
    int id() const;
private:
    Course* m_course;
    int m_oldLessonIndex;
    int m_newLessonIndex;
};

class SetLessonTitleCommand: public QUndoCommand
{
public:
    SetLessonTitleCommand(Course* course, int lessonIndex, const QString& oldTitle, QUndoCommand* parent=0);
    void undo();
    void redo();
    bool mergeWith(const QUndoCommand* command);
    int id() const;
private:
    Course* m_course;
    int m_lessonIndex;
    QString m_oldTitle;
    QString m_newTitle;
};

class SetLessonNewCharactersCommand: public QUndoCommand
{
public:
    SetLessonNewCharactersCommand(Course* course, int lessonIndex, const QString& oldNewCharacters, QUndoCommand* parent = 0);
    void undo();
    void redo();
    bool mergeWith(const QUndoCommand* command);
    int id() const;
private:
    Course* m_course;
    int m_lessonIndex;
    QString m_oldNewCharacters;
    QString m_newNewCharacters;
};

class SetLessonTextCommand: public QUndoCommand
{
public:
    SetLessonTextCommand(Course* course, int lessonIndex, const QString& oldText, QUndoCommand* parent = 0);
    void undo();
    void redo();
    bool mergeWith(const QUndoCommand* command);
    int id() const;
private:
    Course* m_course;
    int m_lessonIndex;
    QString m_oldText;
    QString m_newText;
};

#endif // COURSECOMMANDS_H
