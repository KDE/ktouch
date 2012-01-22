#ifndef PROFILE_H
#define PROFILE_H

#include <QObject>

class Profile : public QObject
{
    Q_OBJECT
    Q_ENUMS(SkillLevel)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(SkillLevel skillLevel READ skillLevel WRITE setSkillLevel NOTIFY skillLevelChanged)

public:
    enum SkillLevel
    {
        Beginner,
        Advanced
    };

    explicit Profile(QObject* parent = 0);
    QString name() const;
    void setName(QString& name);
    SkillLevel skillLevel() const;
    void setSkillLevel(SkillLevel skillLevel);

signals:
    void nameChanged(const QString& newName);
    void skillLevelChanged(SkillLevel newSkillLevel);

private:
    QString m_name;
    SkillLevel m_skillLevel;
};

#endif // PROFILE_H
