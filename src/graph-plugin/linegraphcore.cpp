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

#include "linegraphcore.h"

#include <QAbstractTableModel>

#include <QPainter>

LineGraphCore::LineGraphCore(QDeclarativeItem* parent) :
    QDeclarativeItem(parent),
    m_model(0),
    m_graphStyle(LineGraphCore::Lines),
    m_pitch(50.0),
    m_pointRadius(5.0)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

QAbstractTableModel* LineGraphCore::model() const
{
    return m_model;
}

void LineGraphCore::setModel(QAbstractTableModel* model)
{
    if (model != m_model)
    {
        if (m_model)
        {
            m_model->disconnect(this);
        }

        m_model = model;

        if (m_model)
        {
            connect(m_model, SIGNAL(modelReset()), SLOT(triggerUpdate()));
            connect(m_model, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(triggerUpdate()));
            connect(m_model, SIGNAL(rowsRemoved(QModelIndex,int,int)), SLOT(triggerUpdate()));
            connect(m_model, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), SLOT(triggerUpdate()));
            connect(m_model, SIGNAL(layoutChanged()), SLOT(triggerUpdate()));
            connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(triggerUpdate()));
        }

        triggerUpdate();
        emit modelChanged();
    }
}

QDeclarativeListProperty<Dimension> LineGraphCore::dimensions() {
    return QDeclarativeListProperty<Dimension>(this, 0, &LineGraphCore::appendDimension, &LineGraphCore::countDimensions, &LineGraphCore::dimensionAt, &LineGraphCore::clearDimensions);
}

QList<Dimension*> LineGraphCore::dimensionsList() const
{
    return m_dimensions;
}

LineGraphCore::GraphStyle LineGraphCore::graphStyle() const {
    return m_graphStyle;
}

void LineGraphCore::setGraphStyle(GraphStyle graphStyle) {
    if (graphStyle != m_graphStyle) {
        m_graphStyle = graphStyle;
        triggerUpdate();
        emit graphStyleChanged();
    }
}

qreal LineGraphCore::pitch() const
{
    return m_pitch;
}

void LineGraphCore::setPitch(qreal pitch)
{
    if (pitch != m_pitch)
    {
        m_pitch = pitch;
        triggerUpdate();
        emit pitchChanged();
    }
}

qreal LineGraphCore::pointRadius() const
{
    return m_pointRadius;
}

void LineGraphCore::setPointRadius(qreal pointRadius)
{
    if (pointRadius != m_pointRadius)
    {
        m_pointRadius = pointRadius;
        triggerUpdate();
        emit pointRadiusChanged();
    }
}

void LineGraphCore::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    paintAxisAndLines(painter);
}

void LineGraphCore::paintAxisAndLines(QPainter* painter)
{
    const int minY = qRound(m_pointRadius);
    const int maxY = height() - m_pointRadius;
    const int distance = (maxY - minY) / 4;
    const qreal x1 = x();
    const qreal x2 = x1 + width();
    int y = minY;

    painter->setPen(QPen(QBrush("#808080"), 1));

    for (int i = 0; i < 4; i++, y += distance)
    {
        painter->drawLine(QPointF(x1, y), QPointF(x2, y));
    }

    painter->setPen(QPen(QBrush("#000000"), 1));
    painter->drawLine(QPointF(x1, maxY), QPointF(x2, maxY));
}

void LineGraphCore::triggerUpdate()
{
    emit updated();
    update();
}

void LineGraphCore::appendDimension(QDeclarativeListProperty<Dimension>* list, Dimension *dimension) {
    LineGraphCore* lineGraphCore = qobject_cast<LineGraphCore*>(list->object);
    if (lineGraphCore) {
        dimension->setParent(lineGraphCore);
        lineGraphCore->m_dimensions.append(dimension);
        lineGraphCore->triggerUpdate();
    }
}

int LineGraphCore::countDimensions(QDeclarativeListProperty<Dimension>* list) {
    LineGraphCore* lineGraphCore = qobject_cast<LineGraphCore*>(list->object);
    if (lineGraphCore) {
        return lineGraphCore->m_dimensions.count();
    }
    return -1;
}

Dimension* LineGraphCore::dimensionAt(QDeclarativeListProperty<Dimension>* list, int index) {
    LineGraphCore* lineGraphCore = qobject_cast<LineGraphCore*>(list->object);
    if (lineGraphCore) {
        return lineGraphCore->m_dimensions.at(index);
    }
    return 0;
}

void LineGraphCore::clearDimensions(QDeclarativeListProperty<Dimension>* list) {
    LineGraphCore* lineGraphCore = qobject_cast<LineGraphCore*>(list->object);
    if (lineGraphCore) {
        lineGraphCore->m_dimensions.clear();
        lineGraphCore->triggerUpdate();
    }
}
