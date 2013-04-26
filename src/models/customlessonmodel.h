/*
 *  Copyright 2013  Sebastian Gottfried <sebastiangottfried@web.de>
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


#ifndef CUSTOMLESSONMODEL_H
#define CUSTOMLESSONMODEL_H

#include <QSqlQueryModel>

class Profile;
class KeyboardLayout;

class CustomLessonModel : public QSqlQueryModel
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile READ profile WRITE setProfile NOTIFY profileChanged)
    Q_PROPERTY(QString keyboardLayoutNameFilter READ keyboardLayoutNameFilter WRITE setKeyboardLayoutNameFilter NOTIFY keyboardLayoutNameFilterChanged)
public:
    explicit CustomLessonModel(QObject* parent = 0);
    Profile* profile() const;
    void setProfile(Profile* profile);
    QString keyboardLayoutNameFilter() const;
    void setKeyboardLayoutNameFilter(const QString& name);
public slots:
    void update();
signals:
    void profileChanged();
    void keyboardLayoutNameFilterChanged();
private slots:
    void profileDestroyed();
private:
    Profile* m_profile;
    QString m_keyboardLayoutNameFilter;
};

#endif // CUSTOMLESSONMODEL_H
