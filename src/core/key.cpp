#include "key.h"

#include "keychar.h"

Key::Key(QObject* parent) :
    AbstractKey(parent)
{
}

KeyChar* Key::keyChar(unsigned int index) const
{
    Q_ASSERT(index < m_keyChars.length());
    return m_keyChars.at(index);
}

void Key::addKeyChar(KeyChar* keyChar)
{
    m_keyChars.append(keyChar);
    emit keyCharCountChanged(m_keyChars.count());
}

void Key::removeKeyChar(unsigned int index)
{
    Q_ASSERT(index < m_keyChars.length());
    delete m_keyChars.at(index);
    m_keyChars.removeAt(index);
    emit keyCharCountChanged(m_keyChars.count());
}

void Key::clearKeyChars()
{
    qDeleteAll(m_keyChars);
    m_keyChars.clear();
    emit keyCharCountChanged(m_keyChars.count());
}
