#include "lesson.h"

#include "lessonline.h"

Lesson::Lesson(QObject *parent) :
    QObject(parent)
{
}

LessonLine* Lesson::line(unsigned int index) const
{
    Q_ASSERT(index < m_lines.count());
    return m_lines.at(index);
}

void Lesson::addLine(LessonLine* line)
{
    m_lines.append(line);
    emit lineCountChanged(m_lines.count());
}

void Lesson::removeLine(unsigned int index)
{
    Q_ASSERT(index < m_lines.count());
    delete m_lines.at(index);
    m_lines.removeAt(index);
    emit lineCountChanged(m_lines.count());
}

void Lesson::clearLines()
{
    qDeleteAll(m_lines);
    m_lines.clear();
    emit lineCountChanged(m_lines.count());
}
