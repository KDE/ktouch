/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
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
    Q_PROPERTY(QString lastUsedCourseId READ lastUsedCourseId WRITE setLastUsedCourseId NOTIFY lastUsedCourseIdChanged)

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
    QString lastUsedCourseId() const;
    void setLastUsedCourseId(const QString &id);

signals:
    void idChanged();
    void nameChanged();
    void skillLevelChanged();
    void lastUsedCourseIdChanged();

private:
    int m_id;
    QString m_name;
    SkillLevel m_skillLevel;
    QString m_lastUsedCourseId;
};

#endif // PROFILE_H
