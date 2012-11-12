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

#ifndef SCALEBACKGROUNDITEM_H
#define SCALEBACKGROUNDITEM_H

#include <QDeclarativeItem>

class ScaleBackgroundItem: public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(qreal startAngle READ startAngle WRITE setStartAngle NOTIFY startAngleChanged)
    Q_PROPERTY(qreal stopAngle READ stopAngle WRITE setStopAngle NOTIFY stopAngleChanged)
    Q_PROPERTY(qreal scaleMarkHeight READ scaleMarkHeight WRITE setScaleMarkHeight NOTIFY scaleMarkHeightChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit ScaleBackgroundItem(QDeclarativeItem* parent = 0);
    qreal startAngle() const;
    void setStartAngle(qreal angle);
    qreal stopAngle() const;
    void setStopAngle(qreal angle);
    qreal scaleMarkHeight() const;
    void setScaleMarkHeight(qreal height);
    QColor color() const;
    void setColor(const QColor& color);
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* , QWidget* = 0);
signals:
    void startAngleChanged();
    void stopAngleChanged();
    void scaleMarkHeightChanged();
    void colorChanged();
private:
    qreal m_startAngle;
    qreal m_stopAngle;
    qreal m_scaleMarkHeight;
    QColor m_color;
};

#endif // SCALEBACKGROUNDITEM_H
