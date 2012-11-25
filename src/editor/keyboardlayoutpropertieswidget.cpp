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

#include "keyboardlayoutpropertieswidget.h"

#include <QUndoStack>
#include <QStyledItemDelegate>

#include <KGlobalSettings>
#include <KDebug>

#include "core/keyboardlayout.h"
#include "core/abstractkey.h"
#include "core/key.h"
#include "core/specialkey.h"
#include "undocommands/keyboardlayoutcommands.h"
#include "models/charactersmodel.h"
#include "editor/charactersviewdelegate.h"

KeyboardLayoutPropertiesWidget::KeyboardLayoutPropertiesWidget(QWidget* parent) :
    QWidget(parent),
    m_keyboardLayout(0),
    m_selectedKeyIndex(-1),
    m_selectedKey(0),
    m_readOnly(false),
    m_undoStack(0),
    m_charactersModel(new CharactersModel(this)),
    m_charactersViewDelegate(new CharactersViewDelegate(this))
{
    setupUi(this);
    setFont(KGlobalSettings::smallestReadableFont());
    connect(KGlobalSettings::self(), SIGNAL(appearanceChanged()), SLOT(updateFont()));
    setSelectedKey(-1);

    m_charactersView->setModel(m_charactersModel);
    m_charactersView->setItemDelegate(m_charactersViewDelegate);
    m_charactersView->verticalHeader()->setDefaultSectionSize(m_charactersView->verticalHeader()->minimumSectionSize() + 6);

    connect(m_charactersView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(onCharacterSelected()));
    connect(m_keyboardLayoutTitleLineEdit, SIGNAL(textEdited(QString)), SLOT(setKeyboardLayoutTitle(QString)));
    connect(m_keyboardLayoutNameEdit, SIGNAL(textEdited(QString)), SLOT(setKeyboardLayoutName(QString)));
    connect(m_keyboardLayoutWidthSpinBox, SIGNAL(valueChanged(int)), SLOT(onKeyboardLayoutWidthChanged(int)));
    connect(m_keyboardLayoutHeightSpinBox, SIGNAL(valueChanged(int)), SLOT(onKeyboardLayoutHeightChanged(int)));
    connect(m_keyLeftSpinBox, SIGNAL(valueChanged(int)), SLOT(onKeyLeftChanged(int)));
    connect(m_keyTopSpinBox, SIGNAL(valueChanged(int)), SLOT(onKeyTopChanged(int)));
    connect(m_keyWidthSpinBox, SIGNAL(valueChanged(int)), SLOT(onKeyWidthChanged(int)));
    connect(m_keyHeightSpinBox, SIGNAL(valueChanged(int)), SLOT(onKeyHeightChanged(int)));
    connect(m_keyFingerComboBox, SIGNAL(currentIndexChanged(int)), SLOT(onFingerIndexChanged(int)));
    connect(m_keyHapticMarkerCheckBox, SIGNAL(clicked(bool)), SLOT(setKeyHasHapticMarker(bool)));
    connect(m_addCharacterButton, SIGNAL(clicked()), SLOT(addCharacter()));
    connect(m_removeCharacterButton, SIGNAL(clicked()), SLOT(removeCharacter()));
    connect(m_specialKeyTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(onSpecialKeyTypeChanged(int)));
    connect(m_specialKeyLabelLineEdit, SIGNAL(textEdited(QString)), SLOT(setSpecialKeyLabel(QString)));
    connect(m_specialKeyModifierIdLineEdit, SIGNAL(textEdited(QString)), SLOT(setSpecialKeyModifierId(QString)));
}

void KeyboardLayoutPropertiesWidget::setKeyboardLayout(KeyboardLayout* layout)
{
    if (m_keyboardLayout)
    {
        m_keyboardLayout->disconnect(this);
    }

    m_keyboardLayout = layout;

    m_charactersModel->setKeyboardLayout(layout);
    m_charactersViewDelegate->setKeyboardLayout(layout);

    connect(m_keyboardLayout, SIGNAL(titleChanged()), SLOT(updateKeyboardLayoutTitle()));
    connect(m_keyboardLayout, SIGNAL(nameChanged()), SLOT(updateKeyboardLayoutName()));
    connect(m_keyboardLayout, SIGNAL(widthChanged()), SLOT(updateKeyboardLayoutWidth()));
    connect(m_keyboardLayout, SIGNAL(heightChanged()), SLOT(updateKeyboardLayoutHeight()));
}

