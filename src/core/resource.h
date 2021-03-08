/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef RESOURCE_H
#define RESOURCE_H

#include <QObject>

class Resource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(bool isValid READ isValid WRITE setIsValid NOTIFY isValidChanged)

public:
    explicit Resource(QObject* parent = 0);
    QString id() const;
    void setId(const QString& id);
    bool isValid() const;
    void setIsValid(bool isValid);

signals:
    void idChanged();
    void isValidChanged();

private:
    QString m_id;
    bool m_isValid;
};

#endif // RESOURCE_H
