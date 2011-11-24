#include "keyboardlayout.h"

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

#include "abstractkey.h"
#include "key.h"
#include "specialkey.h"
#include "keychar.h"

KeyboardLayout::KeyboardLayout(QObject *parent) :
    QObject(parent)
{
}

bool KeyboardLayout::loadXML(QIODevice* dev)
{
    QXmlSchema* schema = KeyboardLayout::xmlSchema();
    if (!schema)
    {
        return false;
    }
    QXmlSchemaValidator validator;
    validator.setSchema(*schema);
    if (!validator.validate(dev))
    {
        kWarning() << "invalid keyboard XML";
        return false;
    }
    dev->reset();
    QDomDocument doc;
    QString errorMsg;
    if (!doc.setContent(dev, &errorMsg))
    {
        kWarning() << errorMsg;
        return false;
    }
    return parseXML(&doc);
}

AbstractKey* KeyboardLayout::key(unsigned int index) const
{
    Q_ASSERT(index < m_keys.count());
    return m_keys.at(index);
}

void KeyboardLayout::addKey(AbstractKey* key)
{
    m_keys.append(key);
    key->setParent(this);
    emit keyCountChanged(m_keys.count());
    updateReferenceKey(key);
}

void KeyboardLayout::removeKey(unsigned int index)
{
    Q_ASSERT(index < m_keys.count());
    delete m_keys.at(index);
    m_keys.removeAt(index);
    emit keyCountChanged(m_keys.count());
    updateReferenceKey(0);
}

void KeyboardLayout::clearKeys()
{
    qDeleteAll(m_keys);
    m_keys.clear();
    emit keyCountChanged(m_keys.count());
    updateReferenceKey(0);
}


QXmlSchema* KeyboardLayout::xmlSchema()
{
    if (!s_schema)
    {
        QString resourceName = "schemata/keyboardlayout.xsd";
        QString schemaPath = KGlobal::dirs()->findResource("appdata", resourceName);
        if (schemaPath.isNull())
        {
            kWarning() << "can't find resource" << resourceName;
            return 0;
        }
        QFile schemaFile(schemaPath);
        if (!schemaFile.open(QIODevice::ReadOnly))
        {
            kWarning() << "can't open" << schemaPath;
            return 0;
        }
        QXmlSchema* schema = new QXmlSchema();
        schema->load(&schemaFile, QUrl::fromLocalFile(schemaPath));
        if (!schema->isValid())
        {
            kWarning() << resourceName << "is invalid";
            delete schema;
            return 0;
        }
        s_schema = schema;
    }
    return s_schema;
}

bool KeyboardLayout::parseXML(QDomDocument* doc)
{
    QDomElement root = doc->documentElement();
    setTitle(i18nc("Keyboard Layout Name", root.firstChildElement("title").text().toUtf8()));
    setName(root.firstChildElement("name").text());
    setWidth(root.firstChildElement("width").text().toUInt());
    setHeight(root.firstChildElement("height").text().toUInt());
    clearKeys();
    for (QDomElement keyNode = root.firstChildElement("keys").firstChildElement();
         !keyNode.isNull();
         keyNode = keyNode.nextSiblingElement())
    {
        AbstractKey* abstractKey;

        if (keyNode.tagName() == "key")
        {
            Key* key = new Key(this);
            key->setFingerIndex(keyNode.attribute("fingerIndex").toUInt());
            key->setHasHapticMarker(keyNode.attribute("hasHapticMarker") == "true");
            for (QDomElement charNode = keyNode.firstChildElement("char");
                 !charNode.isNull();
                 charNode = charNode.nextSiblingElement("char"))
            {
                KeyChar* keyChar = new KeyChar(key);
                keyChar->setValue(charNode.text().at(0));
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
        abstractKey->setLeft(keyNode.attribute("left").toUInt());
        abstractKey->setTop(keyNode.attribute("top").toUInt());
        abstractKey->setWidth(keyNode.attribute("width").toUInt());
        abstractKey->setHeight(keyNode.attribute("height").toUInt());
        addKey(abstractKey);
    }
    kDebug() << "read" << m_title << "with" << m_keys.length() << "keys";
    return true;
}

void KeyboardLayout::updateReferenceKey(AbstractKey *newKey)
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

bool KeyboardLayout::compareKeysForReference(const AbstractKey *testKey, const AbstractKey *compareKey) const
{
    return testKey->width() * testKey->height() < compareKey->width() * compareKey->height();
}

QXmlSchema* KeyboardLayout::s_schema;
