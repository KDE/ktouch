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
class Course;
class Lesson;

class LearningProgressModel : public QSqlQueryModel
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile READ profile WRITE setProfile NOTIFY profileChanged)
    Q_PROPERTY(Course* courseFilter READ courseFilter WRITE setCourseFilter NOTIFY courseFilterChanged)
    Q_PROPERTY(Lesson* lessonFilter READ lessonFilter WRITE setLessonFilter NOTIFY lessonFilterChanged)
    Q_PROPERTY(int maxCharactersTypedPerMinute READ maxCharactersTypedPerMinute NOTIFY maxCharactersTypedPerMinuteChanged)
    Q_PROPERTY(qreal minAccuracy READ minAccuracy NOTIFY minAccuracyChanged)
public:
    explicit LearningProgressModel(QObject* parent = 0);
    Profile* profile() const;
    void setProfile(Profile* profile);
    Course* courseFilter() const;
    void setCourseFilter(Course* courseFilter);
    Lesson* lessonFilter() const;
    void setLessonFilter(Lesson* lessonFilter);
    int maxCharactersTypedPerMinute() const;
    qreal minAccuracy() const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Q_INVOKABLE int charactersPerMinute(int row) const;
    Q_INVOKABLE int charactersTyped(int row) const;
    Q_INVOKABLE int errorCount(int row) const;
    Q_INVOKABLE int elapsedTime(int row) const;
    Q_INVOKABLE qreal accuracy(int row) const;
    Q_INVOKABLE QString lessonId(int row) const;
public slots:
    void update();
signals:
    void profileChanged();
    void courseFilterChanged();
    void lessonFilterChanged();
    void maxCharactersTypedPerMinuteChanged();
    void minAccuracyChanged();
private slots:
    void profileDestroyed();
private:
    QVariant data(const QModelIndex& item, int role = Qt::DisplayRole) const;
    QVariant accuracyData(int row, int role = Qt::DisplayRole) const;
    QVariant charactersPerMinuteData(int row, int role = Qt::DisplayRole) const;
    int m_charactersTypedFieldIndex;
    Profile* m_profile;
    Course* m_courseFilter;
    Lesson* m_lessonFilter;
};

#endif // LEARNINGPROGRESSMODEL_H
