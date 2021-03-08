/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
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
