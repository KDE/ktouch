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

#include "lessonfontsizecalculater.h"

#include <QFont>
#include <QFontMetricsF>
#include <QStringList>

#include "core/lesson.h"

LessonFontSizeCalculater::LessonFontSizeCalculater(QObject* parent) :
    QObject(parent),
    m_targetWidth(0),
    m_lesson(0),
    m_scale(1)
{
}

qreal LessonFontSizeCalculater::targetWidth() const
{
    return m_targetWidth;
}

void LessonFontSizeCalculater::setTargetWidth(qreal targetWidth)
{
    if (targetWidth != m_targetWidth)
    {
        m_targetWidth = targetWidth;
        emit targetWidthChanged();

        update();
    }
}

Lesson* LessonFontSizeCalculater::lesson() const
{
    return m_lesson;
}

void LessonFontSizeCalculater::setLesson(Lesson* lesson)
{
    if (lesson != m_lesson)
    {
        m_lesson = lesson;
        emit lessonChanged();

        update();
    }
}

qreal LessonFontSizeCalculater::scale() const
{
    return m_scale;
}

qreal LessonFontSizeCalculater::charWidth() const
{
    return m_charWidth;
}

void LessonFontSizeCalculater::update()
{
    if (!m_lesson || m_targetWidth == 0)
    {
        m_scale = 1;
        emit updated();
        return;
    }

    const QStringList lines = m_lesson->text().split('\n');
    int maxLength = 0;

    foreach(const QString& line, lines)
    {
        maxLength = qMax(maxLength, line.length());
    }

    if (maxLength == 0)
    {
        m_scale = 1;
        emit updated();
        return;
    }

    QFont font("monospace");
    font.setPixelSize(LessonFontSizeCalculater::BasePixelSize);

    QFontMetricsF fontMetrics(font);

    const qreal charWidth = fontMetrics.maxWidth();

    m_scale = m_targetWidth / maxLength / charWidth;
    m_charWidth = charWidth * m_scale;

    emit updated();
}
