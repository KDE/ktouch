/*
 * SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "learningprogressmodel.h"

#include <QSqlRecord>

#include <KLocalizedString>

#include "core/profile.h"
#include "core/course.h"
#include "core/lesson.h"
#include "core/profiledataaccess.h"

LearningProgressModel::LearningProgressModel(QObject* parent) :
    QSqlQueryModel(parent),
    m_profile(nullptr),
    m_courseFilter(nullptr),
    m_lessonFilter(nullptr)
{
}

Profile* LearningProgressModel::profile() const
{
    return m_profile;
}

void LearningProgressModel::setProfile(Profile* profile)
{
    if (profile != m_profile)
    {
        if (m_profile)
        {
            m_profile->disconnect(this);
        }

        m_profile = profile;

        if (m_profile)
        {
            connect(m_profile, &Profile::idChanged, this, &LearningProgressModel::update);
            connect(m_profile, &QObject::destroyed, this, &LearningProgressModel::profileDestroyed);
        }

        update();
        Q_EMIT profileChanged();
    }
}

Course* LearningProgressModel::courseFilter() const
{
    return m_courseFilter;
}

void LearningProgressModel::setCourseFilter(Course* courseFilter)
{
    if (courseFilter != m_courseFilter)
    {
        if (m_courseFilter)
        {
            m_courseFilter->disconnect(this);
        }

        m_courseFilter = courseFilter;

        if (m_courseFilter)
        {
            connect(courseFilter, &Resource::idChanged, this, &LearningProgressModel::update);
        }

        update();
        Q_EMIT courseFilterChanged();
    }
}

Lesson* LearningProgressModel::lessonFilter() const
{
    return m_lessonFilter;
}

void LearningProgressModel::setLessonFilter(Lesson* lessonFilter)
{
    if (lessonFilter != m_lessonFilter)
    {
        if (m_lessonFilter)
        {
            m_lessonFilter->disconnect(this);
        }

        m_lessonFilter = lessonFilter;

        if (m_lessonFilter)
        {
            connect(m_lessonFilter, &Lesson::idChanged, this, &LearningProgressModel::update);
        }

        update();
        Q_EMIT lessonFilterChanged();
    }
}

int LearningProgressModel::maxCharactersTypedPerMinute() const
{
    int max = 0;

    for (int i = 0; i < rowCount(); i++)
    {
        max = qMax(max, charactersPerMinute(i));
    }

    return max;
}

qreal LearningProgressModel::minAccuracy() const
{
    qreal min = 1;

    for (int i = 0; i < rowCount(); i++)
    {
        min = qMin(min, accuracy(i));
    }

    return min;
}

QVariant LearningProgressModel::data(const QModelIndex &idx, int role) const
{
    if (idx.column() > 0) {
        return QSqlQueryModel::data(idx, Qt::DisplayRole);
    }

    switch (role) {
    case Qt::DisplayRole:
        return i18n("Lesson with id %1, elapsed time=%2, accuracy=%3", lessonId(idx.row()), date(idx.row()).toString(), accuracy(idx.row()));
    case DateRole:
        return QDateTime::fromMSecsSinceEpoch(QSqlQueryModel::data(index(idx.row(), 1), Qt::DisplayRole).value<quint64>());
    case CharactersTypedRole:
        return QSqlQueryModel::data(index(idx.row(), 2), Qt::DisplayRole);
    case ErrorCountRole:
        return QSqlQueryModel::data(index(idx.row(), 3), Qt::DisplayRole);
    case ElapsedTimeRole:
        return QSqlQueryModel::data(index(idx.row(), 4), Qt::DisplayRole);
    case LessonIdRole:
        return QSqlQueryModel::data(index(idx.row(), 5), Qt::DisplayRole);
    case AccuracyRole:
        return accuracy(idx.row());
    case CharactersPerMinuteRole:
        return charactersPerMinute(idx.row());
    default:
        return {};
    }
}

QHash<int, QByteArray> LearningProgressModel::roleNames() const
{
    return {
        { Qt::DisplayRole, "displayRole" },
        { DateRole, "date" },
        { CharactersTypedRole, "charactersTyped" },
        { ErrorCountRole, "errorCount" },
        { ElapsedTimeRole, "elapsedTime" },
        { LessonIdRole, "lessonId" },
        { AccuracyRole, "accuracy" },
        { CharactersPerMinuteRole, "charactersPerMinute" },
    };
}

QDateTime LearningProgressModel::date(int row) const
{
    LearningProgressModel* model = const_cast<LearningProgressModel*>(this);
    QSqlRecord record = model->record(row);
    return QDateTime::fromMSecsSinceEpoch(record.value(1).value<quint64>());
}

int LearningProgressModel::charactersPerMinute(int row) const
{
    const int time = this->elapsedTime(row);
    return time > 0 ? charactersTyped(row) * 60000 / time : 0;
}

int LearningProgressModel::charactersTyped(int row) const
{
    LearningProgressModel* model = const_cast<LearningProgressModel*>(this);
    QSqlRecord record = model->record(row);
    return record.value(2).toInt();
}

int LearningProgressModel::errorCount(int row) const
{
    LearningProgressModel* model = const_cast<LearningProgressModel*>(this);
    QSqlRecord record = model->record(row);
    return record.value(3).toInt();
}

int LearningProgressModel::elapsedTime(int row) const
{
    LearningProgressModel* model = const_cast<LearningProgressModel*>(this);
    QSqlRecord record = model->record(row);
    return record.value(4).toInt();
}

qreal LearningProgressModel::accuracy(int row) const
{
    const int charactersTyped = this->charactersTyped(row);
    const int errorCount = this->errorCount(row);

    const qreal accuracy = charactersTyped > 0?
                1.0 - qreal(errorCount) / qreal(errorCount + charactersTyped):
                errorCount == 0? 1.0: 0.0;

    return accuracy;
}

QString LearningProgressModel::lessonId(int row) const
{
    LearningProgressModel* model = const_cast<LearningProgressModel*>(this);
    QSqlRecord record = model->record(row);
    return record.value(5).toString();
}
void LearningProgressModel::update()
{
    ProfileDataAccess access;

    clear();

    if (m_profile)
    {
        const QSqlQuery query = access.learningProgressQuery(m_profile, m_courseFilter, m_lessonFilter);
        setQuery(query);
    }

    Q_EMIT maxCharactersTypedPerMinuteChanged();
    Q_EMIT minAccuracyChanged();
}

void LearningProgressModel::profileDestroyed()
{
    setProfile(nullptr);
}

#include "moc_learningprogressmodel.cpp"
