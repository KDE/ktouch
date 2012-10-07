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

#ifndef BARGRAPHSEGMENT_H
#define BARGRAPHSEGMENT_H

#include <QDeclarativeItem>

class BarGraphCore;

class BarGraphSegment : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(BarGraphCore* barGraphCore READ barGraphCore WRITE setBarGraphCore NOTIFY barGraphCoreChanged)
    Q_PROPERTY(int dimension READ dimension WRITE setDimension NOTIFY dimensionChanged)
    Q_PROPERTY(int row READ row WRITE setRow NOTIFY rowChanged)
    Q_PROPERTY(qreal barHeight READ barHeight NOTIFY barHeightChanged)
public:
    explicit BarGraphSegment(QDeclarativeItem* parent = 0);
    BarGraphCore* barGraphCore() const;
    void setBarGraphCore(BarGraphCore* barGraphCore);
    int dimension() const;
    void setDimension(int dimension);
    int row() const;
    void setRow(int row);
    qreal barHeight() const;
signals:
    void barGraphCoreChanged();
    void dimensionChanged();
    void rowChanged();
    void barHeightChanged();
private slots:
    void triggerUpdate();
private:
    bool valid() const;
    BarGraphCore* m_barGraphCore;
    int m_dimension;
    int m_row;
};

#endif // BARGRAPHSEGMENT_H
