/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef SCALEBACKGROUNDITEM_H
#define SCALEBACKGROUNDITEM_H

#include <QQuickPaintedItem>

class ScaleBackgroundItem: public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(qreal startAngle READ startAngle WRITE setStartAngle NOTIFY startAngleChanged)
    Q_PROPERTY(qreal stopAngle READ stopAngle WRITE setStopAngle NOTIFY stopAngleChanged)
    Q_PROPERTY(qreal scaleMarkHeight READ scaleMarkHeight WRITE setScaleMarkHeight NOTIFY scaleMarkHeightChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit ScaleBackgroundItem(QQuickItem* parent = 0);
    qreal startAngle() const;
    void setStartAngle(qreal angle);
    qreal stopAngle() const;
    void setStopAngle(qreal angle);
    qreal scaleMarkHeight() const;
    void setScaleMarkHeight(qreal height);
    QColor color() const;
    void setColor(const QColor& color);
    void paint(QPainter* painter) override;
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
