/*
 *  SPDX-FileCopyrightText: 2013 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef LESSONPAINTER_H
#define LESSONPAINTER_H

#include <QQuickPaintedItem>
#include <QImage>

#include <QPointer>

class QTextDocument;

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
    void paint(QPainter* painter) override;
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
    QImage m_imageCache;
    bool m_imageCacheDirty;
    TrainingLineCore* m_trainingLineCore;
    int m_currentLine;
    QPointer<QQuickItem> m_cursorItem;
    QRectF m_cursorRectangle;
};

#endif // LESSONPAINTER_H
