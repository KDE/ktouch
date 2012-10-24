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

#include "learningprogressmodel.h"

#include <QSqlRecord>

#include "core/profile.h"
#include "core/course.h"
#include "core/lesson.h"
#include "core/profiledataaccess.h"

LearningProgressModel::LearningProgressModel(QObject* parent) :
    QSqlQueryModel(parent),
    m_profile(0)
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
            connect(m_profile, SIGNAL(idChanged()), SLOT(update()));
            connect(m_profile, SIGNAL(destroyed()), SLOT(profileDestroyed()));
        }

        update();
        emit profileChanged();
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
            connect(courseFilter, SIGNAL(idChanged()), SLOT(update()));
        }

        update();
        emit courseFilterChanged();
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
            connect(m_lessonFilter, SIGNAL(idChanged()), SLOT(update()));
        }

        update();
        emit lessonFilterChanged();
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

int LearningProgressModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    const int originalColumnCount = QSqlQueryModel::columnCount();

    return originalColumnCount > 0? originalColumnCount + 2: 0;
}

QVariant LearningProgressModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Vertical)
        return QSqlQueryModel::headerData(section, orientation, role);

    const int originalColumnCount = QSqlQueryModel::columnCount();

    if (section < originalColumnCount)
        return QSqlQueryModel::headerData(section, orientation, role);

    switch (section - originalColumnCount)
    {
    case 0:
        return QVariant("accuracy");
    case 1:
        return QVariant("characters_per_minute");
    default:
        return QVariant();
    }
}

QVariant LearningProgressModel::data(const QModelIndex &item, int role) const
{
    const int originalColumnCount = QSqlQueryModel::columnCount();

    if (item.column() < originalColumnCount)
        return QSqlQueryModel::data(item, role);

    switch (item.column() - originalColumnCount)
    {
    case 0:
        return accuracyData(item.row(), role);
    case 1:
        return charactersPerMinuteData(item.row(), role);
    default:
        return QVariant();
    }
}

int LearningProgressModel::charactersPerMinute(int row) const
{
    const int charactersTyped = this->charactersTyped(row);
    const int elapsedTime = this->elapsedTime(row);

    return elapsedTime > 0? charactersTyped * 60000 / elapsedTime: 0;
}

int LearningProgressModel::charactersTyped(int row) const
{
    LearningProgressModel* model = const_cast<LearningProgressModel*>(this);
    QSqlRecord record = model->record(row);
    return record.value(1).toInt();
}

int LearningProgressModel::errorCount(int row) const
{
    LearningProgressModel* model = const_cast<LearningProgressModel*>(this);
    QSqlRecord record = model->record(row);
    return record.value(2).toInt();
}

int LearningProgressModel::elapsedTime(int row) const
{
    LearningProgressModel* model = const_cast<LearningProgressModel*>(this);
    QSqlRecord record = model->record(row);
    return record.value(3).toInt();
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
    return record.value(4).toString();
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

    emit maxCharactersTypedPerMinuteChanged();
    emit minAccuracyChanged();
}

QVariant LearningProgressModel::accuracyData(int row, int role) const
{
    const qreal accuracy = this->accuracy(row);

    switch(role)
    {
    case Qt::DisplayRole:
        return QVariant(accuracy);
    default:
        return QVariant();
    }
}

QVariant LearningProgressModel::charactersPerMinuteData(int row, int role) const
{
    int charactersPerMinute = this->charactersPerMinute(row);

    switch(role)
    {
    case Qt::DisplayRole:
        return QVariant(charactersPerMinute);
    default:
        return QVariant();
    }
}

void LearningProgressModel::profileDestroyed()
{
    setProfile(0);
}
