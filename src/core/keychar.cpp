#include "keychar.h"

#include <kdebug.h>

KeyChar::KeyChar(QObject *parent) :
    QObject(parent),
    m_position(KeyChar::Hidden)
{
}

QString KeyChar::positionStr() const
{
    switch (m_position)
    {
    case KeyChar::TopLeft:
        return "topLeft";
    case KeyChar::TopRight:
        return "topRight";
    case KeyChar::BottomLeft:
        return "bottomLeft";
    case KeyChar::BottomRight:
        return "bottomRight";
    case KeyChar::Hidden:
    default:
        return "hidden";
    }
}

void KeyChar::setPositionStr(const QString &position)
{
    if (position == "topLeft")
    {
        m_position = KeyChar::TopLeft;
    }
    else if (position == "topRight")
    {
        m_position = KeyChar::TopRight;
    }
    else if (position == "bottomLeft")
    {
        m_position = KeyChar::BottomLeft;
    }
    else if (position == "bottomRight")
    {
        m_position = KeyChar::BottomRight;
    }
    else
    {
        m_position = KeyChar::Hidden;
    }
}
