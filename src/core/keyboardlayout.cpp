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
    KeyboardLayoutBase(parent),
    m_title(""),
    m_name(""),
    m_width(0),
    m_height(0),
    m_keys(QList<AbstractKey*>()),
    m_referenceKey(0)
{
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

AbstractKey* KeyboardLayout::key(int index) const
{
    Q_ASSERT(index >= 0 && index < m_keys.count());
    return m_keys.at(index);
}

void KeyboardLayout::addKey(AbstractKey* key)
{
    m_keys.append(key);
    key->setParent(this);
    emit keyCountChanged();
    updateReferenceKey(key);
}

void KeyboardLayout::removeKey(int index)
{
    Q_ASSERT(index >= 0 && index < m_keys.count());
    delete m_keys.at(index);
    m_keys.removeAt(index);
    emit keyCountChanged();
    updateReferenceKey(0);
}

void KeyboardLayout::clearKeys()
{
    qDeleteAll(m_keys);
    m_keys.clear();
    emit keyCountChanged();
    updateReferenceKey(0);
}


void KeyboardLayout::updateReferenceKey(AbstractKey *newKey)
{
    if (newKey)
    {
        if (!m_referenceKey)
        {
            m_referenceKey = newKey;
            emit referenceKeyChanged();
            return;
        }
        if (compareKeysForReference(newKey, m_referenceKey))
        {
            m_referenceKey = newKey;
            emit referenceKeyChanged();
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
    emit referenceKeyChanged();
}

bool KeyboardLayout::compareKeysForReference(const AbstractKey *testKey, const AbstractKey *compareKey) const
{
    return testKey->width() * testKey->height() < compareKey->width() * compareKey->height();
}
