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


#ifndef LINEGRAPHPOINT_H
#define LINEGRAPHPOINT_H

#include <QDeclarativeItem>

class LineGraphCore;
class LineGraphBackgroundPainter;

class LineGraphPoint : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(LineGraphCore* lineGraphCore READ lineGraphCore WRITE setGraphCoreItem NOTIFY lineGraphCoreChanged)
    Q_PROPERTY(LineGraphBackgroundPainter* backgroundPainter READ backgroundPainter WRITE setBackgroundPainter NOTIFY backgroundPainterChanged)
    Q_PROPERTY(int dimension READ dimension WRITE setDimension NOTIFY dimensionChanged)
    Q_PROPERTY(int row READ row WRITE setRow NOTIFY rowChanged)
public:
    explicit LineGraphPoint(QDeclarativeItem* parent = 0);
    LineGraphCore* lineGraphCore() const;
    void setGraphCoreItem(LineGraphCore* lineGraphCores);
    LineGraphBackgroundPainter* backgroundPainter() const;
    void setBackgroundPainter(LineGraphBackgroundPainter* backgroundPainter);
    int dimension() const;
    void setDimension(int dimension);
    int row() const;
    void setRow(int row);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
signals:
    void lineGraphCoreChanged();
    void backgroundPainterChanged();
    void dimensionChanged();
    void rowChanged();
    void maxYChanged();
private slots:
    void triggerUpdate();
private:
    void updateGeometry();
    LineGraphCore* m_lineGraphCore;
    LineGraphBackgroundPainter* m_backgroundPainter;
    int m_dimension;
    int m_row;
};

#endif // LINEGRAPHPOINT_H
