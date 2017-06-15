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

#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QTime>
#include <QUrl>

class Utils : public QObject
{
    Q_OBJECT
public:
    explicit Utils(QObject* parent = 0);
    Q_INVOKABLE QUrl findImage(QString name);
    Q_INVOKABLE int getMinutesOfQTime(const QTime& time);
    Q_INVOKABLE int getSecondsOfQTime(const QTime& time);
    Q_INVOKABLE QString uuid();

};


#endif // UTILS_H
