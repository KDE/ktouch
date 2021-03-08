/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QTime>
#include <QUrl>
#include <QColor>

class Utils : public QObject
{
    Q_OBJECT
public:
    explicit Utils(QObject* parent = nullptr);
    Q_INVOKABLE int getMinutesOfQTime(const QTime& time);
    Q_INVOKABLE int getSecondsOfQTime(const QTime& time);
    Q_INVOKABLE QString uuid();
    Q_INVOKABLE QColor alpha(const QColor& color, float alpha);

};


#endif // UTILS_H
