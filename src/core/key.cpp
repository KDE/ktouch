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

#include "key.h"

#include "keychar.h"

Key::Key(QObject* parent) :
    AbstractKey(parent)
{
}

KeyChar* Key::keyChar(unsigned int index) const
{
    Q_ASSERT(index < m_keyChars.length());
    return m_keyChars.at(index);
}

void Key::addKeyChar(KeyChar* keyChar)
{
    m_keyChars.append(keyChar);
    emit keyCharCountChanged(m_keyChars.count());
}

void Key::removeKeyChar(unsigned int index)
{
    Q_ASSERT(index < m_keyChars.length());
    delete m_keyChars.at(index);
    m_keyChars.removeAt(index);
    emit keyCharCountChanged(m_keyChars.count());
}

void Key::clearKeyChars()
{
    qDeleteAll(m_keyChars);
    m_keyChars.clear();
    emit keyCharCountChanged(m_keyChars.count());
}
