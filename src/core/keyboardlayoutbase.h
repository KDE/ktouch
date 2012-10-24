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

#ifndef KEYBOARDLAYOUTBASE_H
#define KEYBOARDLAYOUTBASE_H

#include "resource.h"

#include <QString>

class KeyboardLayoutBase : public Resource
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit KeyboardLayoutBase(QObject *parent = 0);
    const QString& title() const;
    void setTitle(const QString& title);
    const QString& name() const;
    void setName(const QString& name);

signals:
    void titleChanged();
    void nameChanged();

private:
    QString m_title;
    QString m_name;

};

#endif // KEYBOARDLAYOUTBASE_H
