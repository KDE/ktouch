#include "specialkey.h"

SpecialKey::SpecialKey(QObject *parent) :
    AbstractKey(parent),
    m_type(SpecialKey::Other)
{
}

QString SpecialKey::typeStr() const
{
    switch(m_type)
    {
    case SpecialKey::Tab:
        return "tab";
    case SpecialKey::Capslock:
        return "capslock";
    case SpecialKey::Shift:
        return "shift";
    case SpecialKey::Backspace:
        return "backspace";
    case SpecialKey::Return:
        return "return";
    case SpecialKey::Space:
        return "space";
    case SpecialKey::Other:
    default:
        return "other";
    }
}

void SpecialKey::setTypeStr(const QString &typeStr)
{
    if (typeStr == "tab")
    {
        m_type = SpecialKey::Tab;
    }
    else if (typeStr == "capslock")
    {
        m_type = SpecialKey::Capslock;
    }
    else if (typeStr == "shift")
    {
        m_type = SpecialKey::Shift;
    }
    else if (typeStr == "backspace")
    {
        m_type = SpecialKey::Backspace;
    }
    else if (typeStr == "return")
    {
        m_type = SpecialKey::Return;
    }
    else if (typeStr == "space")
    {
        m_type = SpecialKey::Space;
    }
    else
    {
        m_type = SpecialKey::Other;
    }
}
