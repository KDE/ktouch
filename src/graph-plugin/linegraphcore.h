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

#include <QDeclarativeItem>

#include "dimension.h"

class QAbstractTableModel;

class LineGraphCore : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(QAbstractTableModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QDeclarativeListProperty<Dimension> dimensions READ dimensions)
    Q_PROPERTY(GraphStyle graphStyle READ graphStyle WRITE setGraphStyle NOTIFY graphStyleChanged)
    Q_PROPERTY(qreal pitch READ pitch WRITE setPitch NOTIFY pitchChanged)
    Q_PROPERTY(qreal pointRadius READ pointRadius WRITE setPointRadius NOTIFY pointRadiusChanged)
    Q_ENUMS(GraphStyle)
public:
    enum GraphStyle {
        Lines,
        Bars
    };
    explicit LineGraphCore(QDeclarativeItem* parent = 0);
    QAbstractTableModel* model() const;
    void setModel(QAbstractTableModel* model);
    QDeclarativeListProperty<Dimension> dimensions();
    QList<Dimension*> dimensionsList() const;
    GraphStyle graphStyle() const;
    void setGraphStyle(GraphStyle graphStyle);
    qreal pitch() const;
    void setPitch(qreal pitch);
    qreal pointRadius() const;
    void setPointRadius(qreal pointRadius);
signals:
    void modelChanged();
    void graphStyleChanged();
    void pitchChanged();
    void pointRadiusChanged();
    void updated();
private slots:
    void triggerUpdate();
private:
    static void appendDimension(QDeclarativeListProperty<Dimension>* list, Dimension* dimension);
    static int countDimensions(QDeclarativeListProperty<Dimension>* list);
    static Dimension* dimensionAt(QDeclarativeListProperty<Dimension>* list, int index);
    static void clearDimensions(QDeclarativeListProperty<Dimension>* list);
    QAbstractTableModel* m_model;
    QList<Dimension*> m_dimensions;
    GraphStyle m_graphStyle;
    qreal m_pitch;
    qreal m_pointRadius;
};

#endif // GRAPHCOREITEM_H
