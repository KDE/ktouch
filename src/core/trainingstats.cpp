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

#include "trainingstats.h"

#include <QDateTime>
#include <QTimer>

TrainingStats::TrainingStats(QObject* parent) :
    QObject(parent),
    m_timeIsRunning(false),
    m_charactersTyped(0),
    m_elapsedTime(0),
    m_errorCount(0),
    m_isValid(true),
    m_startTime(0),
    m_updateTimer(new QTimer(this))
{
    connect(m_updateTimer, SIGNAL(timeout()), SLOT(update()));
}

int TrainingStats::charactesTyped() const
{
    return m_charactersTyped;
}

void TrainingStats::setCharactersTyped(int charactesTyped)
{
    if(charactesTyped != m_charactersTyped)
    {
        m_charactersTyped = charactesTyped;
        emit statsChanged();
    }
}

QTime TrainingStats::elapsedTime() const
{
    return QTime(0, 0).addMSecs(m_elapsedTime);
}

void TrainingStats::setElapsedTime(const QTime& elapsedTime)
{
    quint64 msec = elapsedTime.msec();

    if(msec != m_elapsedTime)
    {
        m_elapsedTime = msec;
        emit statsChanged();
    }
}

void TrainingStats::setElapsedTime(const quint64& msec)
{
    if(msec != m_elapsedTime)
    {
        m_elapsedTime = msec;
        emit statsChanged();
    }
}

int TrainingStats::errorCount() const
{
    return m_errorCount;
}

void TrainingStats::setErrorCount(int errorCount)
{
    if(errorCount != m_errorCount)
    {
        m_errorCount = errorCount;
        emit statsChanged();
    }
}

bool TrainingStats::isValid() const
{
    return m_isValid;
}

void TrainingStats::setIsValid(bool isValid)
{
    if(isValid != m_isValid)
    {
        m_isValid = isValid;
        emit isValidChanged();
    }
}
QMap< QString, int > TrainingStats::errorMap() const
{
    return m_errorMap;
}

void TrainingStats::setErrorMap(const QMap< QString, int >& errorMap)
{
    m_errorMap = errorMap;
    emit errorsChanged();
}

bool TrainingStats::timeIsRunning() const
{
    return m_timeIsRunning;
}

void TrainingStats::startTraining()
{
    if (!m_timeIsRunning)
    {
        m_timeIsRunning = true;
        m_startTime = QDateTime::currentMSecsSinceEpoch() - m_elapsedTime;
        update();
    }
}

void TrainingStats::stopTraining()
{
    if (m_timeIsRunning)
    {
        m_timeIsRunning = false;
        update();
    }
}

void TrainingStats::reset()
{
    stopTraining();
    m_charactersTyped = 0;
    m_elapsedTime = 0;
    m_errorCount = 0;
    m_errorMap.clear();
    statsChanged();
}

void TrainingStats::logCharacter(QString character, EventType type)
{
    if (type == TrainingStats::CorrectCharacter)
    {
        m_charactersTyped++;
    }
    else
    {
        m_errorCount++;
        if (m_errorMap.contains(character))
        {
            m_errorMap[character]++;
        }
        else
        {
            m_errorMap[character] = 1;
        }

        emit errorsChanged();
    }
}

float TrainingStats::accuracy()
{
    if (m_charactersTyped == 0)
    {
        return m_errorCount == 0? 1.0: 0.0 ;
    }

    return 1.0 - float(m_errorCount) / float(m_errorCount + m_charactersTyped);
}

int TrainingStats::charactersPerMinute()
{
    if (m_elapsedTime == 0)
    {
        return 0;
    }

    return m_charactersTyped * 60000 / m_elapsedTime;
}

void TrainingStats::update()
{
    m_updateTimer->stop();
    if (m_timeIsRunning)
    {
        qint64 now = QDateTime::currentMSecsSinceEpoch();
        m_elapsedTime = now - m_startTime;
        m_updateTimer->start(200);
    }
    emit statsChanged();
}
