/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
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
    return QStringLiteral("abstractKey");
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
