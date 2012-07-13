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

#include "abstractkey.h"

AbstractKey::AbstractKey(QObject *parent) :
    QObject(parent)
{
}

int AbstractKey::left() const
{
    return m_left;
}

void AbstractKey::setLeft(int left)
{
    if (left != m_left)
    {
        m_left = left;
        emit leftChanged();
    }
}

int AbstractKey::top() const
{
    return m_top;
}


void AbstractKey::setTop(int top)
{
    if (top != m_top)
    {
        m_top = top;
        emit topChanged();
    }
}

int AbstractKey::width() const
{
    return m_width;
}

void AbstractKey::setWidth(int width)
{
    Q_ASSERT(width > 0);

    if (width != m_width)
    {
        m_width = width;
        emit widthChanged();
    }
}

int AbstractKey::height() const
{
    return m_height;
}

void AbstractKey::setHeight(int height)
{
    Q_ASSERT(height > 0);

    if (height != m_height)
    {
        m_height = height;
        emit heightChanged();
    }
}

QString AbstractKey::keyType() const
{
    return "abstractKey";
}

void AbstractKey::copyFrom(AbstractKey* source)
{
    setLeft(source->left());
    setTop(source->top());
    setWidth(source->width());
    setHeight(source->height());
}

QRect AbstractKey::rect() const
{
    return QRect(left(), top(), width(), height());
}

void AbstractKey::setRect(const QRect& rect)
{
    setLeft(rect.left());
    setTop(rect.top());
    setWidth(rect.width());
    setHeight(rect.height());
}
