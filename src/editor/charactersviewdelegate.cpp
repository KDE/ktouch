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

#include "charactersviewdelegate.h"

#include <QCompleter>

#include <KComboBox>
#include <KLineEdit>
#include <KLocale>

#include "core/keyboardlayout.h"
#include "core/keychar.h"
#include "core/specialkey.h"

CharactersViewDelegate::CharactersViewDelegate(QObject* parent) :
    QStyledItemDelegate(parent),
    m_keyboardLayout(0)
{
}

KeyboardLayout* CharactersViewDelegate::keyboardLayout() const
{
    return m_keyboardLayout;
}

void CharactersViewDelegate::setKeyboardLayout(KeyboardLayout* keyboardLayout)
{
    m_keyboardLayout = keyboardLayout;
}

QWidget* CharactersViewDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    switch (index.column())
    {
    case 0:
    {
        KLineEdit* lineEdit = new KLineEdit(parent);

        lineEdit->setFrame(false);
        lineEdit->setMaxLength(1);
        return lineEdit;
    }
    case 1:
    {
        KLineEdit* lineEdit = new KLineEdit(parent);

        lineEdit->setFrame(false);

        QStringList wordList;

        for (int i = 0; i < m_keyboardLayout->keyCount(); i++)
        {
            if (SpecialKey* key = qobject_cast<SpecialKey*>(m_keyboardLayout->key(i)))
            {
                wordList << key->modifierId();
            }
        }

        QCompleter* completer = new QCompleter(wordList, lineEdit);
        lineEdit->setCompleter(completer);

        return lineEdit;
    }
    case 2:
    {
        KComboBox* comboBox = new KComboBox(parent);

        comboBox->setEditable(false);
        comboBox->addItem(i18n("Hidden"), KeyChar::Hidden);
        comboBox->addItem(i18n("Top left"), KeyChar::TopLeft);
        comboBox->addItem(i18n("Top right"), KeyChar::TopRight);
        comboBox->addItem(i18n("Bottom left"), KeyChar::BottomLeft);
        comboBox->addItem(i18n("Bottom right"), KeyChar::BottomRight);

        return comboBox;
    }
    default:
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void CharactersViewDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    switch (index.column())
    {
    case 0:
    case 1:
    {
        KLineEdit* lineEdit = static_cast<KLineEdit*>(editor);
        lineEdit->setText(index.data(Qt::EditRole).toString());
        break;
    }
    case 2:
    {
        KComboBox* comboBox = static_cast<KComboBox*>(editor);
        comboBox->setCurrentIndex(index.data(Qt::EditRole).toInt());
        break;
    }
    default:
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void CharactersViewDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    switch (index.column())
    {
    case 0:
    case 1:
    {
        KLineEdit* lineEdit = static_cast<KLineEdit*>(editor);
        model->setData(index, lineEdit->text());
        break;
    }
    case 2:
    {
        KComboBox* comboBox = static_cast<KComboBox*>(editor);
        model->setData(index, comboBox->currentIndex());
        break;
    }
    default:
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}
