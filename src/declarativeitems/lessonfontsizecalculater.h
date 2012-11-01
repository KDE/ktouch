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

#ifndef LESSONFONTSIZECALCULATER_H
#define LESSONFONTSIZECALCULATER_H

#include <QObject>

class Lesson;

class LessonFontSizeCalculater : public QObject
{
    Q_OBJECT
    Q_ENUMS(PointSizes)
    Q_PROPERTY(qreal targetWidth READ targetWidth WRITE setTargetWidth NOTIFY targetWidthChanged)
    Q_PROPERTY(Lesson* lesson READ lesson WRITE setLesson NOTIFY lessonChanged)
    Q_PROPERTY(qreal scale READ scale NOTIFY updated)
    Q_PROPERTY(qreal charWidth READ charWidth NOTIFY updated)
public:
    enum PointSizes{
        BasePixelSize = 20
    };
    explicit LessonFontSizeCalculater(QObject *parent = 0);
    qreal targetWidth() const;
    void setTargetWidth(qreal targetWidth);
    Lesson* lesson() const;
    void setLesson(Lesson* lesson);
    qreal scale() const;
    qreal charWidth() const;
signals:
    void targetWidthChanged();
    void lessonChanged();
    void updated();
private:
    void update();
    qreal m_targetWidth;
    Lesson* m_lesson;
    qreal m_scale;
    qreal m_charWidth;
};

#endif // LESSONFONTSIZECALCULATER_H
