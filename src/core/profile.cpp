#include "profile.h"

Profile::Profile(QObject* parent) :
    QObject(parent)
{
}

QString Profile::name() const
{
    return m_name;
}

void Profile::setName(QString &name)
{
    if (name != m_name)
    {
        m_name = name;
        emit nameChanged(name);
    }
}

Profile::SkillLevel Profile::skillLevel() const
{
    return m_skillLevel;
}

void Profile::setSkillLevel(SkillLevel skillLevel)
{
    if (skillLevel != m_skillLevel)
    {
        m_skillLevel = skillLevel;
        emit skillLevelChanged(skillLevel);
    }
}
