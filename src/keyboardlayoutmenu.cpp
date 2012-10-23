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

#include "keyboardlayoutmenu.h"

#include <KAction>
#include <KLocale>

#include "core/dataindex.h"
#include "preferences.h"

KeyboardLayoutMenu::KeyboardLayoutMenu(QWidget *parent) :
    QMenu(parent),
    m_dataIndex(0)
{
    setTitle(i18n("Choose Keyboard Layout"));
    setKeyboardLayoutName(Preferences::keyboardLayoutName());

    connect(this, SIGNAL(aboutToShow()), SLOT(populate()));
    connect(this, SIGNAL(triggered(QAction*)), SLOT(onActionTriggerd(QAction*)));
}

DataIndex* KeyboardLayoutMenu::dataIndex() const
{
    return m_dataIndex;
}

void KeyboardLayoutMenu::setDataIndex(DataIndex* dataIndex)
{
    m_dataIndex = dataIndex;
}

QString KeyboardLayoutMenu::keyboardLayoutName() const
{
    return m_keyboardLayoutName;
}

void KeyboardLayoutMenu::setKeyboardLayoutName(const QString &keyboardLayoutName)
{
    if (keyboardLayoutName != m_keyboardLayoutName)
    {
        m_keyboardLayoutName = keyboardLayoutName;
        emit keyboardLayoutNameChanged();
    }
}

void KeyboardLayoutMenu::populate()
{
    clear();

    if (!m_dataIndex)
        return;

    for (int i = 0; i < m_dataIndex->keyboardLayoutCount(); i++)
    {
        DataIndexKeyboardLayout* layout = m_dataIndex->keyboardLayout(i);
        KAction* action = new KAction(layout->title(), this);

        action->setData(layout->name());
        action->setCheckable(true);
        action->setChecked(layout->name() == m_keyboardLayoutName);

        addAction(action);
    }
}

void KeyboardLayoutMenu::onActionTriggerd(QAction* action)
{
    const QString name = action->data().toString();

    setKeyboardLayoutName(name);
    Preferences::setKeyboardLayoutName(name);
    Preferences::self()->writeConfig();
}
