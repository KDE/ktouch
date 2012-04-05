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

#ifndef PROFILEDATAACCESS_H
#define PROFILEDATAACCESS_H

#include <QObject>
#include <QList>

class QSqlDatabase;
class QSqlError;
class Profile;

class ProfileDataAccess : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int profileCount READ profileCount NOTIFY profileCountChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
public:
    explicit ProfileDataAccess(QObject* parent = 0);
    Q_INVOKABLE void loadProfiles();
    int profileCount() const;
    Q_INVOKABLE Profile* profile(unsigned int index);
    Q_INVOKABLE Profile* createProfile();
    Q_INVOKABLE void addProfile(Profile* profile);
    Q_INVOKABLE void updateProfile(unsigned int index);
    Q_INVOKABLE void removeProfile(unsigned int index);
    Q_INVOKABLE int indexOfProfile(Profile* profile);
    QString errorMessage() const;
signals:
    void profileCountChanged();
    void errorMessageChanged();
private:
    QSqlDatabase database();
    bool checkDbSchema();
    void raiseError(const QSqlError& error);
    QString m_errorMessage;
    QList<Profile*> m_profiles;
};

#endif // PROFILEDATAACCESS_H
