#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QObject>

#include <QString>
#include <QVariant>

class QXmlSchema;
class QDomDocument;
class QIODevice;
class AbstractKey;

class KeyboardLayout : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(AbstractKey* referenceKey READ referenceKey NOTIFY referenceKeyChanged)
    Q_PROPERTY(unsigned int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(unsigned int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int keyCount READ keyCount NOTIFY keyCountChanged)

public:
    KeyboardLayout(QObject* parent = 0);

    bool loadXML(QIODevice* dev);

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

    unsigned int width() const
    {
        return m_width;
    }

    void setWidth(unsigned int width)
    {
        if (width != m_width)
        {
            m_width = width;
            emit widthChanged(width);
        }
    }

    unsigned int height() const
    {
        return m_height;
    }

    void setHeight(unsigned int height)
    {
        if (height != m_height)
        {
            m_height = height;
            emit heightChanged(height);
        }
    }

    int keyCount() const
    {
        return m_keys.count();
    }

    Q_INVOKABLE AbstractKey* key(unsigned int index) const;
    Q_INVOKABLE void addKey(AbstractKey* key);
    Q_INVOKABLE void removeKey(unsigned int index);
    Q_INVOKABLE void clearKeys();

    AbstractKey* referenceKey()
    {
        return m_referenceKey;
    }

signals:
    void titleChanged(const QString& newTitle);
    void nameChanged(const QString& newName);
    void widthChanged(unsigned int newWidth);
    void heightChanged(unsigned int newHeight);
    void referenceKeyChanged(AbstractKey* newReferenceKey);
    void keyCountChanged(int newKeyCount);

private:
    static QXmlSchema* xmlSchema();

    bool parseXML(QDomDocument* doc);
    void updateReferenceKey(AbstractKey* newKey=0);
    bool compareKeysForReference(const AbstractKey* testKey, const AbstractKey* compareKey) const;

    static QXmlSchema* s_schema;

    QString m_title;
    QString m_name;
    unsigned int m_width;
    unsigned int m_height;
    QList<AbstractKey*> m_keys;
    AbstractKey* m_referenceKey;

};

#endif // KEYBOARD_H
