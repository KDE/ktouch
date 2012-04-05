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

class QXmlSchema;
class QDomDocument;
class QIODevice;
class Lesson;

class Course : public CourseBase
{
    Q_OBJECT
    Q_PROPERTY(int lessonCount READ lessonCount NOTIFY lessonCountChanged)

public:
    explicit Course(QObject *parent = 0);

    int lessonCount() const
    {
        return m_lessons.count();
    }

    Q_INVOKABLE Lesson* lesson(unsigned int index) const;
    Q_INVOKABLE void addLesson(Lesson* lesson);
    Q_INVOKABLE void removeLesson(unsigned int index);
    Q_INVOKABLE void clearLessons();

signals:
    void lessonCountChanged(int newLessonCount);

private:
    bool parseXML(QDomDocument* doc);
    QList<Lesson*> m_lessons;
};

#endif // COURSE_H
