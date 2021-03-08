/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
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
    Q_PROPERTY(int charactesTyped READ charactesTyped WRITE setCharactersTyped NOTIFY statsChanged)
    Q_PROPERTY(QTime elapsedTime READ elapsedTime WRITE setElapsedTime NOTIFY statsChanged)
    Q_PROPERTY(int errorCount READ errorCount NOTIFY statsChanged)
    Q_PROPERTY(bool isValid READ isValid WRITE setIsValid NOTIFY isValidChanged)
    Q_PROPERTY(float accuracy READ accuracy NOTIFY statsChanged)
    Q_PROPERTY(int charactersPerMinute READ charactersPerMinute NOTIFY statsChanged)
    Q_PROPERTY(bool timeIsRunning READ timeIsRunning NOTIFY statsChanged)

public:
    enum EventType {
        CorrectCharacter,
        IncorrectCharacter
    };

    explicit TrainingStats(QObject* parent = 0);
    int charactesTyped() const;
    void setCharactersTyped(int charactesTyped);
    QTime elapsedTime() const;
    void setElapsedTime(const QTime& elapsedTime);
    void setElapsedTime(const quint64& msec);
    int errorCount() const;
    void setErrorCount(int errorCount);
    bool isValid() const;
    void setIsValid(bool isValid);
    QMap<QString, int> errorMap() const;
    void setErrorMap(const QMap<QString, int>& errorMap);
    bool timeIsRunning() const;
    Q_INVOKABLE void startTraining();
    Q_INVOKABLE void stopTraining();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void logCharacter(const QString &character, EventType type);
    float accuracy();
    int charactersPerMinute();

signals:
    void statsChanged();
    void isValidChanged();
    void errorsChanged();

private:
    Q_SLOT void update();
    bool m_timeIsRunning;
    int m_charactersTyped;
    quint64 m_elapsedTime;
    int m_errorCount;
    bool m_isValid;
    QMap<QString, int> m_errorMap;
    quint64 m_startTime;
    QTimer* m_updateTimer;
};

#endif // TRAININGSTATS_H
