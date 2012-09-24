/*
 * Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef LEARNINGPROGRESSMODEL_H
#define LEARNINGPROGRESSMODEL_H

#include <QSqlQueryModel>

class Profile;

class LearningProgressModel : public QSqlQueryModel
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile READ profile WRITE setProfile NOTIFY profileChanged)
public:
    explicit LearningProgressModel(QObject* parent = 0);
    Profile* profile() const;
    void setProfile(Profile* profile);
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex& item, int role = Qt::DisplayRole) const;
    QVariant accuracyData(int row, int role = Qt::DisplayRole) const;
    QVariant charactersPerMinuteData(int row, int role = Qt::DisplayRole) const;
signals:
    void profileChanged();
private:
    int charactersTyped(int row) const;
    int errorCount(int row) const;
    int elapsedTime(int row) const;
    int m_charactersTypedFieldIndex;
    void updateQuery();
    Profile* m_profile;
};

#endif // LEARNINGPROGRESSMODEL_H
