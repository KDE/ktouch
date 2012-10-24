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

#ifndef GRAPHCOREITEM_H
#define GRAPHCOREITEM_H

#include "graphcore.h"

class LineGraphCore : public GraphCore
{
    Q_OBJECT
    Q_PROPERTY(qreal pointRadius READ pointRadius WRITE setPointRadius NOTIFY pointRadiusChanged)
public:
    explicit LineGraphCore(QDeclarativeItem* parent = 0);
    qreal pointRadius() const;
    void setPointRadius(qreal pointRadius);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
signals:
    void pointRadiusChanged();
private:
    qreal m_pointRadius;
};

#endif // GRAPHCOREITEM_H
