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

#include "newkeyboardlayoutwidget.h"

#include "core/dataindex.h"
#include "models/resourcemodel.h"

#include "preferences.h"

#ifdef KTOUCH_BUILD_WITH_X11
#include "x11_helper.h"
#endif


NewKeyboardLayoutWidget::NewKeyboardLayoutWidget(ResourceModel* resourceModel, QWidget* parent) :
    QWidget(parent),
    Ui::NewKeyboardLayoutWidget(),
    m_resourceModel(resourceModel)
{
    setupUi(this);

    connect(m_nameLineEdit, SIGNAL(textChanged(QString)), SLOT(checkName()));
    connect(m_nameLineEdit, SIGNAL(textChanged(QString)), SIGNAL(isValidChanged()));
    connect(m_titleLineEdit, SIGNAL(textChanged(QString)), SIGNAL(isValidChanged()));
    connect(m_pasteCurrentNameButton, SIGNAL(clicked()), SLOT(pasteCurrentName()));

    m_messageWidget->setMessageType(KMessageWidget::Error);
    m_messageWidget->setCloseButtonVisible(false);
    m_messageWidget->hide();
}

QString NewKeyboardLayoutWidget::name() const
{
    return m_nameLineEdit->text();
}

QString NewKeyboardLayoutWidget::title() const
{
    return m_titleLineEdit->text();
}

bool NewKeyboardLayoutWidget::isValid() const
{
    if (m_nameLineEdit->text().isEmpty())
        return false;

    if (m_titleLineEdit->text().isEmpty())
        return false;

    return m_nameIsValid;
}

void NewKeyboardLayoutWidget::pasteCurrentName()
{
#ifdef KTOUCH_BUILD_WITH_X11
    const QString name = X11Helper::getCurrentLayout().toString();
#else
    const QString name = Preferences::keyboardLayoutName();
#endif
    m_nameLineEdit->setText(name);
    m_nameLineEdit->setFocus();
}

void NewKeyboardLayoutWidget::checkName()
{
    const QString name = m_nameLineEdit->text();
    DataIndex* const dataIndex = m_resourceModel->dataIndex();

    m_nameIsValid = true;

    for (int i = 0; i < dataIndex->keyboardLayoutCount(); i++)
    {
        DataIndexKeyboardLayout* const layout = dataIndex->keyboardLayout(i);

        if (layout->source() == DataIndex::UserResource && layout->name() == name)
        {
            m_messageWidget->setText(i18n("There is already a keyboard layout with the same name."));
            m_nameIsValid = false;
            break;
        }
    }

    if (m_nameIsValid)
        m_messageWidget->animatedHide();

    if (!m_nameIsValid)
        m_messageWidget->animatedShow();

    emit isValidChanged();
}
