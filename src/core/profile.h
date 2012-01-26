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
