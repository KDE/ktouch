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


#ifndef LINEGRAPHBACKGROUNDPAINTER_H
#define LINEGRAPHBACKGROUNDPAINTER_H

#include <QDeclarativeItem>

class LineGraphCore;

class LineGraphBackgroundPainter : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(LineGraphCore* lineGraphCore READ lineGraphCore WRITE setLineGraphCore NOTIFY lineGraphCoreChanged)
public:
    explicit LineGraphBackgroundPainter(QDeclarativeItem* parent = 0);
    LineGraphCore* lineGraphCore() const;
    void setLineGraphCore(LineGraphCore* lineGraphCore);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    const QList<QPolygonF>& linePolygons() const;
signals:
    void lineGraphCoreChanged();
private slots:
    void triggerUpdate();
private:
    void updateWidth();
    void updateLinePolygons();
    LineGraphCore* m_lineGraphCore;
    QList<QPolygonF> m_linePolygons;
};

#endif // LINEGRAPHBACKGROUNDPAINTER_H
