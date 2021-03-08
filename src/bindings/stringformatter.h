/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
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