void KeyboardLayoutPropertiesWidget::setUndoStack(QUndoStack* undoStack)
{
    m_undoStack = undoStack;
    m_charactersModel->setUndoStack(undoStack);
}

void KeyboardLayoutPropertiesWidget::setSelectedKey(int index)
{
    if (m_selectedKey)
    {
        m_selectedKey->disconnect(this);
    }

    m_selectedKeyIndex = index;

    if (index == -1)
    {
        m_selectedKey = 0;

        m_charactersModel->setKeyIndex(-1);

        m_stackedWidget->setCurrentWidget(m_keyboardProperties);
    }
    else
    {
        m_selectedKey = 0;
        resetKeyGeometry(m_keyboardLayout->key(index));
        m_selectedKey = m_keyboardLayout->key(index);

        m_stackedWidget->setCurrentWidget(m_keyProperties);

        connect(m_selectedKey, SIGNAL(leftChanged()), SLOT(updateKeyLeft()));
        connect(m_selectedKey, SIGNAL(topChanged()), SLOT(updateKeyTop()));
        connect(m_selectedKey, SIGNAL(widthChanged()), SLOT(updateKeyWidth()));
        connect(m_selectedKey, SIGNAL(heightChanged()), SLOT(updateKeyHeight()));

        if (Key* key = qobject_cast<Key*>(m_selectedKey))
        {
            m_subStackedWidget->setCurrentWidget(m_keyPropertiesSubWidget);

            m_keyHapticMarkerCheckBox->setChecked(key->hasHapticMarker());
            m_keyFingerComboBox->setCurrentIndex(key->fingerIndex());

            m_charactersModel->setKeyIndex(index);

            m_removeCharacterButton->setEnabled(false);

            if (key->keyCharCount() > 0)
            {
                m_charactersView->selectRow(0);
            }

            connect(key, SIGNAL(fingerIndexChanged()), SLOT(updateKeyFingerIndex()));
            connect(key, SIGNAL(hasHapticMarkerChanged()), SLOT(updateKeyHasHapticMarker()));
        }
        else if (SpecialKey* specialKey = qobject_cast<SpecialKey*>(m_selectedKey))
        {
            m_subStackedWidget->setCurrentWidget(m_specialKeyPropertiesSubWidget);

            m_specialKeyTypeComboBox->setCurrentIndex(specialKey->type());
            m_specialKeyLabelLineEdit->setText(specialKey->label());
            m_specialKeyLabelLineEdit->setEnabled(specialKey->type() == SpecialKey::Other);
            m_specialKeyModifierIdLineEdit->setText(specialKey->modifierId());

            m_charactersModel->setKeyIndex(-1);

            connect(specialKey, SIGNAL(typeChanged()), SLOT(updateSpecialKeyType()));
            connect(specialKey, SIGNAL(labelChanged()), SLOT(updateSpecialKeyLabel()));
            connect(specialKey, SIGNAL(modifierIdChanged()), SLOT(updateSpecialKeyModifierId()));
        }
    }
}

void KeyboardLayoutPropertiesWidget::setReadOnly(bool readOnly)
{
    m_readOnly = readOnly;

    m_keyboardLayoutTitleLineEdit->setReadOnly(readOnly);
    m_keyboardLayoutNameEdit->setReadOnly(readOnly);
    m_keyboardLayoutWidthSpinBox->setReadOnly(readOnly);
    m_keyboardLayoutHeightSpinBox->setReadOnly(readOnly);
    m_keyFingerComboBox->setEnabled(!readOnly);
    m_keyHapticMarkerCheckBox->setEnabled(!readOnly);
    m_keyLeftSpinBox->setReadOnly(readOnly);
    m_keyTopSpinBox->setReadOnly(readOnly);
    m_keyWidthSpinBox->setReadOnly(readOnly);
    m_keyHeightSpinBox->setReadOnly(readOnly);
    m_charactersView->setEditTriggers(readOnly? QAbstractItemView::NoEditTriggers: QAbstractItemView::DoubleClicked	| QAbstractItemView::EditKeyPressed);
    m_addCharacterButton->setEnabled(!readOnly);
    m_specialKeyTypeComboBox->setEnabled(!readOnly);
    m_specialKeyLabelLineEdit->setReadOnly(readOnly);
    m_specialKeyModifierIdLineEdit->setReadOnly(readOnly);
}

