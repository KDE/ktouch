#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QObject>

#include <QString>
#include <QVariant>

#include "abstractkey.h"

class QXmlSchema;
class QDomDocument;
class QIODevice;

class Keyboard : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(QString fontSuggestion READ fontSuggestion WRITE setFontSuggestion NOTIFY fontSuggestionChanged)
    Q_PROPERTY(AbstractKey* referenceKey READ referenceKey NOTIFY referenceKeyChanged)

public:
    Keyboard(QObject* parent = 0);

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

    const QString& language() const
    {
        return m_language;
    }

    void setLanguage(const QString& language)
    {
        if (language != m_language)
        {
            m_language = language;
            emit languageChanged(language);
        }
    }

    const QString& fontSuggestion() const
    {
        return m_fontSuggestion;
    }

    void setFontSuggestion(const QString& fontSuggestion)
    {
        if (fontSuggestion != m_fontSuggestion)
        {
            m_fontSuggestion = fontSuggestion;
            emit fontSuggestionChanged(fontSuggestion);
        }
    }

    Q_INVOKABLE int keyCount() const
    {
        return m_keys.count();
    }

    Q_INVOKABLE AbstractKey* key(unsigned int index) const
    {
        Q_ASSERT(index < m_keys.count());
        return m_keys.at(index);
    }

    void setKey(unsigned int index, AbstractKey* key)
    {
        Q_ASSERT(index < m_keys.count());
        m_keys[index] = key;
        key->setParent(this);
        updateReferenceKey(key);
    }

    void addKey(AbstractKey* key)
    {
        Q_ASSERT(index < m_keys.count());
        m_keys.append(key);
        key->setParent(this);
        updateReferenceKey(key);
    }

    void removeKey(unsigned int index)
    {
        Q_ASSERT(index < m_keys.count());
        delete m_keys.at(index);
        m_keys.removeAt(index);
        updateReferenceKey(0);
    }

    AbstractKey* referenceKey()
    {
        return m_referenceKey;
    }

signals:
    void titleChanged(const QString& newTitle);
    void languageChanged(const QString& newLanguage);
    void fontSuggestionChanged(const QString& newFontSuggestion);
    void referenceKeyChanged(AbstractKey* newReferenceKey);
    void resetted();

private:
    static QXmlSchema* xmlSchema();

    bool parseXML(QDomDocument* doc);
    void updateReferenceKey(AbstractKey* newKey=0);
    bool compareKeysForReference(const AbstractKey* testKey, const AbstractKey* compareKey) const;
    void reset();

    static QXmlSchema* s_schema;

    QString m_title;
    QString m_language;
    QString m_fontSuggestion;
    QList<AbstractKey*> m_keys;
    AbstractKey* m_referenceKey;

};

#endif // KEYBOARD_H
