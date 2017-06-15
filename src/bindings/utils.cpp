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

#include "utils.h"

#include <QTime>
#include <QUuid>
#include <QtDebug>
#include <QStandardPaths>

Utils::Utils(QObject* parent):
    QObject(parent)
{
}

QUrl Utils::findImage(QString name)
{
    const QString relPath = QString("images/") + name;
    const QString path = QStandardPaths::locate(QStandardPaths::DataLocation, relPath);

    if (path.isNull())
    {
        qWarning() << "can't find image resource:" << name;
        return QUrl();
    }

    return QUrl::fromLocalFile(path);
}

int Utils::getMinutesOfQTime(const QTime& time)
{
    if (!time.isValid())
    {
        qWarning() << "invalid QTime passed";
        return 0;
    }

    return time.minute();
}

int Utils::getSecondsOfQTime(const QTime& time)
{
    if (!time.isValid())
    {
        qWarning() << "invalid QTime passed";
        return 0;
    }

    return time.second();
}

QString Utils::uuid()
{
    return QUuid::createUuid().toString();
}
