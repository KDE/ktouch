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

#ifndef ABSTRACTKEY_H
#define ABSTRACTKEY_H

#include <QObject>

class AbstractKey : public QObject
{
    Q_OBJECT
    Q_PROPERTY(unsigned int left READ left WRITE setLeft NOTIFY leftChanged)
    Q_PROPERTY(unsigned int top READ top WRITE setTop NOTIFY topChanged)
    Q_PROPERTY(unsigned int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(unsigned int height READ height WRITE setHeight NOTIFY heightChanged)

public:
    explicit AbstractKey(QObject *parent = 0);

    Q_INVOKABLE virtual QString keyType() const {
        return "abstractKey";
    }

    unsigned int left() const
    {
        return m_left;
    }

    void setLeft(unsigned int left)
    {
        if (left != m_left)
        {
            m_left = left;
            emit leftChanged(left);
        }
    }

    unsigned int top() const
    {
        return m_top;
    }

    void setTop(unsigned int top)
    {
        if (top != m_top)
        {
            m_top = top;
            emit topChanged(top);
        }
    }

    int width() const
    {
        return m_width;
    }

    void setWidth(unsigned int width)
    {
        if (width != m_width)
        {
            m_width = width;
            emit widthChanged(width);
        }
    }

    int height() const
    {
        return m_height;
    }

    void setHeight(unsigned int height)
    {
        if (height != m_height)
        {
            m_height = height;
            emit heightChanged(height);
        }
    }

signals:
    void leftChanged(unsigned int newLeft);
    void topChanged(unsigned int newTop);
    void widthChanged(unsigned int newWidth);
    void heightChanged(unsigned int newHeight);

private:
    unsigned int m_left;
    unsigned int m_top;
    unsigned int m_width;
    unsigned int m_height;
};

#endif // ABSTRACTKEY_H
