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

#ifndef CURSORSHAPEAREA_H
#define CURSORSHAPEAREA_H

#include <QDeclarativeItem>

class CursorShapeArea : public QDeclarativeItem
{
    Q_OBJECT

    Q_PROPERTY(Qt::CursorShape cursorShape READ cursorShape WRITE setCursorShape NOTIFY cursorShapeChanged)

public:
    explicit CursorShapeArea(QDeclarativeItem* parent = 0);

    Qt::CursorShape cursorShape() const;
    Q_INVOKABLE void setCursorShape(Qt::CursorShape cursorShape);

private:
    int m_currentShape;

signals:
    void cursorShapeChanged();
};

#endif // CURSORSHAPEAREA_H
