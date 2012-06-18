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

#ifndef COURSE_H
#define COURSE_H

#include "coursebase.h"

#include <QString>
#include <QList>

class QSignalMapper;
class Lesson;

class Course : public CourseBase
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(int lessonCount READ lessonCount NOTIFY lessonCountChanged)

public:
    explicit Course(QObject *parent = 0);
    QString id() const;
    void setId(const QString& id);
    int lessonCount() const;
    Q_INVOKABLE Lesson* lesson(int index) const;
    Q_INVOKABLE void addLesson(Lesson* lesson);
    Q_INVOKABLE void insertLesson(int index, Lesson* lesson);
    Q_INVOKABLE void removeLesson(int index);
    Q_INVOKABLE void clearLessons();
    Q_INVOKABLE void copyFrom(Course* source);

signals:
    void idChanged();
    void lessonCountChanged();
    void lessonAboutToBeAdded(Lesson* lesson, int index);
    void lessonAdded();
    void lessonsAboutToBeRemoved(int first, int last);
    void lessonsRemoved();

private slots:
    void updateLessonCharacters(int firstIndex = 0);

private:
    Q_DISABLE_COPY(Course)
    QString m_id;
    QList<Lesson*> m_lessons;
    QSignalMapper* m_signalMapper;
};

#endif // COURSE_H
