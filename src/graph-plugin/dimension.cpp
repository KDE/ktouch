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

#include "dimension.h"

Dimension::Dimension(QDeclarativeItem* parent) :
    QDeclarativeItem(parent),
    m_color(Qt::black),
    m_dataColumn(0),
    m_maximumValue(256)
{
}

QColor Dimension::color() const
{
    return m_color;
}

void Dimension::setColor(const QColor &color)
{
    if (color != m_color)
    {
        m_color = color;
        emit colorChanged();
    }
}

int Dimension::dataColumn() const
{
    return m_dataColumn;
}

void Dimension::setDataColumn(int dataColumn)
{
    if (dataColumn != m_dataColumn)
    {
        m_dataColumn = dataColumn;
        emit dataColumnChanged();
    }
}

qreal Dimension::maximumValue() const
{
    return m_maximumValue;
}

void Dimension::setMaximumValue(qreal maximumValue)
{
    if (maximumValue != m_maximumValue)
    {
        m_maximumValue = maximumValue;
        emit maximumValueChanged();
    }
}
