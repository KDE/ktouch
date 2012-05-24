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
    Q_INVOKABLE QString keyType() const ;
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
