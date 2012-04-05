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
