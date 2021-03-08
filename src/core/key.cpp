/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "key.h"

#include "keychar.h"

Key::Key(QObject* parent) :
    AbstractKey(parent),
    m_fingerIndex(0),
    m_hasHapticMarker(false)
{
}

QString Key::keyType() const
{
    return QStringLiteral("key");
}

int Key::fingerIndex() const
{
    return m_fingerIndex;
}

void Key::setFingerIndex(int finger)
{
    Q_ASSERT(finger >= 0 && finger <= 8);

    if(finger != m_fingerIndex)
    {
        m_fingerIndex = finger;
        emit fingerIndexChanged();
    }
}

bool Key::hasHapticMarker() const
{
    return m_hasHapticMarker;
}

void Key::setHasHapticMarker(bool hasHapticMarker)
{
    if(hasHapticMarker != m_hasHapticMarker)
    {
        m_hasHapticMarker = hasHapticMarker;
        emit hasHapticMarkerChanged();
    }
}

const QList<KeyChar*>& Key::keyChars() const
{
    return m_keyChars;
}

int Key::keyCharCount() const
{
    return m_keyChars.count();
}

KeyChar* Key::keyChar(int index) const
{
    Q_ASSERT(index >= 0 && index < m_keyChars.length());
    return m_keyChars.at(index);
}

void Key::addKeyChar(KeyChar* keyChar)
{
    emit keyCharAboutToBeAdded(keyChar, m_keyChars.length());
    m_keyChars.append(keyChar);
    keyChar->setParent(this);
    emit keyCharCountChanged();
    emit keyCharAdded();
}

void Key::insertKeyChar(int index, KeyChar* keyChar)
{
    Q_ASSERT(index >= 0 && index < m_keyChars.length());
    emit keyCharAboutToBeAdded(keyChar, index);
    m_keyChars.insert(index, keyChar);
    keyChar->setParent(this);
    emit keyCharCountChanged();
    emit keyCharAdded();
}

void Key::removeKeyChar(int index)
{
    Q_ASSERT(index >= 0 && index < m_keyChars.length());
    emit keyCharsAboutToBeRemoved(index, index);
    delete m_keyChars.at(index);
    m_keyChars.removeAt(index);
    emit keyCharCountChanged();
    emit keyCharsRemoved();
}

void Key::clearKeyChars()
{
    if (m_keyChars.isEmpty())
        return;

    emit keyCharsAboutToBeRemoved(0, m_keyChars.length() - 1);
    qDeleteAll(m_keyChars);
    m_keyChars.clear();
    emit keyCharCountChanged();
    emit keyCharsRemoved();
}

void Key::copyFrom(Key* source)
{
    AbstractKey::copyFrom(source);
    setFingerIndex(source->fingerIndex());
    setHasHapticMarker(source->hasHapticMarker());
    clearKeyChars();
    for (int j = 0; j < source->keyCharCount(); j++) {
        KeyChar* keyChar = new KeyChar(this);
        keyChar->copyFrom(source->keyChar(j));
        addKeyChar(keyChar);
    }
}
