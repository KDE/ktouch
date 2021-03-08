/*
 * SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef DBACCESS_H
#define DBACCESS_H

#include <QObject>

class QSqlDatabase;
class QSqlError;

class DbAccess : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
public:
    explicit DbAccess(QObject* parent = 0);
    QString errorMessage() const;

signals:
    void errorMessageChanged();

protected:
    QSqlDatabase database();
    void raiseError(const QSqlError& error);
private:
    bool checkDbSchema();
    bool migrateFrom1_0To1_1();
    QString m_errorMessage;
};

#endif // DBACCESS_H
