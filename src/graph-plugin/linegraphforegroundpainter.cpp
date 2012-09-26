/*
 * Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "linegraphforegroundpainter.h"

#include <QPainter>

#include "linegraphcore.h"

LineGraphForegroundPainter::LineGraphForegroundPainter(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

LineGraphCore* LineGraphForegroundPainter::lineGraphCore() const
{
    return m_lineGraphCore;
}

void LineGraphForegroundPainter::setGraphCoreItem(LineGraphCore* lineGraphCore)
{
    if (lineGraphCore != m_lineGraphCore)
    {
        if (m_lineGraphCore)
        {
            m_lineGraphCore->disconnect(this);
        }

        m_lineGraphCore = lineGraphCore;

        if (m_lineGraphCore)
        {
            connect(m_lineGraphCore, SIGNAL(updated()), SLOT(triggerUpdate()));
        }

        update();
        emit lineGraphCoreChanged();
    }
}

QColor LineGraphForegroundPainter::backgroundColor() const
{
    return m_backgroundColor;
}

void LineGraphForegroundPainter::setBackgroundColor(const QColor& backgroundColor)
{
    if (backgroundColor != m_backgroundColor)
    {
        m_backgroundColor = backgroundColor;
        triggerUpdate();
        emit backgroundColorChanged();
    }
}

void LineGraphForegroundPainter::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    if (!m_lineGraphCore)
        return;

    const qreal overlapWidth = m_lineGraphCore->pitch() / 2;

    QColor startColor = m_backgroundColor;
    QColor stopColor = startColor;
    stopColor.setAlpha(0);

    QLinearGradient leftFadeOutGradiend(QPointF(0, 0), QPointF(overlapWidth, 0));
    leftFadeOutGradiend.setColorAt(0, startColor);
    leftFadeOutGradiend.setColorAt(1, stopColor);
    painter->fillRect(QRectF(QPointF(0, 0), QPointF(overlapWidth, height())), QBrush(leftFadeOutGradiend));

    QLinearGradient rightFadeOutGradiend(QPointF(width() - overlapWidth , 0), QPointF(width(), 0));
    rightFadeOutGradiend.setColorAt(0, stopColor);
    rightFadeOutGradiend.setColorAt(1, startColor);
    painter->fillRect(QRectF(QPointF(width() - overlapWidth , 0), QPointF(width(), height())), QBrush(rightFadeOutGradiend));

}

void LineGraphForegroundPainter::triggerUpdate()
{
    if (!m_lineGraphCore)
        return;

    update();
}