void KeyboardLayoutPropertiesWidget::updateFont()
{
    setFont(KGlobalSettings::smallestReadableFont());
}

void KeyboardLayoutPropertiesWidget::setKeyboardLayoutTitle(const QString& title)
{
    QUndoCommand* command = new SetKeyboardLayoutTitleCommand(m_keyboardLayout, title);
    m_undoStack->push(command);
}

void KeyboardLayoutPropertiesWidget::setKeyboardLayoutName(const QString& name)
{
    QUndoCommand* command = new SetKeyboardLayoutNameCommand(m_keyboardLayout, name);
    m_undoStack->push(command);
}

void KeyboardLayoutPropertiesWidget::setKeyboardLayoutSize(const QSize& size)
{
    QUndoCommand* command = new SetKeyboardLayoutSizeCommand(m_keyboardLayout, size);
    m_undoStack->push(command);
}

void KeyboardLayoutPropertiesWidget::setKeyGeometry(const QRect& rect)
{
    QUndoCommand* command = new SetKeyGeometryCommand(m_keyboardLayout, m_selectedKeyIndex, rect);
    m_undoStack->push(command);
}

void KeyboardLayoutPropertiesWidget::setKeyFingerIndex(int fingerIndex)
{
    QUndoCommand* command = new SetKeyFingerIndexCommand(m_keyboardLayout, m_selectedKeyIndex, fingerIndex);
    m_undoStack->push(command);
}

void KeyboardLayoutPropertiesWidget::setKeyHasHapticMarker(bool hasHapticMarker)
{
    QUndoCommand* command = new SetKeyHasHapticMarkerCommand(m_keyboardLayout, m_selectedKeyIndex, hasHapticMarker);
    m_undoStack->push(command);
}

void KeyboardLayoutPropertiesWidget::addCharacter()
{
    QUndoCommand* command = new AddKeyCharCommand(m_keyboardLayout, m_selectedKeyIndex);
    m_undoStack->push(command);
    const int row = m_charactersModel->rowCount() - 1;
    m_charactersView->selectRow(row);
    m_charactersView->edit(m_charactersModel->index(row, 0));
}

void KeyboardLayoutPropertiesWidget::removeCharacter()
{
    const int index = m_charactersView->selectionModel()->selectedRows().first().row();
    QUndoCommand* command = new RemoveKeyCharCommand(m_keyboardLayout, m_selectedKeyIndex, index);
    m_undoStack->push(command);

    if (m_charactersModel->rowCount() > 0)
    {
        const int row = qMin(index, m_charactersModel->rowCount() - 1);
        m_charactersView->selectRow(row);
    }
}

void KeyboardLayoutPropertiesWidget::setSpecialKeyType(int type)
{
    QUndoCommand* command = new SetSpecialKeyTypeCommand(m_keyboardLayout, m_selectedKeyIndex, static_cast<SpecialKey::Type>(type));
    m_undoStack->push(command);
}

void KeyboardLayoutPropertiesWidget::setSpecialKeyLabel(const QString& label)
{
    QUndoCommand* command = new SetSpecialKeyLabelCommand(m_keyboardLayout, m_selectedKeyIndex, label);
    m_undoStack->push(command);
}

void KeyboardLayoutPropertiesWidget::setSpecialKeyModifierId(const QString& id)
{
    QUndoCommand* command = new SetSpecialKeyModifierIdCommand(m_keyboardLayout, m_selectedKeyIndex, id);
    m_undoStack->push(command);
}

