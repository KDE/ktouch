#include "course.h"

#include <QFile>
#include <QUrl>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QXmlSchema>
#include <QXmlSchemaValidator>

#include <kstandarddirs.h>
#include <klocale.h>
#include <kdebug.h>

#include "lesson.h"

Course::Course(QObject *parent) :
    CourseBase(parent)
{
}

Lesson* Course::lesson(unsigned int index) const
{
    Q_ASSERT(index < m_lessons.count());
    return m_lessons.at(index);
}

void Course::addLesson(Lesson* lesson)
{
    m_lessons.append(lesson);
    lesson->setParent(this);
    emit lessonCountChanged(m_lessons.count());
}

void Course::removeLesson(unsigned int index)
{
    Q_ASSERT(index < m_lessons.count());
    delete m_lessons.at(index);
    m_lessons.removeAt(index);
    emit lessonCountChanged(m_lessons.count());
}

void Course::clearLessons()
{
    qDeleteAll(m_lessons);
    m_lessons.clear();
    emit lessonCountChanged(m_lessons.count());
}
