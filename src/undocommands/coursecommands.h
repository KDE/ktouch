/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */


#ifndef COURSECOMMANDS_H
#define COURSECOMMANDS_H

#include <QUndoCommand>

class Course;
class Lesson;

class SetCourseTitleCommand: public QUndoCommand
{
public:
    SetCourseTitleCommand(Course* course, const QString& oldTitle, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    bool mergeWith(const QUndoCommand* command) override;
    int id() const override;
private:
    Course* m_course;
    QString m_oldTitle;
    QString m_newTitle;
};

class SetCourseKeyboadLayoutNameCommand: public QUndoCommand
{
public:
    SetCourseKeyboadLayoutNameCommand(Course* course, const QString& oldKeyboardLayoutName, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    bool mergeWith(const QUndoCommand* command) override;
    int id() const override;
private:
    Course* m_course;
    QString m_oldKeyboardLayoutName;
    QString m_newKeyboardLayoutName;
};

class SetCourseDescriptionCommand: public QUndoCommand
{
public:
    SetCourseDescriptionCommand(Course* course, const QString& oldDescription, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    bool mergeWith(const QUndoCommand* command) override;
    int id() const override;
private:
    Course* m_course;
    QString m_oldDescription;
    QString m_newDescription;
};

class AddLessonCommand: public QUndoCommand
{
public:
    AddLessonCommand(Course* course, int lessonIndex, const QString& lessonTitle, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    bool mergeWith(const QUndoCommand* command) override;
    int id() const override;
private:
    Course* m_course;
    int m_lessonIndex;
    QString m_lessonId;
};

class RemoveLessonCommand: public QUndoCommand
{
public:
    RemoveLessonCommand(Course* course, int lessonIndex, QUndoCommand* parent = nullptr);
    virtual ~RemoveLessonCommand();
    void undo() override;
    void redo() override;
    bool mergeWith(const QUndoCommand* command) override;
    int id() const override;
private:
    Course* m_course;
    int m_lessonIndex;
    Lesson* m_backupLesson;
};

class MoveLessonCommand: public QUndoCommand
{
public:
    MoveLessonCommand(Course* course, int oldLessonIndex, int newLessonIndex, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    bool mergeWith(const QUndoCommand* command) override;
    int id() const override;
private:
    Course* m_course;
    int m_oldLessonIndex;
    int m_newLessonIndex;
};

class SetLessonTitleCommand: public QUndoCommand
{
public:
    SetLessonTitleCommand(Course* course, int lessonIndex, const QString& oldTitle, QUndoCommand* parent=0);
    void undo() override;
    void redo() override;
    bool mergeWith(const QUndoCommand* command) override;
    int id() const override;
private:
    Course* m_course;
    int m_lessonIndex;
    QString m_oldTitle;
    QString m_newTitle;
};

class SetLessonNewCharactersCommand: public QUndoCommand
{
public:
    SetLessonNewCharactersCommand(Course* course, int lessonIndex, const QString& oldNewCharacters, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    bool mergeWith(const QUndoCommand* command) override;
    int id() const override;
private:
    Course* m_course;
    int m_lessonIndex;
    QString m_oldNewCharacters;
    QString m_newNewCharacters;
};

class SetLessonTextCommand: public QUndoCommand
{
public:
    SetLessonTextCommand(Course* course, int lessonIndex, const QString& oldText, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    bool mergeWith(const QUndoCommand* command) override;
    int id() const override;
private:
    Course* m_course;
    int m_lessonIndex;
    QString m_oldText;
    QString m_newText;
};

#endif // COURSECOMMANDS_H
