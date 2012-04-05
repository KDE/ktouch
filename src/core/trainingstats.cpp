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
    m_ellapsedTime(0),
    m_errorCount(0),
    m_startTime(0),
    m_updateTimer(new QTimer(this))
{
    connect(m_updateTimer, SIGNAL(timeout()), SLOT(update()));
}

void TrainingStats::startTraining()
{
    if (!m_timeIsRunning)
    {
        m_timeIsRunning = true;
        m_startTime = QDateTime::currentMSecsSinceEpoch() - m_ellapsedTime;
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
    m_ellapsedTime = 0;
    m_errorCount = 0;
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
    if (m_ellapsedTime == 0)
    {
        return 0;
    }

    return m_charactersTyped * 60000 / m_ellapsedTime;
}

void TrainingStats::update()
{
    m_updateTimer->stop();
    if (m_timeIsRunning)
    {
        qint64 now = QDateTime::currentMSecsSinceEpoch();
        m_ellapsedTime = now - m_startTime;
        m_updateTimer->start(200);
    }
    emit statsChanged();
}
