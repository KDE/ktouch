/*
 *  Copyright 2013  Sebastian Gottfried <sebastiangottfried@web.de>
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

#ifndef LESSONPAINTER_H
#define LESSONPAINTER_H

#include <QQuickPaintedItem>
#include <QPixmap>

#include <QPointer>

class QTextDocument;
class QTextFrame;

class Lesson;
class TrainingLineCore;
struct LessonPainterPrivate;

class LessonPainter : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(Lesson* lesson READ lesson WRITE setLesson NOTIFY lessonChanged)
    Q_PROPERTY(qreal maximumWidth READ maximumWidth WRITE setMaximumWidth NOTIFY maximumWidthChanged)
    Q_PROPERTY(qreal maximumHeight READ maximumHeight WRITE setMaximumHeight NOTIFY maximumHeightChanged)
    Q_PROPERTY(TrainingLineCore* trainingLineCore READ trainingLineCore WRITE setTrainingLineCore NOTIFY trainingLineCoreChanged)
    Q_PROPERTY(QRectF cursorRectangle READ cursorRectangle NOTIFY cursorRectangleChanged)
public:
    explicit LessonPainter(QQuickItem* parent = 0);
    ~LessonPainter();
    Lesson* lesson() const;
    void setLesson(Lesson* lesson);
    qreal maximumWidth() const;
    void setMaximumWidth(qreal maximumWidth);
    qreal maximumHeight() const;
    void setMaximumHeight(qreal maximumHeight);
    TrainingLineCore* trainingLineCore() const;
    void setTrainingLineCore(TrainingLineCore* trainingLineCore);
    QRectF cursorRectangle() const;
public slots:
    void reset();
signals:
    void lessonChanged();
    void maximumWidthChanged();
    void maximumHeightChanged();
    void trainingLineCoreChanged();
    void cursorRectangleChanged();
    void done();
protected:
    void paint(QPainter* painter);
private slots:
    void updateLayout();
    void resetTrainingStatus();
    void updateTrainingStatus();
    void advanceToNextTrainingLine();
private:
    void updateDoc();
    void invalidateImageCache();
    void checkImageCache();
    void updateCursorRectangle();
    LessonPainterPrivate* d;
    QPointer<Lesson> m_lesson;
    QStringList m_lines;
    QTextDocument* m_doc;
    qreal m_textScale;
    qreal m_maximumWidth;
    qreal m_maximumHeight;
    QPixmap m_imageCache;
    bool m_imageCacheDirty;
    TrainingLineCore* m_trainingLineCore;
    int m_currentLine;
    QPointer<QQuickItem> m_cursorItem;
    QRectF m_cursorRectangle;
};

#endif // LESSONPAINTER_H
