/*
 *  SPDX-FileCopyrightText: 2013 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "lessonpainter.h"

#include <qmath.h>
#include <QAbstractTextDocumentLayout>
#include <QPainter>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextFrame>

#include "core/lesson.h"
#include "declarativeitems/traininglinecore.h"

struct LessonPainterPrivate
{
    LessonPainterPrivate()
    {
        blockFormat.setLineHeight(200, QTextBlockFormat::ProportionalHeight);

        textCharFormat.setFontFamily(QStringLiteral("monospace"));
        textCharFormat.setFontPointSize(10);
        textCharFormat.setForeground(QColor("#000"));
        textCharFormat.setFontHintingPreference(QFont::PreferVerticalHinting);

        placeHolderCharFormat = textCharFormat;
        placeHolderCharFormat.setForeground(QColor("#888"));

        errorCharFormat = textCharFormat;
        errorCharFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
        errorCharFormat.setUnderlineColor(QColor("#f00"));

        preeditCharFromat = textCharFormat;
        preeditCharFromat.setBackground(QColor("#d0d0d0"));

        titleCharFormat = textCharFormat;
        titleCharFormat.setFontFamily(QStringLiteral("sans-serif"));
        titleCharFormat.setFontPointSize(15);
    }

    QTextBlockFormat blockFormat;

    QTextCharFormat textCharFormat;
    QTextCharFormat placeHolderCharFormat;
    QTextCharFormat errorCharFormat;
    QTextCharFormat preeditCharFromat;
    QTextCharFormat titleCharFormat;
};

LessonPainter::LessonPainter(QQuickItem* parent) :
    QQuickPaintedItem(parent),
    d(new LessonPainterPrivate()),
    m_doc(new QTextDocument(this)),
    m_textScale(1.0),
    m_maximumWidth(0),
    m_maximumHeight(-1),
    m_imageCacheDirty(false),
    m_trainingLineCore(0),
    m_currentLine(0)
{
    this->setFlag(QQuickPaintedItem::ItemHasContents, true);
    m_doc->setUseDesignMetrics(true);
}

LessonPainter::~LessonPainter()
{
    delete d;
}

Lesson* LessonPainter::lesson() const
{
    return m_lesson;
}

void LessonPainter::setLesson(Lesson* lesson)
{
    if (lesson != m_lesson)
    {
        if (m_lesson)
        {
            m_lesson->disconnect(this);
        }

        m_lesson = lesson;

        if (m_lesson)
        {
            connect(m_lesson.data(), &Lesson::titleChanged, this, &LessonPainter::reset);
            connect(m_lesson.data(), &Lesson::textChanged, this, &LessonPainter::reset);
        }

        reset();

        emit lessonChanged();
    }
}

qreal LessonPainter::maximumWidth() const
{
    return m_maximumWidth;
}

void LessonPainter::setMaximumWidth(qreal maximumWidth)
{
    if (maximumWidth != m_maximumWidth)
    {
        m_maximumWidth = maximumWidth;
        emit maximumWidthChanged();
        updateLayout();
    }
}

qreal LessonPainter::maximumHeight() const
{
    return m_maximumHeight;
}

void LessonPainter::setMaximumHeight(qreal maximumHeight)
{
    if (maximumHeight != m_maximumHeight)
    {
        m_maximumHeight = maximumHeight;
        emit maximumHeightChanged();
        updateLayout();
    }
}

TrainingLineCore* LessonPainter::trainingLineCore() const
{
    return m_trainingLineCore;
}

void LessonPainter::setTrainingLineCore(TrainingLineCore* trainingLineCore)
{
    if (trainingLineCore != m_trainingLineCore)
    {
        if (m_trainingLineCore)
        {
            m_trainingLineCore->disconnect(this);
        }

        m_trainingLineCore = trainingLineCore;
        emit trainingLineCoreChanged();

        if (m_trainingLineCore)
        {
            connect(m_trainingLineCore, &TrainingLineCore::actualLineChanged, this, &LessonPainter::updateTrainingStatus);
            connect(m_trainingLineCore, &TrainingLineCore::preeditStringChanged, this, &LessonPainter::updateTrainingStatus);
            connect(m_trainingLineCore, &TrainingLineCore::done, this, &LessonPainter::advanceToNextTrainingLine);
        }
    }
}

QRectF LessonPainter::cursorRectangle() const
{
    return m_cursorRectangle;
}

void LessonPainter::reset()
{
    m_lines = m_lesson? m_lesson->text().split('\n'): QStringList();
    updateDoc();
    resetTrainingStatus();
}

void LessonPainter::paint(QPainter* painter)
{
    if (m_imageCacheDirty)
    {
        const auto device = painter->device();
        QImage img(QSize(qFloor(device->width()), qFloor(device->height())), QImage::Format_ARGB32_Premultiplied);
        img.setDevicePixelRatio(img.width() / width());
        img.fill(Qt::transparent);
        QPainter painter(&img);
        painter.scale(m_textScale, m_textScale);
        m_doc->drawContents(&painter);
        m_imageCache = img;
        m_imageCacheDirty = false;
    }

    painter->drawImage(0, 0, m_imageCache);
}

void LessonPainter::updateLayout()
{
    invalidateImageCache();

    if (!m_lesson)
    {
        setWidth(0);
        setHeight(0);
        return;
    }

    // ### reset text width from previous run
    m_doc->setTextWidth(-1);

    const qreal docWidth = m_doc->idealWidth();
    const qreal docHeight = m_doc->size().height();

    m_textScale = m_maximumHeight != -1?
                qMin(m_maximumWidth / docWidth, m_maximumHeight / docHeight):
                m_maximumWidth / docWidth;

    // ### without this text alignment won't work
    m_doc->setTextWidth(docWidth);

    setWidth(qCeil(docWidth * m_textScale));
    setHeight(qCeil(docHeight * m_textScale));

    updateCursorRectangle();
}

void LessonPainter::resetTrainingStatus()
{
    if (!m_trainingLineCore || m_lines.length() == 0)
        return;

    m_trainingLineCore->reset();
    m_currentLine = 0;
    m_trainingLineCore->setReferenceLine(m_lines[0]);
}

void LessonPainter::updateTrainingStatus()
{
    if (m_currentLine >= m_lines.length())
        return;

    QTextCursor cursor(m_doc);

    const QString referenceLine = m_trainingLineCore->referenceLine();
    const QString actualLine = m_trainingLineCore->actualLine();
    const QString preeditString = m_trainingLineCore->preeditString();
    const QTextBlock block = m_doc->findBlockByNumber(m_currentLine + 1);
    const int blockPosition = block.position();

    for (int linePos = 0; linePos < referenceLine.length(); linePos++)
    {
        const bool typed = linePos < actualLine.length();
        const bool preedit = !typed &&  linePos - actualLine.length() < preeditString.length();
        const bool correct = typed && actualLine.at(linePos) == referenceLine.at(linePos);

        const QTextCharFormat charFormat = typed?
                    (correct? d->textCharFormat: d->errorCharFormat):
                    (preedit? d->preeditCharFromat: d->placeHolderCharFormat);

        const QChar displayedChar = typed?
                    actualLine.at(linePos):
                    preedit? preeditString.at(linePos - actualLine.length()): referenceLine.at(linePos);

        const int charPosition = blockPosition + linePos;

        cursor.setPosition(charPosition, QTextCursor::MoveAnchor);
        cursor.setPosition(charPosition + 1, QTextCursor::KeepAnchor);
        cursor.deleteChar();
        cursor.insertText(QString(displayedChar), charFormat);
    }

    invalidateImageCache();
    updateCursorRectangle();
    update();
}

void LessonPainter::advanceToNextTrainingLine()
{
    m_currentLine++;

    if (m_currentLine < m_lines.length())
    {
        m_trainingLineCore->setReferenceLine(m_lines.at(m_currentLine));
    }
    else
    {
        m_trainingLineCore->setReferenceLine(QString());
        emit done();
    }
}

void LessonPainter::updateDoc()
{
    m_doc->clear();

    if (!m_lesson) {
        updateLayout();
        return;
    }

    m_doc->setDocumentMargin(20.0);

    QTextCursor cursor(m_doc);
    QTextBlockFormat blockFormat = d->blockFormat;

    const QString lessonTitle = m_lesson->title();

    blockFormat.setAlignment(lessonTitle.isRightToLeft()? Qt::AlignRight: Qt::AlignLeft);
    cursor.setBlockFormat(blockFormat);
    cursor.setBlockCharFormat(d->titleCharFormat);
    cursor.insertText(lessonTitle);

    const QTextCharFormat textFormat = m_trainingLineCore? d->placeHolderCharFormat: d->textCharFormat;

    foreach (const QString& line, m_lines)
    {
        blockFormat.setAlignment(line.isRightToLeft()? Qt::AlignRight: Qt::AlignLeft);
        cursor.insertBlock(d->blockFormat, textFormat);
        cursor.insertText(line);
    }


    updateLayout();
}

void LessonPainter::invalidateImageCache()
{
    m_imageCacheDirty = true;
    m_imageCache = QImage();
}

void LessonPainter::checkImageCache()
{
    if (!m_imageCacheDirty)
        return;

    QImage img(QSize(qFloor(width()), qFloor(height())), QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::transparent);
    QPainter painter(&img);
    painter.scale(m_textScale, m_textScale);
    m_doc->drawContents(&painter);

    m_imageCache = img;
    m_imageCacheDirty = false;
}

void LessonPainter::updateCursorRectangle()
{
    if (!m_trainingLineCore || m_lines.length() == 0 || m_currentLine >= m_lines.length())
        return;

    const QString actualLine = m_trainingLineCore->actualLine();
    const QString preeditString = m_trainingLineCore->preeditString();
    const QTextBlock block = m_doc->findBlockByNumber(m_currentLine + 1);
    const QAbstractTextDocumentLayout* docLayout = m_doc->documentLayout();
    const QTextLayout* layout = block.layout();
    const QPointF blockPos = docLayout->blockBoundingRect(block).topLeft();
    const int relCursorPos = actualLine.length() + preeditString.length();
    const QTextLine line = layout->lineForTextPosition(relCursorPos);

    m_cursorRectangle = QRectF(
                m_textScale * (blockPos.x() + line.cursorToX(relCursorPos)),
                m_textScale * (blockPos.y() + line.y()),
                1,
                m_textScale * (line.height()));

    emit cursorRectangleChanged();
}
