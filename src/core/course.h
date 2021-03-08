/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef COURSE_H
#define COURSE_H

#include "coursebase.h"

#include <QString>
#include <QList>

class Lesson;
class DataIndexCourse;

class Course : public CourseBase
{
    Q_OBJECT
    Q_PROPERTY(DataIndexCourse* associatedDataIndexCourse READ associatedDataIndexCourse WRITE setAssociatedDataIndexCourse NOTIFY associatedDataIndexCourseChanged)
    Q_PROPERTY(int lessonCount READ lessonCount NOTIFY lessonCountChanged)
    Q_PROPERTY(Kind kind READ kind WRITE setKind NOTIFY kindChanged)

public:
    enum Kind {
        SequentialCourse,
        LessonCollection
    };
    Q_ENUM(Kind)

    explicit Course(QObject *parent = 0);
    DataIndexCourse* associatedDataIndexCourse() const;
    void setAssociatedDataIndexCourse(DataIndexCourse* dataIndexCourse);
    int lessonCount() const;
    Kind kind() const;
    void setKind(Kind kind);
    void setId(const QString& id);
    void setTitle(const QString& title);
    void setDescription(const QString& description);
    void setKeyboardLayoutName(const QString& keyboardLayoutName);
    Q_INVOKABLE Lesson* lesson(int index) const;
    Q_INVOKABLE void addLesson(Lesson* lesson);
    Q_INVOKABLE void insertLesson(int index, Lesson* lesson);
    Q_INVOKABLE void removeLesson(int index);
    Q_INVOKABLE int indexOfLesson(Lesson* lesson);
    Q_INVOKABLE void clearLessons();
    Q_INVOKABLE void copyFrom(Course* source);

signals:
    void associatedDataIndexCourseChanged();
    void lessonCountChanged();
    void kindChanged();
    void lessonAboutToBeAdded(Lesson* lesson, int index);
    void lessonAdded();
    void lessonsAboutToBeRemoved(int first, int last);
    void lessonsRemoved();

private slots:
    void updateLessonCharacters(int firstIndex = 0);

private:
    Q_DISABLE_COPY(Course)
    DataIndexCourse* m_associatedDataIndexCourse;
    Kind m_kind;
    QList<Lesson*> m_lessons;
};

#endif // COURSE_H
