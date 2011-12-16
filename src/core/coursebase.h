#ifndef COURSEBASE_H
#define COURSEBASE_H

#include <QObject>

#include <QString>

class CourseBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString keyboardLayoutName READ keyboardLayoutName WRITE setKeyboardLayoutName NOTIFY keyboardLayoutNameChanged)

public:
    explicit CourseBase(QObject* parent = 0);

    QString title() const
    {
        return m_title;
    }

    void setTitle(const QString& title)
    {
        if (title != m_title)
        {
            m_title = title;
            emit titleChanged(title);
        }
    }

    QString description() const
    {
        return m_description;
    }

    void setDescription(const QString& description)
    {
        if (description != m_description)
        {
            m_description = description;
            emit descriptionChanged(description);
        }
    }

    QString keyboardLayoutName() const
    {
        return m_keyboardLayoutName;
    }

    void setKeyboardLayoutName(const QString& keyboardLayoutName)
    {
        if (keyboardLayoutName != m_keyboardLayoutName)
        {
            m_keyboardLayoutName = keyboardLayoutName;
            emit keyboardLayoutNameChanged(keyboardLayoutName);
        }
    }

signals:

    void titleChanged(const QString& newTitle);
    void descriptionChanged(const QString& newDescription);
    void keyboardLayoutNameChanged(const QString& newKeyboardLayoutName);

private:
    QString m_title;
    QString m_description;
    QString m_keyboardLayoutName;

};

#endif // COURSEBASE_H
