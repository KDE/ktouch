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

#include <KGlobalSettings>
#include <KDebug>

#include "core/keyboardlayout.h"
#include "core/abstractkey.h"
#include "core/key.h"
#include "core/specialkey.h"
#include "editor/keyboardlayoutcommands.h"

KeyboardLayoutPropertiesWidget::KeyboardLayoutPropertiesWidget(QWidget* parent) :
    QWidget(parent),
    m_keyboardLayout(0),
    m_selectedKey(0)
{
    setupUi(this);
    setFont(KGlobalSettings::smallestReadableFont());
    connect(KGlobalSettings::self(), SIGNAL(appearanceChanged()), SLOT(updateFont()));
    setSelectedKey(0);

    connect(m_keyboardLayoutTitleLineEdit, SIGNAL(textEdited(QString)), SLOT(setKeyboardLayoutTitle(QString)));
    connect(m_keyboardLayoutNameEdit, SIGNAL(textEdited(QString)), SLOT(setKeyboardLayoutName(QString)));
    connect(m_keyboardLayoutWidthSpinBox, SIGNAL(valueChanged(int)), SLOT(onKeyboardLayoutWidthChanged(int)));
    connect(m_keyboardLayoutHeightSpinBox, SIGNAL(valueChanged(int)), SLOT(onKeyboardLayoutHeightChanged(int)));
}

void KeyboardLayoutPropertiesWidget::setKeyboardLayout(KeyboardLayout* layout)
{
    if (m_keyboardLayout)
    {
        m_keyboardLayout->disconnect(this);
    }

    m_keyboardLayout = layout;

    connect(m_keyboardLayout, SIGNAL(titleChanged()), SLOT(updateKeyboardLayoutTitle()));
    connect(m_keyboardLayout, SIGNAL(nameChanged()), SLOT(updateKeyboardLayoutName()));
    connect(m_keyboardLayout, SIGNAL(widthChanged()), SLOT(updateKeyboardLayoutWidth()));
    connect(m_keyboardLayout, SIGNAL(heightChanged()), SLOT(updateKeyboardLayoutHeight()));
}

void KeyboardLayoutPropertiesWidget::setUndoStack(QUndoStack* undoStack)
{
    m_undoStack = undoStack;
}

void KeyboardLayoutPropertiesWidget::setSelectedKey(AbstractKey* key)
{
    m_selectedKey = key;

    if (key == 0)
    {
        m_stackedWidget->setCurrentWidget(m_keyboardProperties);
    }
    else
    {
        m_stackedWidget->setCurrentWidget(m_keyProperties);

        if (qobject_cast<Key*>(key))
        {
            m_subStackedWidget->setCurrentWidget(m_keyPropertiesSubWidget);
        }
        else if (qobject_cast<SpecialKey*>(key))
        {
            m_subStackedWidget->setCurrentWidget(m_specialKeyPropertiesSubWidget);
        }
    }
}

void KeyboardLayoutPropertiesWidget::setReadOnly(bool readOnly)
{
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
    m_specialKeyTypeComboBox->setEnabled(readOnly);
    m_specialKeyLabelLineEdit->setReadOnly(!readOnly);
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
