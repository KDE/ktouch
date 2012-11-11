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


#ifndef CHARACTERSVIEWEDITORFACTORIES_H
#define CHARACTERSVIEWEDITORFACTORIES_H

#include <QItemEditorFactory>

class KeyboardLayout;

class CharacterValueEditorFactory : public QItemEditorFactory
{
public:
    CharacterValueEditorFactory();
    QWidget* createEditor(QVariant::Type type, QWidget* parent) const;
};

class CharacterModifierIdEditorFactory : public QItemEditorFactory
{
public:
    CharacterModifierIdEditorFactory();
    KeyboardLayout* keyboardLayout() const;
    void setKeyboardLayout(KeyboardLayout* keyboardLayout);
    QWidget* createEditor(QVariant::Type type, QWidget* parent) const;
private:
    KeyboardLayout* m_keyboardLayout;
};

class CharacterPositionEditorFactory : public QItemEditorFactory
{
public:
    CharacterPositionEditorFactory();
    QWidget* createEditor(QVariant::Type type, QWidget* parent) const;
};

#endif // CHARACTERVALUEEDITORFACTORY_H
