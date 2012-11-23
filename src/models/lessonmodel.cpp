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


#include "lessonmodel.h"

#include <QSignalMapper>

#include <KLocale>

#include <core/course.h>
#include <core/lesson.h>

LessonModel::LessonModel(QObject* parent) :
    QAbstractListModel(parent),
    m_course(0),
    m_signalMapper(new QSignalMapper(this))
{
    connect(m_signalMapper, SIGNAL(mapped(int)), SLOT(emitLessonChanged(int)));
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
            connect(m_course, SIGNAL(lessonAboutToBeAdded(Lesson*,int)), SLOT(onLessonAboutToBeAdded(Lesson*,int)));
            connect(m_course, SIGNAL(lessonAdded()), SLOT(onLessonAdded()));
            connect(m_course, SIGNAL(lessonsAboutToBeRemoved(int,int)), SLOT(onLessonsAboutToBeRemoved(int,int)));
            connect(m_course, SIGNAL(lessonsRemoved()), SLOT(onLessonsRemoved()));
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
    connect(lesson, SIGNAL(titleChanged()), m_signalMapper, SLOT(map()));
    connect(lesson, SIGNAL(newCharactersChanged()), m_signalMapper, SLOT(map()));
    connect(lesson, SIGNAL(textChanged()), m_signalMapper, SLOT(map()));
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

void LessonModel::updateMappings()
{
    for (int i = 0; i < m_course->lessonCount(); i++)
    {
        m_signalMapper->setMapping(m_course->lesson(i), i);
    }
}
