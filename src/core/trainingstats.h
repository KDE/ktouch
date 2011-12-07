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

    Q_INVOKABLE void startTraining();
    Q_INVOKABLE void stopTraining();
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
