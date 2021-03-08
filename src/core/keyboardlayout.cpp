/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "keyboardlayout.h"

#include <QFile>
#include <QUrl>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QXmlSchema>
#include <QXmlSchemaValidator>


#include "abstractkey.h"
#include "key.h"
#include "keychar.h"
#include "specialkey.h"
#include "dataindex.h"

KeyboardLayout::KeyboardLayout(QObject *parent) :
    KeyboardLayoutBase(parent),
    m_associatedDataIndexKeyboardLayout(0),
    m_title(QLatin1String("")),
    m_name(QLatin1String("")),
    m_width(0),
    m_height(0),
    m_keys(QList<AbstractKey*>()),
    m_referenceKey(0)
{
}

DataIndexKeyboardLayout* KeyboardLayout::associatedDataIndexKeyboardLayout() const
{
    return m_associatedDataIndexKeyboardLayout;
}

void KeyboardLayout::setAssociatedDataIndexKeyboardLayout(DataIndexKeyboardLayout* dataIndexKeyboardLayout)
{
    if (dataIndexKeyboardLayout != m_associatedDataIndexKeyboardLayout)
    {
        m_associatedDataIndexKeyboardLayout = dataIndexKeyboardLayout;
        emit associatedDataIndexKeyboardLayoutChanged();
    }
}

void KeyboardLayout::setId(const QString& id)
{
    KeyboardLayoutBase::setId(id);

    if (m_associatedDataIndexKeyboardLayout)
    {
        m_associatedDataIndexKeyboardLayout->setId(id);
    }
}

void KeyboardLayout::setTitle(const QString& title)
{
    KeyboardLayoutBase::setTitle(title);

    if (m_associatedDataIndexKeyboardLayout)
    {
        m_associatedDataIndexKeyboardLayout->setTitle(title);
    }
}

void KeyboardLayout::setName(const QString& name)
{
    KeyboardLayoutBase::setName(name);

    if (m_associatedDataIndexKeyboardLayout)
    {
        m_associatedDataIndexKeyboardLayout->setName(name);
    }
}

int KeyboardLayout::width() const
{
    return m_width;
}

void KeyboardLayout::setWidth(int width)
{
    if(width != m_width)
    {
        m_width = width;
        emit widthChanged();
    }
}

int KeyboardLayout::height() const
{
    return m_height;
}

void KeyboardLayout::setHeight(int height)
{
    if(height != m_height)
    {
        m_height = height;
        emit heightChanged();
    }
}

int KeyboardLayout::keyCount() const
{
    return m_keys.count();
}

AbstractKey* KeyboardLayout::referenceKey()
{
    return m_referenceKey;
}

void KeyboardLayout::copyFrom(KeyboardLayout* source)
{
    setIsValid(false);
    setTitle(source->title());
    setName(source->name());
    setWidth(source->width());
    setHeight(source->height());
    clearKeys();
    for(int i = 0; i < source->keyCount(); i++)
    {
        AbstractKey* const abstractSourceKey = source->key(i);
        AbstractKey* abstractKey = 0;

        Key* const sourceKey = qobject_cast<Key*>(abstractSourceKey);
        if (sourceKey)
        {
            Key* key = new Key(this);
            key->copyFrom(sourceKey);
            abstractKey = key;
        }

        SpecialKey* const sourceSpecialKey = qobject_cast<SpecialKey*>(abstractSourceKey);
        if (sourceSpecialKey)
        {
            SpecialKey* specialKey = new SpecialKey(this);
            specialKey->copyFrom(sourceSpecialKey);
            abstractKey = specialKey;
        }

        addKey(abstractKey);
    }
    setIsValid(true);
}

QString KeyboardLayout::allCharacters() const
{
    QString result;
    for (const auto abstractKey: m_keys)
    {
        Key* const key = qobject_cast<Key*>(abstractKey);
        if (key)
        {
            for (const auto keyChar : key->keyChars())
            {
                result.append(keyChar->value());
            }
        }
    }
    return result;
}

AbstractKey* KeyboardLayout::key(int index) const
{
    Q_ASSERT(index >= 0 && index < m_keys.count());
    return m_keys.at(index);
}

int KeyboardLayout::keyIndex(AbstractKey* key) const
{
    return m_keys.indexOf(key);
}

void KeyboardLayout::addKey(AbstractKey* key)
{
    m_keys.append(key);
    key->setParent(this);
    connect(key, &AbstractKey::widthChanged, this, [=] { onKeyGeometryChanged(m_keys.count() - 1); } );
    connect(key, &AbstractKey::heightChanged, this, [=] { onKeyGeometryChanged(m_keys.count() - 1); } );
    emit keyCountChanged();
    updateReferenceKey(key);
}

void KeyboardLayout::insertKey(int index, AbstractKey* key)
{
    m_keys.insert(index, key);
    key->setParent(this);
    connect(key, &AbstractKey::widthChanged, this, [=] { onKeyGeometryChanged(m_keys.count() - 1); } );
    connect(key, &AbstractKey::heightChanged, this, [=] { onKeyGeometryChanged(m_keys.count() - 1); } );
    emit keyCountChanged();
    updateReferenceKey(key);
}

void KeyboardLayout::removeKey(int index)
{
    Q_ASSERT(index >= 0 && index < m_keys.count());
    AbstractKey* key = m_keys.at(index);
    m_keys.removeAt(index);
    emit keyCountChanged();
    updateReferenceKey(0);
    key->deleteLater();
}

void KeyboardLayout::clearKeys()
{
    if (m_keys.isEmpty())
        return;

    qDeleteAll(m_keys);
    m_keys.clear();
    emit keyCountChanged();
    updateReferenceKey(0);
}

QSize KeyboardLayout::size() const
{
    return QSize(m_width, m_height);
}

void KeyboardLayout::setSize(const QSize& size)
{
    setWidth(size.width());
    setHeight(size.height());
}

void KeyboardLayout::onKeyGeometryChanged(int keyIndex)
{
    updateReferenceKey(key(keyIndex));
}

void KeyboardLayout::updateReferenceKey(AbstractKey *testKey)
{
    if (testKey)
    {
        if (!m_referenceKey)
        {
            m_referenceKey = testKey;
            emit referenceKeyChanged();
            return;
        }
        if (compareKeysForReference(testKey, m_referenceKey))
        {
            m_referenceKey = testKey;
            emit referenceKeyChanged();
            return;
        }
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
    emit referenceKeyChanged();
}

bool KeyboardLayout::compareKeysForReference(const AbstractKey *testKey, const AbstractKey *compareKey) const
{
    return testKey->width() * testKey->height() < compareKey->width() * compareKey->height();
}
