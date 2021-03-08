/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */


#include "lessonmodel.h"

#include <KLocalizedString>

#include <core/course.h>
#include <core/lesson.h>

LessonModel::LessonModel(QObject* parent) :
    QAbstractListModel(parent),
    m_course(0)
{
}

Course* LessonModel::course() const
{
    return m_course;
}

void LessonModel::setCourse(Course* course)
{
    if (m_course != course)
    {
        beginResetModel();

        if (m_course)
        {
            m_course->disconnect(this);
        }

        m_course = course;

        if (m_course)
        {
            connect(m_course, &Course::lessonAboutToBeAdded, this, &LessonModel::onLessonAboutToBeAdded);
            connect(m_course, &Course::lessonAdded, this, &LessonModel::onLessonAdded);
            connect(m_course, &Course::lessonsAboutToBeRemoved, this, &LessonModel::onLessonsAboutToBeRemoved);
            connect(m_course, &Course::lessonsRemoved, this, &LessonModel::onLessonsRemoved);
        }

        endResetModel();

        emit courseChanged();
    }
}

QVariant LessonModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_course->lessonCount())
        return QVariant();

    Lesson* const lesson = m_course->lesson(index.row());

    switch(role)
    {
        case Qt::DisplayRole:
            return !lesson->title().isEmpty()?
                QVariant(lesson->title()): QVariant(i18n("<No title>"));
        case Qt::ToolTipRole:
            return QVariant(i18n("<p>New characters: %1</p><p>%2</p>", lesson->newCharacters(), lesson->text()));
        case LessonModel::DataRole:
            return QVariant::fromValue<QObject*>(lesson);
        default:
            return QVariant();
    }
}

int LessonModel::rowCount(const QModelIndex& parent) const
{
    if (!m_course)
        return 0;

    if (parent.isValid())
        return 0;

    return m_course->lessonCount();
}

void LessonModel::onLessonAboutToBeAdded(Lesson* lesson, int index)
{
    connect(lesson, &Lesson::titleChanged, this, [=] { emitLessonChanged(index); });
    connect(lesson, &Lesson::newCharactersChanged, this, [=] { emitLessonChanged(index); });
    connect(lesson, &Lesson::textChanged, this, [=] { emitLessonChanged(index); });
    beginInsertRows(QModelIndex(), index, index);
}

void LessonModel::onLessonAdded()
{
    updateMappings();
    endInsertRows();
}

void LessonModel::onLessonsAboutToBeRemoved(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
}

void LessonModel::onLessonsRemoved()
{
    endRemoveRows();
}

void LessonModel::emitLessonChanged(int row)
{
    emit lessonChanged(row);
    emit dataChanged(index(row, 0), index(row, 0));
}


QVariant LessonModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Vertical)
        return QVariant(section + 1);
    return QVariant(i18n("Title"));
}

QHash<int, QByteArray> LessonModel::roleNames() const
{
    QHash<int, QByteArray> names = QAbstractItemModel::roleNames();
    names.insert(LessonModel::DataRole, "dataRole");
    return names;
}

void LessonModel::updateMappings()
{
    for (int i = 0; i < m_course->lessonCount(); i++)
    {
        disconnect(m_course->lesson(i), &Lesson::titleChanged, this, nullptr);
        disconnect(m_course->lesson(i), &Lesson::newCharactersChanged, this, nullptr);
        disconnect(m_course->lesson(i), &Lesson::textChanged, this, nullptr);
        connect(m_course->lesson(i), &Lesson::titleChanged, this, [=] { emitLessonChanged(i); });
        connect(m_course->lesson(i), &Lesson::newCharactersChanged, this, [=] { emitLessonChanged(i); });
        connect(m_course->lesson(i), &Lesson::textChanged, this, [=] { emitLessonChanged(i); });
    }
}
