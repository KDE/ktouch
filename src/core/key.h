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
    Q_PROPERTY(int fingerIndex READ fingerIndex WRITE setFingerIndex NOTIFY fingerIndexChanged)
    Q_PROPERTY(bool hasHapticMarker READ hasHapticMarker WRITE setHasHapticMarker NOTIFY hasHapticMarkerChanged)
    Q_PROPERTY(int keyCharCount READ keyCharCount NOTIFY keyCharCountChanged)

public:
    explicit Key(QObject* parent = 0);
    Q_INVOKABLE QString keyType() const;
    int fingerIndex() const;
    void setFingerIndex(int finger);
    bool hasHapticMarker() const;
    void setHasHapticMarker(bool hasHapticMarker);
    int keyCharCount() const;
    Q_INVOKABLE KeyChar* keyChar(int index) const;
    Q_INVOKABLE void addKeyChar(KeyChar* keyChar);
    Q_INVOKABLE void insertKeyChar(int index, KeyChar* keyChar);
    Q_INVOKABLE void removeKeyChar(int index);
    Q_INVOKABLE void clearKeyChars();
    Q_INVOKABLE void copyFrom(Key* source);

signals:
    void fingerIndexChanged();
    void hasHapticMarkerChanged();
    void keyCharCountChanged();
    void keyCharAboutToBeAdded(KeyChar* keyChar, int index);
    void keyCharAdded();
    void keyCharsAboutToBeRemoved(int first, int last);
    void keyCharsRemoved();

private:
    int m_fingerIndex;
    bool m_hasHapticMarker;
    QList<KeyChar*> m_keyChars;
};

#endif // KEY_H
