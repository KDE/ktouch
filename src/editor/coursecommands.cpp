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


#include "coursecommands.h"

#include <KLocale>

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
