/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef KEYBOARDLAYOUTBASE_H
#define KEYBOARDLAYOUTBASE_H

#include "resource.h"

#include <QString>

class KeyboardLayoutBase : public Resource
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit KeyboardLayoutBase(QObject *parent = 0);
    const QString& title() const;
    void setTitle(const QString& title);
    const QString& name() const;
    void setName(const QString& name);

signals:
    void titleChanged();
    void nameChanged();

private:
    QString m_title;
    QString m_name;

};

#endif // KEYBOARDLAYOUTBASE_H
