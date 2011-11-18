#ifndef LESSONLINE_H
#define LESSONLINE_H

#include <QObject>
#include <QString>

class LessonLine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString value READ value WRITE setValue NOTIFY valueChanged)

public:
    explicit LessonLine(QObject *parent = 0);

    QString value() const
    {
        return m_value;
    }

    void setValue(const QString& value)
    {
        if (value != m_value)
        {
            m_value = value;
            emit valueChanged(value);
        }
    }

signals:
    void valueChanged(const QString& newValue);

private:
    QString m_value;
};

#endif // LESSONLINE_H
