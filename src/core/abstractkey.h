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
#include <QRect>

class AbstractKey : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int left READ left WRITE setLeft NOTIFY leftChanged)
    Q_PROPERTY(int top READ top WRITE setTop NOTIFY topChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)

public:
    explicit AbstractKey(QObject *parent = 0);

    Q_INVOKABLE virtual QString keyType() const;
    int left() const;
    void setLeft(int left);
    int top() const;
    void setTop(int top);
    int width() const;
    void setWidth(int width);
    int height() const;
    void setHeight(int height);
    Q_INVOKABLE void copyFrom(AbstractKey* source);

    QRect rect() const;
    void setRect(const QRect& rect);

signals:
    void leftChanged();
    void topChanged();
    void widthChanged();
    void heightChanged();

private:
    int m_left;
    int m_top;
    int m_width;
    int m_height;
};

#endif // ABSTRACTKEY_H
