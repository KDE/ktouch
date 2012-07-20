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

#include <KGlobalSettings>
#include <KDebug>

#include "core/abstractkey.h"
#include "core/key.h"
#include "core/specialkey.h"

KeyboardLayoutPropertiesWidget::KeyboardLayoutPropertiesWidget(QWidget* parent) :
    QWidget(parent),
    m_selectedKey(0)
{
    setupUi(this);
    setFont(KGlobalSettings::smallestReadableFont());
    connect(KGlobalSettings::self(), SIGNAL(appearanceChanged()), SLOT(updateFont()));
    setSelectedKey(0);
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

