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

#ifndef DIMENSION_H
#define DIMENSION_H

#include <QDeclarativeItem>

#include <QColor>
#include <QDeclarativeListProperty>

class Dimension : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(int dataColumn READ dataColumn WRITE setDataColumn NOTIFY dataColumnChanged)
    Q_PROPERTY(qreal maximumValue READ maximumValue WRITE setMaximumValue NOTIFY maximumValueChanged)
public:
    explicit Dimension(QDeclarativeItem* parent = 0);
    QColor color() const;
    void setColor(const QColor& color);
    int dataColumn() const;
    void setDataColumn(int dataColumn);
    qreal maximumValue() const;
    void setMaximumValue(qreal maximumValue);
signals:
    void colorChanged();
    void dataColumnChanged();
    void maximumValueChanged();
private:
    QColor m_color;
    int m_dataColumn;
    qreal m_maximumValue;
};

#endif // DIMENSION_H
