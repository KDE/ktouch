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
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(int lessonCount READ lessonCount NOTIFY lessonCountChanged)

public:
    explicit Course(QObject *parent = 0);
    QString id() const;
    void setId(const QString& id);
    int lessonCount() const;
    Q_INVOKABLE Lesson* lesson(int index) const;
    Q_INVOKABLE void addLesson(Lesson* lesson);
    Q_INVOKABLE void removeLesson(int index);
    Q_INVOKABLE void clearLessons();
    Q_INVOKABLE void copyFrom(Course* source);

signals:
    void idChanged();
    void lessonCountChanged();

private:
    Q_DISABLE_COPY(Course)
    bool parseXML(QDomDocument* doc);
    QString m_id;
    QList<Lesson*> m_lessons;
};

#endif // COURSE_H
