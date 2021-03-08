/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */


#ifndef GRIDITEM_H
#define GRIDITEM_H

#include <QQuickPaintedItem>

class GridItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(qreal lineDistance READ lineDistance WRITE setLineDistance NOTIFY lineDistanceChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
public:
    explicit GridItem(QQuickItem* parent = 0);
    qreal lineDistance() const;
    void setLineDistance(qreal distance);
    QColor color() const;
    void setColor(const QColor& color);
    QColor backgroundColor() const;
    void setBackgroundColor(const QColor& backgroundColor);
    void paint(QPainter* painter) override;
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
