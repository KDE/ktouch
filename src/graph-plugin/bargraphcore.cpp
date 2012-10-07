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

#include "bargraphcore.h"

BarGraphCore::BarGraphCore(QDeclarativeItem* parent) :
    GraphCore(parent)
{
    connect(this, SIGNAL(pitchChanged()), SIGNAL(barWidthChanged()));
}

qreal BarGraphCore::barWidth() const
{
    return qRound(pitch() / (dimensionsList().length() + 1));
}

void BarGraphCore::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    paintAxisAndLines(painter, 0.0);
}
