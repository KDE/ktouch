/*
 * SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
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
    enum ExtraRole {
        DateRole = Qt::UserRole + 1,
        CharactersTypedRole,
        ErrorCountRole,
        ElapsedTimeRole,
        LessonIdRole,
        AccuracyRole,
        CharactersPerMinuteRole,
    };

    explicit LearningProgressModel(QObject* parent = nullptr);
    Profile* profile() const;
    void setProfile(Profile* profile);
    Course* courseFilter() const;
    void setCourseFilter(Course* courseFilter);
    Lesson* lessonFilter() const;
    void setLessonFilter(Lesson* lessonFilter);
    int maxCharactersTypedPerMinute() const;
    qreal minAccuracy() const;
    Q_INVOKABLE QDateTime date(int row) const;
    Q_INVOKABLE int charactersPerMinute(int row) const;
    Q_INVOKABLE int charactersTyped(int row) const;
    Q_INVOKABLE int errorCount(int row) const;
    Q_INVOKABLE int elapsedTime(int row) const;
    Q_INVOKABLE qreal accuracy(int row) const;
    Q_INVOKABLE QString lessonId(int row) const;
public Q_SLOTS:
    void update();
Q_SIGNALS:
    void profileChanged();
    void courseFilterChanged();
    void lessonFilterChanged();
    void maxCharactersTypedPerMinuteChanged();
    void minAccuracyChanged();
private Q_SLOTS:
    void profileDestroyed();
private:
    QVariant data(const QModelIndex& item, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    int m_charactersTypedFieldIndex;
    Profile* m_profile;
    Course* m_courseFilter;
    Lesson* m_lessonFilter;
};

#endif // LEARNINGPROGRESSMODEL_H
