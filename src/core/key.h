/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
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
    Q_INVOKABLE QString keyType() const override;
    int fingerIndex() const;
    void setFingerIndex(int finger);
    bool hasHapticMarker() const;
    void setHasHapticMarker(bool hasHapticMarker);
    const QList<KeyChar*>& keyChars() const;
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
