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

#ifndef STRINGFORMATTER_H
#define STRINGFORMATTER_H

#include <QObject>

class QTime;

class StringFormatter : public QObject
{
    Q_OBJECT
public:
    explicit StringFormatter(QObject* parent = 0);
    Q_INVOKABLE QString formatTime(const QTime& time);
    Q_INVOKABLE QString formatTimeDiff(const QTime& from, const QTime& to);
    Q_INVOKABLE QString formatAccuracy(qreal accuracy);
    Q_INVOKABLE QString formatAccuracyDiff(qreal from, qreal to);
    Q_INVOKABLE QString formatSign(qreal value);
};

#endif // STRINGFORMATTER_H
