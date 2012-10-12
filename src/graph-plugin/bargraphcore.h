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

#ifndef BARGRAPHCORE_H
#define BARGRAPHCORE_H

#include "graphcore.h"

class BarGraphCore : public GraphCore
{
    Q_OBJECT
    Q_PROPERTY(int textRole READ textRole WRITE setTextRole NOTIFY textRoleChanged)
    Q_PROPERTY(qreal barWidth READ barWidth NOTIFY barWidthChanged)
public:
    explicit BarGraphCore(QDeclarativeItem* parent = 0);
    int textRole() const;
    void setTextRole(int textRole);
    qreal barWidth() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
signals:
    void textRoleChanged();
    void barWidthChanged();
private:
    int m_textRole;
};

#endif // BARGRAPHCORE_H
