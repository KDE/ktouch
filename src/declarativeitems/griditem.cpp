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

#include "griditem.h"

#include <QPainter>

GridItem::GridItem(QDeclarativeItem* parent) :
    QDeclarativeItem(parent),
    m_lineDistance(10.0),
    m_color(Qt::black),
    m_backgroundColor(Qt::white)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
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

void GridItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget *)
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
