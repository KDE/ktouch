/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "griditem.h"

#include <QPainter>

GridItem::GridItem(QQuickItem* parent) :
    QQuickPaintedItem(parent),
    m_lineDistance(10.0),
    m_color(Qt::black),
    m_backgroundColor(Qt::white)
{
    setFlag(QQuickItem::QQuickItem::ItemHasContents, true);
}

qreal GridItem::lineDistance() const
{
    return m_lineDistance;
}

void GridItem::setLineDistance(qreal distance)
{
    if (distance != m_lineDistance)
    {
        m_lineDistance = distance;
        emit lineDistanceChanged();
    }
}

QColor GridItem::color() const
{
    return m_color;
}

void GridItem::setColor(const QColor& color)
{
    if (color != m_color)
    {
        m_color = color;
        emit colorChanged();
        update();
    }
}

QColor GridItem::backgroundColor() const
{
    return m_backgroundColor;
}

void GridItem::setBackgroundColor(const QColor& backgroundColor)
{
    if (backgroundColor != m_backgroundColor)
    {
        m_backgroundColor = backgroundColor;
        emit backgroundColorChanged();
    }
}

void GridItem::paint(QPainter* painter)
{
    const QRect rect = boundingRect().toRect();

    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(m_backgroundColor));

    painter->drawRect(rect);

    painter->setPen(m_color);

    for (qreal x = rect.left(); x < rect.right(); x += m_lineDistance)
    {
        const int roundedX = qRound(x);
        painter->drawLine(roundedX, rect.top(), roundedX, rect.bottom());
    }

    for (qreal y = rect.top(); y < rect.bottom(); y += m_lineDistance)
    {
        const int roundedY = qRound(y);
        painter->drawLine(rect.left(), roundedY, rect.right(), roundedY);
    }
}
