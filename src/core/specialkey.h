/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

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
    Q_INVOKABLE QString keyType() const override;
    QString typeStr() const;
    void setTypeStr(const QString& typeStr);
    Type type() const;
    void setType(Type type);
    QString modifierId() const;
    void setModifierId(const QString& modifierId);
    QString label() const ;
    void setLabel(const QString& label);
    Q_INVOKABLE void copyFrom(SpecialKey* source);

signals:
    void typeChanged();
    void modifierIdChanged();
    void labelChanged();

private:
    Type m_type;
    QString m_modifierId;
    QString m_label;
};

#endif // SPECIALKEY_H
