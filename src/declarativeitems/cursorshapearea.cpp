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

#include "cursorshapearea.h"

CursorShapeArea::CursorShapeArea(QDeclarativeItem* parent) :
    QDeclarativeItem(parent),
    m_currentShape(-1)
{
}

Qt::CursorShape CursorShapeArea::cursorShape() const
{
    return cursor().shape();
}

void CursorShapeArea::setCursorShape(Qt::CursorShape cursorShape)
{
    if (m_currentShape == (int) cursorShape)
      return;

    setCursor(cursorShape);
    emit cursorShapeChanged();

    m_currentShape = cursorShape;
}
