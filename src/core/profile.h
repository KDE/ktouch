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

#ifndef PROFILE_H
#define PROFILE_H

#include <QObject>

class Profile : public QObject
{
    Q_OBJECT
    Q_ENUMS(SkillLevel)
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(SkillLevel skillLevel READ skillLevel WRITE setSkillLevel NOTIFY skillLevelChanged)

public:
    enum SkillLevel
    {
        Beginner = 1,
        Advanced
    };

    explicit Profile(QObject* parent = 0);
    int id() const;
    void setId(int id);
    QString name() const;
    void setName(const QString& name);
    SkillLevel skillLevel() const;
    void setSkillLevel(SkillLevel skillLevel);

signals:
    void idChanged(int newId);
    void nameChanged(const QString& newName);
    void skillLevelChanged(SkillLevel newSkillLevel);

private:
    int m_id;
    QString m_name;
    SkillLevel m_skillLevel;
};

#endif // PROFILE_H