void KeyboardLayoutPropertiesWidget::updateKeyboardLayoutTitle()
{
    const QString title = m_keyboardLayout->title();

    if (title != m_keyboardLayoutTitleLineEdit->text())
    {
        m_keyboardLayoutTitleLineEdit->setText(title);
    }
}

void KeyboardLayoutPropertiesWidget::updateKeyboardLayoutName()
{
    const QString name = m_keyboardLayout->name();

    if (name != m_keyboardLayoutNameEdit->text())
    {
        m_keyboardLayoutNameEdit->setText(name);
    }
}

void KeyboardLayoutPropertiesWidget::updateKeyboardLayoutWidth()
{
    const int width = m_keyboardLayout->width();

    if (width != m_keyboardLayoutWidthSpinBox->value())
    {
        m_keyboardLayoutWidthSpinBox->setValue(width);
    }
}

void KeyboardLayoutPropertiesWidget::updateKeyboardLayoutHeight()
{
    const int height = m_keyboardLayout->height();

    if (height != m_keyboardLayoutWidthSpinBox->value())
    {
        m_keyboardLayoutHeightSpinBox->setValue(height);
    }
}

void KeyboardLayoutPropertiesWidget::updateKeyLeft()
{
    Q_ASSERT(m_selectedKey);

    const int left = m_selectedKey->left();

    m_keyLeftSpinBox->setMaximum(m_keyboardLayout->width() - m_selectedKey->width());

    if (left != m_keyLeftSpinBox->value())
    {
        m_keyLeftSpinBox->setValue(left);
    }

    m_keyWidthSpinBox->setMaximum(m_keyboardLayout->width() - left);
}

void KeyboardLayoutPropertiesWidget::updateKeyTop()
{
    Q_ASSERT(m_selectedKey);

    const int top = m_selectedKey->top();

    m_keyTopSpinBox->setMaximum(m_keyboardLayout->height() - m_selectedKey->height());

    if (top != m_keyTopSpinBox->value())
    {
        m_keyTopSpinBox->setValue(top);
    }

    m_keyHeightSpinBox->setMaximum(m_keyboardLayout->height() - top);
}

void KeyboardLayoutPropertiesWidget::updateKeyWidth()
{
    Q_ASSERT(m_selectedKey);

    const int width = m_selectedKey->width();

    m_keyWidthSpinBox->setMaximum(m_keyboardLayout->width() - m_selectedKey->left());

    if (width != m_keyWidthSpinBox->value())
    {
        m_keyWidthSpinBox->setValue(width);
    }

    m_keyLeftSpinBox->setMaximum(m_keyboardLayout->width() - width);
}

void KeyboardLayoutPropertiesWidget::updateKeyHeight()
{
    Q_ASSERT(m_selectedKey);

    const int height = m_selectedKey->height();

    m_keyHeightSpinBox->setMaximum(m_keyboardLayout->height() - m_selectedKey->top());

    if (height != m_keyHeightSpinBox->value())
    {
        m_keyHeightSpinBox->setValue(height);
    }

    m_keyTopSpinBox->setMaximum(m_keyboardLayout->height() - height);
}

void KeyboardLayoutPropertiesWidget::resetKeyGeometry(AbstractKey* key)
{
    m_keyLeftSpinBox->setMaximum(m_keyboardLayout->width() - key->width());
    m_keyTopSpinBox->setMaximum(m_keyboardLayout->height() - key->height());
    m_keyWidthSpinBox->setMaximum(m_keyboardLayout->width() - key->left());
    m_keyHeightSpinBox->setMaximum(m_keyboardLayout->height() - key->top());
    m_keyTopSpinBox->setValue(key->top());
    m_keyLeftSpinBox->setValue(key->left());
    m_keyWidthSpinBox->setValue(key->width());
    m_keyHeightSpinBox->setValue(key->height());
}

void KeyboardLayoutPropertiesWidget::updateKeyFingerIndex()
{
    Key* const key = qobject_cast<Key*>(m_selectedKey);

    Q_ASSERT(key);

    m_keyFingerComboBox->setCurrentIndex(key->fingerIndex());
}

