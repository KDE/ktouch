/*
 *  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "keyboardlayout.h"

#include <QSignalMapper>
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
#include "dataindex.h"

KeyboardLayout::KeyboardLayout(QObject *parent) :
    KeyboardLayoutBase(parent),
    m_associatedDataIndexKeyboardLayout(0),
    m_title(""),
    m_name(""),
    m_width(0),
    m_height(0),
    m_keys(QList<AbstractKey*>()),
    m_referenceKey(0),
    m_signalMapper(new QSignalMapper(this))
{
    connect(m_signalMapper, SIGNAL(mapped(int)), SLOT(onKeyGeometryChanged(int)));
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
    connect(key, SIGNAL(widthChanged()), m_signalMapper, SLOT(map()));
    connect(key, SIGNAL(heightChanged()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(key, m_keys.count() - 1);
    emit keyCountChanged();
    updateReferenceKey(key);
}

void KeyboardLayout::insertKey(int index, AbstractKey* key)
{
    m_keys.insert(index, key);
    key->setParent(this);
    connect(key, SIGNAL(widthChanged()), m_signalMapper, SLOT(map()));
    connect(key, SIGNAL(heightChanged()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(key, m_keys.count() - 1);
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
    if (m_keys.count() == 0)
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
