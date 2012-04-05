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

#ifndef TRAININGSTATS_H
#define TRAININGSTATS_H

#include <QObject>
#include <QChar>
#include <QTime>
#include <QMap>
#include <QString>

class QTimer;

class TrainingStats : public QObject
{
    Q_OBJECT
    Q_ENUMS(EventType)
    Q_PROPERTY(unsigned int charactesTyped READ charactesTyped WRITE setCharactersTyped NOTIFY statsChanged)
    Q_PROPERTY(QTime ellapsedTime READ ellapsedTime WRITE setEllapsedTime NOTIFY statsChanged)
    Q_PROPERTY(unsigned int errorCount READ errorCount NOTIFY statsChanged)
    Q_PROPERTY(float accuracy READ accuracy NOTIFY statsChanged)
    Q_PROPERTY(int charactersPerMinute READ charactersPerMinute NOTIFY statsChanged)
    Q_PROPERTY(bool timeIsRunning READ timeIsRunning NOTIFY statsChanged)

public:
    enum EventType {
        CorrectCharacter,
        IncorrectCharacter
    };

    explicit TrainingStats(QObject* parent = 0);

    int charactesTyped() const
    {
        return m_charactersTyped;
    }

    void setCharactersTyped(unsigned int charactesTyped)
    {
        if (charactesTyped != m_charactersTyped)
        {
            m_charactersTyped = charactesTyped;
            emit statsChanged();
        }
    }

    QTime ellapsedTime() const
    {
        return QTime(0, 0).addMSecs(m_ellapsedTime);
    }

    void setEllapsedTime(QTime& ellapsedTime)
    {
        quint64 msec = ellapsedTime.msec();
        if (msec != m_ellapsedTime)
        {
            m_ellapsedTime = msec;
            emit statsChanged();
        }
    }

    unsigned int errorCount() const
    {
        return m_errorCount;
    }

    bool timeIsRunning() const
    {
        return m_timeIsRunning;
    }

    Q_INVOKABLE void startTraining();
    Q_INVOKABLE void stopTraining();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void logCharacter(QString character, EventType type);
    float accuracy();
    int charactersPerMinute();

signals:
    void statsChanged();

private:
    Q_SLOT void update();
    bool m_timeIsRunning;
    unsigned int m_charactersTyped;
    quint64 m_ellapsedTime;
    unsigned int m_errorCount;
    QMap<QString, int> m_errorMap;
    quint64 m_startTime;
    QTimer* m_updateTimer;
};

#endif // TRAININGSTATS_H
