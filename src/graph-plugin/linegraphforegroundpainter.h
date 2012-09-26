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

#ifndef LINEGRAPHFOREGROUNDPAINTER_H
#define LINEGRAPHFOREGROUNDPAINTER_H

#include <QDeclarativeItem>

class LineGraphCore;

class LineGraphForegroundPainter : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(LineGraphCore* lineGraphCore READ lineGraphCore WRITE setGraphCoreItem NOTIFY lineGraphCoreChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
public:
    explicit LineGraphForegroundPainter(QDeclarativeItem* parent = 0);
    LineGraphCore* lineGraphCore() const;
    void setGraphCoreItem(LineGraphCore* lineGraphCore);
    QColor backgroundColor() const;
    void setBackgroundColor(const QColor& backgroundColor);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
signals:
    void lineGraphCoreChanged();
    void backgroundColorChanged();
private slots:
    void triggerUpdate();
private:
    LineGraphCore* m_lineGraphCore;
    QColor m_backgroundColor;
};

#endif // LINEGRAPHFOREGROUNDPAINTER_H
