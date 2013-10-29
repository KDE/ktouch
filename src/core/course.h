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
class DataIndexCourse;

class Course : public CourseBase
{
    Q_OBJECT
    Q_PROPERTY(DataIndexCourse* associatedDataIndexCourse READ associatedDataIndexCourse WRITE setAssociatedDataIndexCourse NOTIFY associatedDataIndexCourseChanged)
    Q_PROPERTY(int lessonCount READ lessonCount NOTIFY lessonCountChanged)
    Q_PROPERTY(bool doSyncLessonCharacters READ doSyncLessonCharacters WRITE setDoSyncLessonCharacters NOTIFY doSyncLessonCharactersChanged)

public:
    explicit Course(QObject *parent = 0);
    DataIndexCourse* associatedDataIndexCourse() const;
    void setAssociatedDataIndexCourse(DataIndexCourse* dataIndexCourse);
    int lessonCount() const;
    bool doSyncLessonCharacters() const;
    void setDoSyncLessonCharacters(bool doSync);
    void setId(const QString& id);
    void setTitle(const QString& title);
    void setDescription(const QString& description);
    void setKeyboardLayoutName(const QString& keyboardLayoutName);
    Q_INVOKABLE Lesson* lesson(int index) const;
    Q_INVOKABLE void addLesson(Lesson* lesson);
    Q_INVOKABLE void insertLesson(int index, Lesson* lesson);
    Q_INVOKABLE void removeLesson(int index);
    Q_INVOKABLE void clearLessons();
    Q_INVOKABLE void copyFrom(Course* source);

signals:
    void associatedDataIndexCourseChanged();
    void lessonCountChanged();
    void doSyncLessonCharactersChanged();
    void lessonAboutToBeAdded(Lesson* lesson, int index);
    void lessonAdded();
    void lessonsAboutToBeRemoved(int first, int last);
    void lessonsRemoved();

private slots:
    void updateLessonCharacters(int firstIndex = 0);

private:
    Q_DISABLE_COPY(Course)
    DataIndexCourse* m_associatedDataIndexCourse;
    bool m_doSyncLessonCharacters;
    QList<Lesson*> m_lessons;
    QSignalMapper* m_signalMapper;
};

#endif // COURSE_H
