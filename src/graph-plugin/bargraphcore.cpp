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
    GraphCore(parent),
    m_textRole(-1)
{
    connect(this, SIGNAL(pitchChanged()), SIGNAL(barWidthChanged()));
}

int BarGraphCore::textRole() const
{
    return m_textRole;
}

void BarGraphCore::setTextRole(int textRole)
{
    if (textRole != m_textRole)
    {
        m_textRole = textRole;

        triggerUpdate();

        emit textRoleChanged();
    }
}


qreal BarGraphCore::barWidth() const
{
    return qRound(pitch() / (dimensionsList().length() + 1));
}

void BarGraphCore::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    paintAxisAndLines(painter, 0.0);
}
