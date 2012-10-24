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

#include "linegraphcore.h"

LineGraphCore::LineGraphCore(QDeclarativeItem* parent) :
    GraphCore(parent),
    m_pointRadius(5.0)
{
}

qreal LineGraphCore::pointRadius() const
{
    return m_pointRadius;
}

void LineGraphCore::setPointRadius(qreal pointRadius)
{
    if (pointRadius != m_pointRadius)
    {
        m_pointRadius = pointRadius;
        triggerUpdate();
        emit pointRadiusChanged();
    }
}

void LineGraphCore::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    paintAxisAndLines(painter, m_pointRadius);
}
