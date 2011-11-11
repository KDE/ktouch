#include "keyboard.h"

#include <QFile>
#include <QUrl>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QXmlSchema>
#include <QXmlSchemaValidator>

#include <kstandarddirs.h>
#include <klocale.h>
#include <kdebug.h>

#include "key.h"
#include "specialkey.h"

QXmlSchema* Keyboard::s_schema;

Keyboard::Keyboard(QObject *parent) :
    QObject(parent)
{
}

bool Keyboard::loadXML(QIODevice* dev)
{
    QXmlSchema* schema = Keyboard::xmlSchema();
    if (!schema)
    {
        return false;
    }
    QXmlSchemaValidator validator;
    validator.setSchema(*schema);
    if (!validator.validate(dev))
    {
        kDebug() << "invalid keyboard XML";
        return false;
    }
    dev->reset();
    QDomDocument doc;
    QString errorMsg;
    if (!doc.setContent(dev, &errorMsg))
    {
        kDebug() << errorMsg;
        return false;
    }
    return parseXML(&doc);
}

QXmlSchema* Keyboard::xmlSchema()
{
    if (!Keyboard::s_schema)
    {
        QString resourceName = "schemata/keyboard.xsd";
        QString schemaPath = KGlobal::dirs()->findResource("appdata", resourceName);
        if (schemaPath.isNull())
        {
            kDebug() << "can't find resource" << resourceName;
            return 0;
        }
        QFile schemaFile(schemaPath);
        if (!schemaFile.open(QIODevice::ReadOnly))
        {
            kDebug() << "can't open" << schemaPath;
            return 0;
        }
        QXmlSchema* schema = new QXmlSchema();
        schema->load(&schemaFile, QUrl::fromLocalFile(schemaPath));
        if (!schema->isValid())
        {
            kDebug() << resourceName << "is invalid";
            delete schema;
            return 0;
        }
        s_schema = schema;
    }
    return s_schema;
}

bool Keyboard::parseXML(QDomDocument* doc)
{
    reset();
    QDomElement root = doc->documentElement();
    setTitle(i18nc("Keyboard Layout Name", root.firstChildElement("title").text().toUtf8()));
    setLanguage(root.firstChildElement("language").text());
    setFontSuggestion(root.firstChildElement("fontSuggestion").text());
    for (QDomElement keyNode = root.firstChildElement("keys").firstChildElement();
         !keyNode.isNull();
         keyNode = keyNode.nextSiblingElement())
    {
        AbstractKey* abstractKey;

        if (keyNode.tagName() == "key")
        {
            Key* key = new Key(this);
            key->setFingerIndex(keyNode.attribute("fingerIndex").toInt());
            key->setHasHapticMarker(keyNode.attribute("hasHapticMarker") == "true");
            for (QDomElement charNode = keyNode.firstChildElement("char");
                 !charNode.isNull();
                 charNode = charNode.nextSiblingElement("char"))
            {
                KeyChar* keyChar = new KeyChar(key);
                keyChar->setValue(charNode.text());
                keyChar->setPositionStr(charNode.attribute("position"));
                keyChar->setModifier(charNode.attribute("modifier"));
                key->addKeyChar(keyChar);
            }
            abstractKey = key;
        }
        else if (keyNode.tagName() == "specialKey")
        {
            SpecialKey* specialKey = new SpecialKey(this);
            specialKey->setTypeStr(keyNode.attribute("type"));
            specialKey->setModifierId(keyNode.attribute("modifierId"));
            specialKey->setLabel(keyNode.attribute("label"));
            abstractKey = specialKey;
        }
        else
        {
            return false;
        }
        abstractKey->setLeft(keyNode.attribute("left").toInt());
        abstractKey->setTop(keyNode.attribute("top").toInt());
        abstractKey->setWidth(keyNode.attribute("width").toInt());
        abstractKey->setHeight(keyNode.attribute("height").toInt());
        addKey(abstractKey);
    }
    kDebug() << "read" << m_title << "with" << m_keys.length() << "keys";
    return true;
}

void Keyboard::updateReferenceKey(AbstractKey *newKey)
{
    if (newKey)
    {
        if (!m_referenceKey)
        {
            m_referenceKey = newKey;
            emit referenceKeyChanged(newKey);
            return;
        }
        if (compareKeysForReference(newKey, m_referenceKey))
        {
            m_referenceKey = newKey;
            emit referenceKeyChanged(newKey);
        }
        return;
    }
    AbstractKey* canditate = 0;
    foreach(AbstractKey* key, m_keys)
    {
        if (canditate == 0)
        {
            canditate = key;
        }
        else
        {
            if (compareKeysForReference(key, canditate))
            {
                canditate = key;
            }
        }
    }
    m_referenceKey = canditate;
    emit referenceKeyChanged(canditate);
}

bool Keyboard::compareKeysForReference(const AbstractKey *testKey, const AbstractKey *compareKey) const
{
    return testKey->width() * testKey->height() < compareKey->width() * compareKey->height();
}

void Keyboard::reset()
{
    qDeleteAll(m_keys);
    m_keys.empty();
    m_title = QString();
    m_language = QString();
    m_fontSuggestion = QString();
    m_referenceKey = 0;
    emit resetted();
}

