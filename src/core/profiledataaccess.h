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

#ifndef PROFILEDATAACCESS_H
#define PROFILEDATAACCESS_H

#include "core/dbaccess.h"

#include <QObject>
#include <QDateTime>
#include <QList>
#include <QSqlQuery>

class Profile;
class TrainingStats;
class Course;
class Lesson;
class KeyboardLayout;

class ProfileDataAccess : public DbAccess
{
    Q_OBJECT
    Q_PROPERTY(int profileCount READ profileCount NOTIFY profileCountChanged)
    Q_ENUMS(CourseProgressType)

public:
    enum CourseProgressType
    {
        LastUnlockedLesson = 1,
        LastSelectedLesson = 2
    };

    explicit ProfileDataAccess(QObject* parent = 0);

    Q_INVOKABLE void loadProfiles();
    int profileCount() const;
    Q_INVOKABLE Profile* profile(int index);
    Q_INVOKABLE Profile* createProfile();
    Q_INVOKABLE void addProfile(Profile* profile);
    Q_INVOKABLE void updateProfile(int index);
    Q_INVOKABLE void removeProfile(int index);
    Q_INVOKABLE int indexOfProfile(Profile* profile);

    Q_INVOKABLE void loadReferenceTrainingStats(TrainingStats* stats, Profile* profile, const QString& courseId, const QString& lessonId);
    Q_INVOKABLE void saveTrainingStats(TrainingStats* stats, Profile* profile, const QString& courseId, const QString& lessonId);

    Q_INVOKABLE QString courseProgress(Profile* profile, const QString& courseId, CourseProgressType type);
    Q_INVOKABLE void saveCourseProgress(const QString& lessonId, Profile* profile, const QString& courseId, CourseProgressType type);

    Q_INVOKABLE int lessonsTrained(Profile* profile);
    Q_INVOKABLE quint64 totalTrainingTime(Profile* profile);
    Q_INVOKABLE QDateTime lastTrainingSession(Profile* profile);

    Q_INVOKABLE bool loadCustomLessons(Profile* profile, const QString& keyboardLayoutNameFilter, Course* target);

    Q_INVOKABLE bool storeCustomLesson(Lesson* lesson, Profile* profile, const QString& keyboardLayoutName);
    Q_INVOKABLE bool deleteCustomLesson(const QString& id);

    QSqlQuery learningProgressQuery(Profile* profile, Course* courseFilter = 0, Lesson* lessonFilter = 0);

signals:
    void profileCountChanged();

private:
    int findCourseProgressId(Profile* profile, const QString &courseId, CourseProgressType type, bool* ok);
    QList<Profile*> m_profiles;
};

#endif // PROFILEDATAACCESS_H
