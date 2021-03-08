/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "scalebackgrounditem.h"

#include <QPainter>
#include <QPainterPath>

ScaleBackgroundItem::ScaleBackgroundItem(QQuickItem* parent) :
    QQuickPaintedItem(parent),
    m_startAngle(0),
    m_stopAngle(0),
    m_scaleMarkHeight(0)
{
    setFlag(QQuickItem::ItemHasContents, true);
}

qreal ScaleBackgroundItem::startAngle() const
{
    return m_startAngle;
}

void ScaleBackgroundItem::setStartAngle(qreal angle)
{
    if (angle != m_startAngle)
    {
        m_startAngle = angle;
        emit startAngleChanged();
        update();
    }
}

qreal ScaleBackgroundItem::stopAngle() const
{
    return m_stopAngle;
}

void ScaleBackgroundItem::setStopAngle(qreal angle)
{
    if (angle != m_stopAngle)
    {
        m_stopAngle = angle;
        emit stopAngleChanged();
        update();
    }
}

qreal ScaleBackgroundItem::scaleMarkHeight() const
{
    return m_scaleMarkHeight;
}

void ScaleBackgroundItem::setScaleMarkHeight(qreal height)
{
    if (height != m_scaleMarkHeight)
    {
        m_scaleMarkHeight = height;
        emit scaleMarkHeightChanged();
        update();
    }
}

QColor ScaleBackgroundItem::color() const
{
    return m_color;
}

void ScaleBackgroundItem::setColor(const QColor& color)
{
    if (color != m_color)
    {
        m_color = color;
        emit colorChanged();
        update();
    }
}

void ScaleBackgroundItem::paint(QPainter* painter)
{
    QBrush brush(m_color);
    painter->setPen(Qt::NoPen);
    painter->setBrush(brush);
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    const qreal sweepLength = m_stopAngle - m_startAngle;
    path.arcMoveTo(boundingRect(), m_startAngle);
    path.arcTo(boundingRect(), m_startAngle, sweepLength);
    QRectF innerRect = boundingRect();
    innerRect.adjust(m_scaleMarkHeight, m_scaleMarkHeight, -m_scaleMarkHeight, -m_scaleMarkHeight);
    path.arcTo(innerRect, m_stopAngle, -sweepLength);
    painter->drawPath(path);
}
