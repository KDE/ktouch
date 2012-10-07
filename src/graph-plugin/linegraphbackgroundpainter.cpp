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


#include "linegraphbackgroundpainter.h"

#include <QAbstractTableModel>
#include <QPainter>

#include "linegraphcore.h"
#include "dimension.h"

#include <KDebug>

LineGraphBackgroundPainter::LineGraphBackgroundPainter(QDeclarativeItem* parent) :
    QDeclarativeItem(parent),
    m_lineGraphCore(0)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

LineGraphCore* LineGraphBackgroundPainter::lineGraphCore() const
{
    return m_lineGraphCore;
}

void LineGraphBackgroundPainter::setLineGraphCore(LineGraphCore* lineGraphCore)
{
    if (lineGraphCore != m_lineGraphCore)
    {
        if (m_lineGraphCore)
        {
            m_lineGraphCore->disconnect(this);
        }

        m_lineGraphCore = lineGraphCore;

        if (m_lineGraphCore)
        {
            connect(m_lineGraphCore, SIGNAL(updated()), SLOT(triggerUpdate()));
        }

        update();
        emit lineGraphCoreChanged();
    }
}

const QList<QPolygonF>& LineGraphBackgroundPainter::linePolygons() const
{
    return m_linePolygons;
}

void LineGraphBackgroundPainter::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    if (m_lineGraphCore->model()->rowCount() == 0)
        return;

    QList<Dimension*> dimensions = m_lineGraphCore->dimensionsList();
    const qreal radius = m_lineGraphCore->pointRadius();
    const qreal maxY = height();

    for (int i = 0; i < dimensions.length(); i++)
    {

        QPolygonF line = m_linePolygons.at(i);

        line << QPointF(line.last().x(), maxY - radius);
        line << QPointF(line.first().x(), maxY - radius);

        QColor bgColor = dimensions.at(i)->color();
        bgColor.setAlphaF(0.4);
        painter->setBrush(bgColor);
        painter->setPen(Qt::NoPen);

        painter->drawPolygon(line);
    }
}

void LineGraphBackgroundPainter::triggerUpdate()
{
    if (!m_lineGraphCore->model())
        return;

    updateLinePolygons();
    updateWidth();
    update();
}

void LineGraphBackgroundPainter::updateWidth()
{
    QAbstractTableModel* model = m_lineGraphCore->model();

    if (!model)
    {
        setWidth(0);
        return;
    }

    setWidth(model->rowCount() * m_lineGraphCore->pitch());
}

void LineGraphBackgroundPainter::updateLinePolygons()
{
    m_linePolygons.clear();

    QList<Dimension*> dimensions = m_lineGraphCore->dimensionsList();
    QAbstractTableModel* model = m_lineGraphCore->model();
    const qreal pitch = m_lineGraphCore->pitch();
    const qreal radius = m_lineGraphCore->pointRadius();

    foreach(Dimension* dimension, dimensions)
    {
        const int column = dimension->dataColumn();
        const qreal maxValue = dimension->maximumValue();
        const qreal maxY = height();

        QPolygonF line;

        for (int row = 0; row < model->rowCount(); row++)
        {
            const qreal value = model->data(model->index(row, column)).toReal();
            line << QPointF(pointPos(pitch, radius, row, value, maxValue, maxY));
        }

        m_linePolygons << line;
    }
}

QPointF LineGraphBackgroundPainter::pointPos(qreal pitch, qreal radius, int row, qreal value, qreal maxValue, qreal maxY)
{
    const qreal x = (qreal(row) + 0.5) * pitch;
    const qreal y =  maxY - ((maxY - 2 * radius) * value / maxValue) - radius;
    return QPointF(x, y);
}

