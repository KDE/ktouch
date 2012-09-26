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
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(int precision READ precision WRITE setPrecision NOTIFY precisionChanged)
    Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)
    Q_PROPERTY(qreal unitFactor READ unitFactor WRITE setUnitFactor NOTIFY unitFactorChanged)

public:
    explicit Dimension(QDeclarativeItem* parent = 0);
    QColor color() const;
    void setColor(const QColor& color);
    int dataColumn() const;
    void setDataColumn(int dataColumn);
    qreal maximumValue() const;
    void setMaximumValue(qreal maximumValue);
    QString label() const;
    void setLabel(const QString& label);
    int precision() const;
    void setPrecision(int precision);
    QString unit() const;
    void setUnit(const QString& unit);
    qreal unitFactor() const;
    void setUnitFactor(qreal unitFactor);
    Q_INVOKABLE QString formatValue(qreal value);
signals:
    void colorChanged();
    void dataColumnChanged();
    void maximumValueChanged();
    void labelChanged();
    void precisionChanged();
    void unitChanged();
    void unitFactorChanged();
    void updated();
private:
    QColor m_color;
    int m_dataColumn;
    qreal m_maximumValue;
    QString m_label;
    int m_precision;
    QString m_unit;
    qreal m_unitFactor;
};

#endif // DIMENSION_H
