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
    m_selectedKeyIndex(-1),
    m_selectedKey(0)
{
    setupUi(this);
    setFont(KGlobalSettings::smallestReadableFont());
    connect(KGlobalSettings::self(), SIGNAL(appearanceChanged()), SLOT(updateFont()));
    setSelectedKey(-1);

    connect(m_keyboardLayoutTitleLineEdit, SIGNAL(textEdited(QString)), SLOT(setKeyboardLayoutTitle(QString)));
    connect(m_keyboardLayoutNameEdit, SIGNAL(textEdited(QString)), SLOT(setKeyboardLayoutName(QString)));
    connect(m_keyboardLayoutWidthSpinBox, SIGNAL(valueChanged(int)), SLOT(onKeyboardLayoutWidthChanged(int)));
    connect(m_keyboardLayoutHeightSpinBox, SIGNAL(valueChanged(int)), SLOT(onKeyboardLayoutHeightChanged(int)));
    connect(m_keyLeftSpinBox, SIGNAL(valueChanged(int)), SLOT(onKeyLeftChanged(int)));
    connect(m_keyTopSpinBox, SIGNAL(valueChanged(int)), SLOT(onKeyTopChanged(int)));
    connect(m_keyWidthSpinBox, SIGNAL(valueChanged(int)), SLOT(onKeyWidthChanged(int)));
    connect(m_keyHeightSpinBox, SIGNAL(valueChanged(int)), SLOT(onKeyHeightChanged(int)));
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

        if (qobject_cast<Key*>(m_selectedKey))
        {
            m_subStackedWidget->setCurrentWidget(m_keyPropertiesSubWidget);
        }
        else if (qobject_cast<SpecialKey*>(m_selectedKey))
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

void KeyboardLayoutPropertiesWidget::setKeyGeometry(const QRect& rect)
{
    QUndoCommand* command = new SetKeyGeometryCommand(m_keyboardLayout, m_selectedKeyIndex, rect);
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
