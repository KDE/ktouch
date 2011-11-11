#ifndef SPECIALKEY_H
#define SPECIALKEY_H

#include "abstractkey.h"

class SpecialKey : public AbstractKey
{
    Q_OBJECT
    Q_ENUMS(Type)
    Q_PROPERTY(Type type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString modifierId READ modifierId WRITE setModifierId NOTIFY modifierIdChanged)
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)

public:
    enum Type {
        Tab,
        Capslock,
        Shift,
        Backspace,
        Return,
        Space,
        Other
    };

    explicit SpecialKey(QObject *parent = 0);

    QString typeStr() const;
    void setTypeStr(const QString& typeStr);

    Q_INVOKABLE QString keyType() const {
        return "specialKey";
    }

    Type type() const
    {
        return m_type;
    }

    void setType(Type type)
    {
        if (type != m_type)
        {
            m_type = type;
            emit typeChanged(type);
        }
    }

    QString modifierId() const
    {
        return m_modifierId;
    }

    void setModifierId(const QString& modifierId)
    {
        if (modifierId != m_modifierId)
        {
            m_modifierId = modifierId;
            emit modifierIdChanged(modifierId);
        }
    }

    QString label() const
    {
        return m_label;
    }

    void setLabel(const QString& label)
    {
        if (label != m_label)
        {
            m_label = label;
            emit labelChanged(label);
        }
    }

signals:
    void typeChanged(Type newType);
    void modifierIdChanged(const QString& newModifierId);
    void labelChanged(const QString& newLabel);

private:
    Type m_type;
    QString m_modifierId;
    QString m_label;
};

#endif // SPECIALKEY_H
