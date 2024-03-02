/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "course.h"

#include "lesson.h"
#include "dataindex.h"

Course::Course(QObject *parent) :
    CourseBase(parent),
    m_associatedDataIndexCourse(nullptr),
    m_kind(Course::SequentialCourse)
{
}

DataIndexCourse* Course::associatedDataIndexCourse() const
{
    return m_associatedDataIndexCourse;
}

void Course::setAssociatedDataIndexCourse(DataIndexCourse* dataIndexCourse)
{
    if (dataIndexCourse != m_associatedDataIndexCourse)
    {
        m_associatedDataIndexCourse = dataIndexCourse;
        Q_EMIT associatedDataIndexCourseChanged();
    }
}

Course::Kind Course::kind() const
{
    return m_kind;
}

void Course::setKind(Kind kind)
{
    if (kind != m_kind)
    {
        m_kind = kind;
        Q_EMIT kindChanged();
    }
}

void Course::setId(const QString& id)
{
    CourseBase::setId(id);

    if (m_associatedDataIndexCourse)
    {
        m_associatedDataIndexCourse->setId(id);
    }
}

void Course::setTitle(const QString &title)
{
    CourseBase::setTitle(title);

    if (m_associatedDataIndexCourse)
    {
        m_associatedDataIndexCourse->setTitle(title);
    }
}

void Course::setDescription(const QString &description)
{
    CourseBase::setDescription(description);

    if (m_associatedDataIndexCourse)
    {
        m_associatedDataIndexCourse->setDescription(description);
    }
}

void Course::setKeyboardLayoutName(const QString &keyboardLayoutName)
{
    CourseBase::setKeyboardLayoutName(keyboardLayoutName);

    if (m_associatedDataIndexCourse)
    {
        m_associatedDataIndexCourse->setKeyboardLayoutName(keyboardLayoutName);
    }
}

int Course::lessonCount() const
{
    return m_lessons.count();
}

Lesson* Course::lesson(int index) const
{
    Q_ASSERT(index >= 0 && index < m_lessons.count());
    return m_lessons.at(index);
}

void Course::addLesson(Lesson* lesson)
{
    Q_EMIT lessonAboutToBeAdded(lesson, m_lessons.length());
    m_lessons.append(lesson);
    lesson->setParent(this);
    const int index = m_lessons.length() - 1;
    updateLessonCharacters(index);
    connect(lesson, &Lesson::newCharactersChanged, this, [=] { updateLessonCharacters(index); });
    Q_EMIT lessonCountChanged();
    Q_EMIT lessonAdded();
}

void Course::insertLesson(int index, Lesson* lesson)
{
    Q_ASSERT(index >= 0 && index < m_lessons.count());
    Q_EMIT lessonAboutToBeAdded(lesson, index);
    m_lessons.insert(index, lesson);
    lesson->setParent(this);
    updateLessonCharacters(index);
    connect(lesson, &Lesson::newCharactersChanged, this, [=] { updateLessonCharacters(index); });
    Q_EMIT lessonCountChanged();
    Q_EMIT lessonAdded();
}

void Course::removeLesson(int index)
{
    Q_ASSERT(index >= 0 && index < m_lessons.count());
    Q_EMIT lessonsAboutToBeRemoved(index, index);
    Lesson* const lesson = m_lessons.at(index);
    m_lessons.removeAt(index);
    delete lesson;
    updateLessonCharacters(index);
    Q_EMIT lessonCountChanged();
    Q_EMIT lessonsRemoved();
}

int Course::indexOfLesson(Lesson* lesson)
{
    return m_lessons.indexOf(lesson);
}

void Course::clearLessons()
{
    if (m_lessons.isEmpty())
        return;

    Q_EMIT lessonsAboutToBeRemoved(0, m_lessons.length() - 1);
    qDeleteAll(m_lessons);
    m_lessons.clear();
    Q_EMIT lessonCountChanged();
    Q_EMIT lessonsRemoved();
}

void Course::copyFrom(Course* source)
{
    setIsValid(false);
    setId(source->id());
    setTitle(source->title());
    setDescription(source->description());
    setKeyboardLayoutName(source->keyboardLayoutName());
    setKind(source->kind());
    clearLessons();
    for (int i = 0; i < source->lessonCount(); i++)
    {
        Lesson* lesson = new Lesson(this);
        lesson->copyFrom(source->lesson(i));
        addLesson(lesson);
    }
    setIsValid(true);
}

void Course::updateLessonCharacters(int firstIndex)
{
    if (m_kind == Course::LessonCollection)
    {
        return;
    }

    QString characters = firstIndex > 0? lesson(firstIndex - 1)->characters(): QLatin1String("");

    for (int i = firstIndex; i < lessonCount(); i++)
    {
        Lesson* const lesson = this->lesson(i);
        const QString newChars = lesson->newCharacters();
        for (int j = 0; j < newChars.length(); j++)
        {
            const QChar newChar = newChars.at(j);
            if (!characters.contains(newChar))
                characters.append(newChar);
        }
        lesson->setCharacters(characters);
    }
}

#include "moc_course.cpp"