void KeyboardLayoutPropertiesWidget::updateKeyHasHapticMarker()
{
    Key* const key = qobject_cast<Key*>(m_selectedKey);

    Q_ASSERT(key);

    m_keyHapticMarkerCheckBox->setChecked(key->hasHapticMarker());
}

void KeyboardLayoutPropertiesWidget::updateSpecialKeyType()
{
    SpecialKey* const specialKey = qobject_cast<SpecialKey*>(m_selectedKey);

    Q_ASSERT(specialKey);

    m_specialKeyTypeComboBox->setCurrentIndex(specialKey->type());
}

void KeyboardLayoutPropertiesWidget::updateSpecialKeyLabel()
{
    SpecialKey* const specialKey = qobject_cast<SpecialKey*>(m_selectedKey);

    Q_ASSERT(specialKey);

    const QString label = specialKey->label();

    if (label != m_specialKeyLabelLineEdit->text())
    {
        m_specialKeyLabelLineEdit->setText(label);
    }
}

void KeyboardLayoutPropertiesWidget::updateSpecialKeyModifierId()
{
    SpecialKey* const specialKey = qobject_cast<SpecialKey*>(m_selectedKey);

    Q_ASSERT(specialKey);

    const QString modifierId = specialKey->modifierId();

    if (modifierId != m_specialKeyModifierIdLineEdit->text())
    {
        m_specialKeyModifierIdLineEdit->setText(modifierId);
    }
}

void KeyboardLayoutPropertiesWidget::onKeyboardLayoutWidthChanged(int width)
{
    if (width != m_keyboardLayout->width())
    {
        setKeyboardLayoutSize(QSize(width, m_keyboardLayout->height()));
    }
}

void KeyboardLayoutPropertiesWidget::onKeyboardLayoutHeightChanged(int height)
{
    if (height != m_keyboardLayout->height())
    {
        setKeyboardLayoutSize(QSize(m_keyboardLayout->width(), height));
    }
}

void KeyboardLayoutPropertiesWidget::onKeyLeftChanged(int left)
{
    if (!m_selectedKey)
        return;

    if (left != m_selectedKey->left())
    {
        QRect rect = m_selectedKey->rect();
        rect.moveLeft(left);
        setKeyGeometry(rect);
    }
}

void KeyboardLayoutPropertiesWidget::onKeyTopChanged(int top)
{
    if (!m_selectedKey)
        return;

    if (top != m_selectedKey->top())
    {
        QRect rect = m_selectedKey->rect();
        rect.moveTop(top);
        setKeyGeometry(rect);
    }
}

void KeyboardLayoutPropertiesWidget::onKeyWidthChanged(int width)
{
    if (!m_selectedKey)
        return;

    if (width != m_selectedKey->width())
    {
        QRect rect = m_selectedKey->rect();
        rect.setWidth(width);
        setKeyGeometry(rect);
    }
}

void KeyboardLayoutPropertiesWidget::onKeyHeightChanged(int height)
{
    if (!m_selectedKey)
        return;

    if (height != m_selectedKey->height())
    {
        QRect rect = m_selectedKey->rect();
        rect.setHeight(height);
        setKeyGeometry(rect);
    }
}

void KeyboardLayoutPropertiesWidget::onFingerIndexChanged(int fingerIndex)
{
    Key* const key = qobject_cast<Key*>(m_selectedKey);

    Q_ASSERT(key);

    if (fingerIndex != key->fingerIndex())
    {
        setKeyFingerIndex(fingerIndex);
    }
}

void KeyboardLayoutPropertiesWidget::onCharacterSelected()
{
    m_removeCharacterButton->setEnabled(!m_readOnly && m_charactersView->selectionModel()->hasSelection());
}

void KeyboardLayoutPropertiesWidget::onSpecialKeyTypeChanged(int type)
{
    SpecialKey* const specialKey = qobject_cast<SpecialKey*>(m_selectedKey);

    Q_ASSERT(specialKey);

    if (type != specialKey->type())
    {
        setSpecialKeyType(type);
    }
}
