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

#include "scalebackgrounditem.h"

#include <QPainter>
#include <QPainterPath>

ScaleBackgroundItem::ScaleBackgroundItem(QDeclarativeItem* parent) :
    QDeclarativeItem(parent),
    m_startAngle(0),
    m_stopAngle(0),
    m_scaleMarkHeight(0)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
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

void ScaleBackgroundItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
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
