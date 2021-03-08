/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "utils.h"

#include <QUuid>
#include <QtDebug>
#include <QStandardPaths>

Utils::Utils(QObject* parent):
    QObject(parent)
{
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

QColor Utils::alpha(const QColor& color, float alpha)
{
    auto result = QColor(color);
    result.setAlphaF(alpha * result.alphaF());
    return result;
}
