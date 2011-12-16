#ifndef KEYBOARDLAYOUTBASE_H
#define KEYBOARDLAYOUTBASE_H

#include <QObject>

#include <QString>

class KeyboardLayoutBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit KeyboardLayoutBase(QObject *parent = 0);

    const QString& title() const
    {
        return m_title;
    }

    void setTitle(const QString& title)
    {
        m_title = title;
        emit titleChanged(title);
    }

    const QString& name() const
    {
        return m_name;
    }

    void setName(const QString& name)
    {
        if (name != m_name)
        {
            m_name = name;
            emit nameChanged(name);
        }
    }

signals:
    void titleChanged(const QString& newTitle);
    void nameChanged(const QString& newName);

private:
    QString m_title;
    QString m_name;

};

#endif // KEYBOARDLAYOUTBASE_H
