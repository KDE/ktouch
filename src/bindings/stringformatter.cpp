/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "stringformatter.h"

#include <QTime>

StringFormatter::StringFormatter(QObject* parent) :
    QObject(parent)
{
}

QString StringFormatter::formatTime(const QTime& time)
{
    return time.toString(QStringLiteral("mm:ss.zzz")).left(7);
}

QString StringFormatter::formatTimeDiff(const QTime& from, const QTime& to)
{
    const int diff = from.msecsTo(to);
    const QTime diffTime = QTime(0, 0).addMSecs(diff > 0? diff: -diff);
    return QStringLiteral("%1 %2").arg(formatSign(diff), formatTime(diffTime));
}

QString StringFormatter::formatAccuracy(qreal accuracy)
{
    return QStringLiteral("%1%").arg(100 * accuracy, 0, 'f', 1);
}

QString StringFormatter::formatAccuracyDiff(qreal from, qreal to)
{
    const qreal diff = to - from;
    const QString accuracy = formatAccuracy(diff > 0? diff: -diff);
    return QStringLiteral("%1 %2").arg(formatSign(diff), accuracy);
}

QString StringFormatter::formatSign(qreal value)
{
    if (value > 0)
        return QStringLiteral("+");
    if (value < 0)
        return QStringLiteral("-");
    return QString::fromUtf8("\u00B1");
}
