#ifndef KEY_H
#define KEY_H

#include "abstractkey.h"

#include <QList>

#include "keychar.h"

class Key : public AbstractKey
{
    Q_OBJECT
    Q_PROPERTY(unsigned int fingerIndex READ fingerIndex WRITE setFingerIndex NOTIFY fingerIndexChanged)
    Q_PROPERTY(bool hasHapticMarker READ hasHapticMarker WRITE setHasHapticMarker NOTIFY hasHapticMarkerChanged)

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

    Q_INVOKABLE int keyCharCount() const
    {
        return m_keyChars.count();
    }

    Q_INVOKABLE KeyChar* keyChar(unsigned int index) const
    {
        Q_ASSERT(index < m_keyChars.length());
        return m_keyChars.at(index);
    }

    void setKeyChar(unsigned int index, KeyChar* keyChar)
    {
        Q_ASSERT(index < m_keyChars.length());
        m_keyChars[index] = keyChar;
    }

    void addKeyChar(KeyChar* keyChar)
    {
        m_keyChars.append(keyChar);
    }

    void removeKeyChar(unsigned int index)
    {
        Q_ASSERT(index < m_keyChars.length());
        m_keyChars.removeAt(index);
    }

signals:
    void fingerIndexChanged(unsigned int newFingerIndex);
    void hasHapticMarkerChanged(bool newHasHapticMarker);

private:
    unsigned int m_fingerIndex;
    bool m_hasHapticMarker;
    QList<KeyChar*> m_keyChars;
};

#endif // KEY_H
