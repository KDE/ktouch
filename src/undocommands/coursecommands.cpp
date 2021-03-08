/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */


#include "coursecommands.h"

#include <KLocalizedString>

#include "core/course.h"
#include "core/lesson.h"

SetCourseTitleCommand::SetCourseTitleCommand(Course* course, const QString& oldTitle, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_course(course),
    m_oldTitle(oldTitle),
    m_newTitle(course->title())
{
    setText(i18n("Set title"));
}

void SetCourseTitleCommand::undo()
{
    m_course->setTitle(m_oldTitle);
}

void SetCourseTitleCommand::redo()
{
    m_course->setTitle(m_newTitle);
}

bool SetCourseTitleCommand::mergeWith(const QUndoCommand* command)
{
    const SetCourseTitleCommand* setCourseTitleCommand = static_cast<const SetCourseTitleCommand*>(command);

    if (m_course != setCourseTitleCommand->m_course)
        return false;

    m_newTitle = setCourseTitleCommand->m_newTitle;
    return true;
}

int SetCourseTitleCommand::id() const
{
    return 0x9a2e787f;
}

SetCourseKeyboadLayoutNameCommand::SetCourseKeyboadLayoutNameCommand(Course* course, const QString& oldKeyboardLayoutName, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_course(course),
    m_oldKeyboardLayoutName(oldKeyboardLayoutName),
    m_newKeyboardLayoutName(course->keyboardLayoutName())
{
    setText(i18n("Set keyboard layout"));
}

void SetCourseKeyboadLayoutNameCommand::undo()
{
    m_course->setKeyboardLayoutName(m_oldKeyboardLayoutName);
}

void SetCourseKeyboadLayoutNameCommand::redo()
{
    m_course->setKeyboardLayoutName(m_newKeyboardLayoutName);
}

bool SetCourseKeyboadLayoutNameCommand::mergeWith(const QUndoCommand* command)
{
    const SetCourseKeyboadLayoutNameCommand* setCourseKeyboardLayoutNameCommand = static_cast<const SetCourseKeyboadLayoutNameCommand*>(command);

    if (m_course != setCourseKeyboardLayoutNameCommand->m_course)
        return false;

    m_newKeyboardLayoutName = setCourseKeyboardLayoutNameCommand->m_newKeyboardLayoutName;
    return true;
}

int SetCourseKeyboadLayoutNameCommand::id() const
{
    return 0xc6c1d878;
}

SetCourseDescriptionCommand::SetCourseDescriptionCommand(Course* course, const QString& oldDescription, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_course(course),
    m_oldDescription(oldDescription),
    m_newDescription(course->description())
{
    setText(i18n("Edit description"));
}

void SetCourseDescriptionCommand::undo()
{
    m_course->setDescription(m_oldDescription);
}

void SetCourseDescriptionCommand::redo()
{
    m_course->setDescription(m_newDescription);
}

bool SetCourseDescriptionCommand::mergeWith(const QUndoCommand* command)
{
    const SetCourseDescriptionCommand* setCourseDescriptionCommand = static_cast<const SetCourseDescriptionCommand*>(command);

    if (m_course != setCourseDescriptionCommand->m_course)
        return false;

    m_newDescription = setCourseDescriptionCommand->m_newDescription;
    return true;
}

int SetCourseDescriptionCommand::id() const
{
    return 0x264f63fb;
}

AddLessonCommand::AddLessonCommand(Course* course, int lessonIndex,const QString& lessonId, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_course(course),
    m_lessonIndex(lessonIndex),
    m_lessonId(lessonId)
{
    setText(i18n("Add lesson"));
}

void AddLessonCommand::undo()
{
    m_course->removeLesson(m_lessonIndex);
}

void AddLessonCommand::redo()
{
    Lesson* lesson = new Lesson();

    lesson->setId(m_lessonId);

    if (m_lessonIndex == m_course->lessonCount())
    {
        m_course->addLesson(lesson);
    }
    else
    {
        m_course->insertLesson(m_lessonIndex, lesson);
    }
}

bool AddLessonCommand::mergeWith(const QUndoCommand* command)
{
    Q_UNUSED(command);
    return false;
}

int AddLessonCommand::id() const
{
    return 0x34f95ddf;
}

RemoveLessonCommand::RemoveLessonCommand(Course* course, int lessonIndex, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_course(course),
    m_lessonIndex(lessonIndex),
    m_backupLesson(0)
{
    setText(i18n("Remove lesson"));
}

RemoveLessonCommand::~RemoveLessonCommand()
{
    if (m_backupLesson)
    {
        delete m_backupLesson;
    }
}

void RemoveLessonCommand::undo()
{
    Q_ASSERT(m_backupLesson);

    if (m_lessonIndex == m_course->lessonCount())
    {
        m_course->addLesson(m_backupLesson);
    }
    else
    {
        m_course->insertLesson(m_lessonIndex, m_backupLesson);
    }

    m_backupLesson = 0;
}

void RemoveLessonCommand::redo()
{
    m_backupLesson = new Lesson();
    m_backupLesson->copyFrom(m_course->lesson(m_lessonIndex));
    m_course->removeLesson(m_lessonIndex);
}

bool RemoveLessonCommand::mergeWith(const QUndoCommand* command)
{
    Q_UNUSED(command)
    return false;
}

int RemoveLessonCommand::id() const
{
    return 0x36f2ef42;
}

MoveLessonCommand::MoveLessonCommand(Course* course, int oldLessonIndex, int newLessonIndex, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_course(course),
    m_oldLessonIndex(oldLessonIndex),
    m_newLessonIndex(newLessonIndex)
{
    setText(i18n("Move lesson"));
}

void MoveLessonCommand::undo()
{
    Lesson* lesson = new Lesson();

    lesson->copyFrom(m_course->lesson(m_newLessonIndex));
    m_course->removeLesson(m_newLessonIndex);

    if (m_oldLessonIndex == m_course->lessonCount())
    {
        m_course->addLesson(lesson);
    }
    else
    {
        m_course->insertLesson(m_oldLessonIndex, lesson);
    }

}

void MoveLessonCommand::redo()
{
    Lesson* lesson = new Lesson();

    lesson->copyFrom(m_course->lesson(m_oldLessonIndex));
    m_course->removeLesson(m_oldLessonIndex);

    if (m_newLessonIndex == m_course->lessonCount())
    {
        m_course->addLesson(lesson);
    }
    else
    {
        m_course->insertLesson(m_newLessonIndex, lesson);
    }
}

bool MoveLessonCommand::mergeWith(const QUndoCommand* command)
{
    const MoveLessonCommand* moveLessonCommand = static_cast<const MoveLessonCommand*>(command);

    if (m_course != moveLessonCommand->m_course)
        return false;

    if (m_newLessonIndex != moveLessonCommand->m_oldLessonIndex)
        return false;

    m_newLessonIndex = moveLessonCommand->m_newLessonIndex;
    return true;
}

int MoveLessonCommand::id() const
{
    return 0xf3993bad;
}

SetLessonTitleCommand::SetLessonTitleCommand(Course* course, int lessonIndex, const QString& oldTitle, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_course(course),
    m_lessonIndex(lessonIndex),
    m_oldTitle(oldTitle),
    m_newTitle(course->lesson(lessonIndex)->title())
{
    setText(i18n("Set lesson title"));
}

void SetLessonTitleCommand::undo()
{
    m_course->lesson(m_lessonIndex)->setTitle(m_oldTitle);
}

void SetLessonTitleCommand::redo()
{
    m_course->lesson(m_lessonIndex)->setTitle(m_newTitle);
}

bool SetLessonTitleCommand::mergeWith(const QUndoCommand* command)
{
    const SetLessonTitleCommand* setLessonTitleCommand = static_cast<const SetLessonTitleCommand*>(command);

    if (m_course != setLessonTitleCommand->m_course)
        return false;

    if (m_lessonIndex != setLessonTitleCommand->m_lessonIndex)
        return false;

    m_newTitle = setLessonTitleCommand->m_newTitle;
    return true;
}

int SetLessonTitleCommand::id() const
{
    return 0x4499bef5;
}

SetLessonNewCharactersCommand::SetLessonNewCharactersCommand(Course* course, int lessonIndex, const QString& oldNewCharacters, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_course(course),
    m_lessonIndex(lessonIndex),
    m_oldNewCharacters(oldNewCharacters),
    m_newNewCharacters(course->lesson(lessonIndex)->newCharacters())
{
    setText(i18n("Set new characters for lesson"));
}

void SetLessonNewCharactersCommand::undo()
{
    m_course->lesson(m_lessonIndex)->setNewCharacters(m_oldNewCharacters);
}

void SetLessonNewCharactersCommand::redo()
{
    m_course->lesson(m_lessonIndex)->setNewCharacters(m_newNewCharacters);
}

bool SetLessonNewCharactersCommand::mergeWith(const QUndoCommand* command)
{
    const SetLessonNewCharactersCommand* setLessonNewCharactersCommand = static_cast<const SetLessonNewCharactersCommand*>(command);

    if (m_course != setLessonNewCharactersCommand->m_course)
        return false;

    if (m_lessonIndex != setLessonNewCharactersCommand->m_lessonIndex)
        return false;

    m_newNewCharacters = setLessonNewCharactersCommand->m_newNewCharacters;
    return true;
}

int SetLessonNewCharactersCommand::id() const
{
    return 0x325e1dc2;
}

SetLessonTextCommand::SetLessonTextCommand(Course* course, int lessonIndex, const QString& oldText, QUndoCommand* parent) :
    QUndoCommand(parent),
    m_course(course),
    m_lessonIndex(lessonIndex),
    m_oldText(oldText),
    m_newText(course->lesson(lessonIndex)->text())
{
    setText(i18n("Set lesson text"));
}

void SetLessonTextCommand::undo()
{
    m_course->lesson(m_lessonIndex)->setText(m_oldText);
}

void SetLessonTextCommand::redo()
{
    m_course->lesson(m_lessonIndex)->setText(m_newText);
}

bool SetLessonTextCommand::mergeWith(const QUndoCommand* command)
{
    const SetLessonTextCommand* setLessonTextCommand = static_cast<const SetLessonTextCommand*>(command);

    if (m_course != setLessonTextCommand->m_course)
        return false;

    if (m_lessonIndex != setLessonTextCommand->m_lessonIndex)
        return false;

    m_newText = setLessonTextCommand->m_newText;
    return true;
}

int SetLessonTextCommand::id() const
{
    return 0xdea53874;
}
