/*
 * Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "charactersvieweditorfactories.h"

#include <QCompleter>

#include <KLocale>
#include <KLineEdit>
#include <KComboBox>

#include "core/keyboardlayout.h"
#include "core/specialkey.h"
#include "core/key.h"
#include "core/keychar.h"

CharacterValueEditorFactory::CharacterValueEditorFactory()
{
}

QWidget* CharacterValueEditorFactory::createEditor(QVariant::Type type, QWidget *parent) const
{
    Q_UNUSED(type)

    KLineEdit* widget = new KLineEdit(parent);

    widget->setMaxLength(1);
    widget->setFrame(false);

    return widget;
}

CharacterModifierIdEditorFactory::CharacterModifierIdEditorFactory() :
    m_keyboardLayout(0)
{
}

KeyboardLayout* CharacterModifierIdEditorFactory::keyboardLayout() const
{
    return m_keyboardLayout;
}

void CharacterModifierIdEditorFactory::setKeyboardLayout(KeyboardLayout* keyboardLayout)
{
    m_keyboardLayout = keyboardLayout;
}

QWidget* CharacterModifierIdEditorFactory::createEditor(QVariant::Type type, QWidget *parent) const
{
    Q_UNUSED(type)

    KLineEdit* widget = new KLineEdit(parent);
    QStringList wordList;

    for (int i = 0; i < m_keyboardLayout->keyCount(); i++)
    {
        if (SpecialKey* key = qobject_cast<SpecialKey*>(m_keyboardLayout->key(i)))
        {
            wordList << key->modifierId();
        }
    }

    QCompleter* completer = new QCompleter(wordList, widget);
    widget->setCompleter(completer);
    widget->setFrame(false);

    return widget;
}

CharacterPositionEditorFactory::CharacterPositionEditorFactory()
{
}

QWidget* CharacterPositionEditorFactory::createEditor(QVariant::Type type, QWidget *parent) const
{
    Q_UNUSED(type)

    KComboBox* widget = new KComboBox(parent);

    widget->setEditable(false);
    widget->addItem(i18n("Hidden"), KeyChar::Hidden);
    widget->addItem(i18n("Top left"), KeyChar::TopLeft);
    widget->addItem(i18n("Top right"), KeyChar::TopRight);
    widget->addItem(i18n("Bottom left"), KeyChar::BottomLeft);
    widget->addItem(i18n("Bottom right"), KeyChar::BottomRight);

    return widget;
}
