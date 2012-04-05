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

#ifndef KEY_H
#define KEY_H

#include "abstractkey.h"

#include <QList>

class KeyChar;

class Key : public AbstractKey
{
    Q_OBJECT
    Q_PROPERTY(unsigned int fingerIndex READ fingerIndex WRITE setFingerIndex NOTIFY fingerIndexChanged)
    Q_PROPERTY(bool hasHapticMarker READ hasHapticMarker WRITE setHasHapticMarker NOTIFY hasHapticMarkerChanged)
    Q_PROPERTY(int keyCharCount READ keyCharCount NOTIFY keyCharCountChanged)

public:
    explicit Key(QObject* parent = 0);

    Q_INVOKABLE QString keyType() const {
        return "key";
    }

    int fingerIndex() const
    {
        return m_fingerIndex;
    }

    void setFingerIndex(unsigned int finger)
    {
        Q_ASSERT(finger < 8);
        if (finger != m_fingerIndex)
        {
            m_fingerIndex = finger;
            emit fingerIndexChanged(finger);
        }
    }

    bool hasHapticMarker() const
    {
        return m_hasHapticMarker;
    }

    void setHasHapticMarker(bool hasHapticMarker)
    {
        if (hasHapticMarker != m_hasHapticMarker)
        {
            m_hasHapticMarker = hasHapticMarker;
            emit hasHapticMarkerChanged(hasHapticMarker);
        }
    }

    int keyCharCount() const
    {
        return m_keyChars.count();
    }

    Q_INVOKABLE KeyChar* keyChar(unsigned int index) const;
    Q_INVOKABLE void addKeyChar(KeyChar* keyChar);
    Q_INVOKABLE void removeKeyChar(unsigned int index);
    Q_INVOKABLE void clearKeyChars();

signals:
    void fingerIndexChanged(unsigned int newFingerIndex);
    void hasHapticMarkerChanged(bool newHasHapticMarker);
    void keyCharCountChanged(int newKeyCharCount);

private:
    unsigned int m_fingerIndex;
    bool m_hasHapticMarker;
    QList<KeyChar*> m_keyChars;
};

#endif // KEY_H
