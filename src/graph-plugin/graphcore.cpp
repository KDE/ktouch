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


#include "graphcore.h"

#include <QAbstractTableModel>
#include <QPainter>

GraphCore::GraphCore(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    m_model(0),
    m_pitch(50.0)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

QAbstractTableModel* GraphCore::model() const
{
    return m_model;
}

void GraphCore::setModel(QAbstractTableModel* model)
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

QDeclarativeListProperty<Dimension> GraphCore::dimensions() {
    return QDeclarativeListProperty<Dimension>(this, 0, &GraphCore::appendDimension, &GraphCore::countDimensions, &GraphCore::dimensionAt, &GraphCore::clearDimensions);
}

QList<Dimension*> GraphCore::dimensionsList() const
{
    return m_dimensions;
}

qreal GraphCore::pitch() const
{
    return m_pitch;
}

void GraphCore::setPitch(qreal pitch)
{
    if (pitch != m_pitch)
    {
        m_pitch = pitch;
        triggerUpdate();
        emit pitchChanged();
    }
}

void GraphCore::triggerUpdate()
{
    emit updated();
    update();
}

void GraphCore::paintAxisAndLines(QPainter* painter, qreal offset)
{
    const int minY = qRound(offset);
    const int maxY = height() - offset;
    const int distance = (maxY - minY) / 4;
    const qreal x1 = x();
    const qreal x2 = x1 + width();
    int y = minY;

    painter->setBrush(QBrush("#808080"));
    painter->setPen(Qt::NoPen);

    for (int i = 0; i < 4; i++, y += distance)
    {
        painter->drawRect(QRectF(QPointF(x1, y), QPointF(x2, y + 1)));
    }

    painter->setBrush(QBrush("#000000"));
    painter->drawRect(QRectF(QPointF(x1, maxY), QPointF(x2, maxY + 1)));
}

void GraphCore::appendDimension(QDeclarativeListProperty<Dimension>* list, Dimension *dimension) {
    GraphCore* graphCore = qobject_cast<GraphCore*>(list->object);
    if (graphCore) {
        dimension->setParent(graphCore);
        graphCore->m_dimensions.append(dimension);
        connect(dimension, SIGNAL(updated()), graphCore, SLOT(triggerUpdate()));
        graphCore->triggerUpdate();
    }
}

int GraphCore::countDimensions(QDeclarativeListProperty<Dimension>* list) {
    GraphCore* graphCore = qobject_cast<GraphCore*>(list->object);
    if (graphCore) {
        return graphCore->m_dimensions.count();
    }
    return -1;
}

Dimension* GraphCore::dimensionAt(QDeclarativeListProperty<Dimension>* list, int index) {
    GraphCore* graphCore = qobject_cast<GraphCore*>(list->object);
    if (graphCore) {
        return graphCore->m_dimensions.at(index);
    }
    return 0;
}

void GraphCore::clearDimensions(QDeclarativeListProperty<Dimension>* list) {
    GraphCore* graphCore = qobject_cast<GraphCore*>(list->object);
    if (graphCore) {
        foreach (Dimension* dimension, graphCore->m_dimensions)
        {
            dimension->disconnect(graphCore);
        }
        graphCore->m_dimensions.clear();
        graphCore->triggerUpdate();
    }
}
