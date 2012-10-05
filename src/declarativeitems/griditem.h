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


#ifndef GRIDITEM_H
#define GRIDITEM_H

#include <QDeclarativeItem>

class GridItem : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(qreal lineDistance READ lineDistance WRITE setLineDistance NOTIFY lineDistanceChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
public:
    explicit GridItem(QDeclarativeItem* parent = 0);
    qreal lineDistance() const;
    void setLineDistance(qreal distance);
    QColor color() const;
    void setColor(const QColor& color);
    QColor backgroundColor() const;
    void setBackgroundColor(const QColor& backgroundColor);
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget* = 0);
signals:
    void lineDistanceChanged();
    void colorChanged();
    void backgroundColorChanged();
private:
    qreal m_lineDistance;
    QColor m_color;
    QColor m_backgroundColor;
};

#endif // GRIDITEM_H
