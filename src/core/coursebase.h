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

#ifndef COURSEBASE_H
#define COURSEBASE_H

#include "resource.h"

#include <QString>

class CourseBase : public Resource
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString keyboardLayoutName READ keyboardLayoutName WRITE setKeyboardLayoutName NOTIFY keyboardLayoutNameChanged)

public:
    explicit CourseBase(QObject* parent = 0);

    QString title() const;
    void setTitle(const QString& title);
    QString description() const;
    void setDescription(const QString& description);
    QString keyboardLayoutName() const;
    void setKeyboardLayoutName(const QString& keyboardLayoutName);

signals:

    void titleChanged();
    void descriptionChanged();
    void keyboardLayoutNameChanged();

private:
    QString m_title;
    QString m_description;
    QString m_keyboardLayoutName;

};

#endif // COURSEBASE_H
