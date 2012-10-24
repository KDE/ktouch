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


#ifndef GRAPHCORE_H
#define GRAPHCORE_H

#include <QDeclarativeItem>

#include "dimension.h"

class QAbstractTableModel;

class GraphCore : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(QAbstractTableModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QDeclarativeListProperty<Dimension> dimensions READ dimensions CONSTANT)
    Q_PROPERTY(qreal pitch READ pitch WRITE setPitch NOTIFY pitchChanged)
public:
    explicit GraphCore(QDeclarativeItem *parent = 0);
    QAbstractTableModel* model() const;
    void setModel(QAbstractTableModel* model);
    QDeclarativeListProperty<Dimension> dimensions();
    QList<Dimension*> dimensionsList() const;
    qreal pitch() const;
    void setPitch(qreal pitch);

signals:
    void modelChanged();
    void graphStyleChanged();
    void pitchChanged();
    void updated();
protected slots:
    void triggerUpdate();
protected:
    void paintAxisAndLines(QPainter* painter, qreal offset);
private:
    static void appendDimension(QDeclarativeListProperty<Dimension>* list, Dimension* dimension);
    static int countDimensions(QDeclarativeListProperty<Dimension>* list);
    static Dimension* dimensionAt(QDeclarativeListProperty<Dimension>* list, int index);
    static void clearDimensions(QDeclarativeListProperty<Dimension>* list);
    QAbstractTableModel* m_model;
    QList<Dimension*> m_dimensions;
    qreal m_pitch;
};

#endif // GRAPHCORE_H
