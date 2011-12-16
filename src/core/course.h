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
    bool loadXML(QIODevice* dev);

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
    static QXmlSchema* xmlSchema();
    bool parseXML(QDomDocument* doc);
    static QXmlSchema* s_schema;
    QList<Lesson*> m_lessons;
};

#endif // COURSE_H
