/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "newkeyboardlayoutwidget.h"

#include <KLocalizedString>

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

    connect(m_nameLineEdit, &QLineEdit::textChanged, this, &NewKeyboardLayoutWidget::checkName);
    connect(m_nameLineEdit, &QLineEdit::textChanged, this, &NewKeyboardLayoutWidget::isValidChanged);
    connect(m_titleLineEdit, &QLineEdit::textChanged, this, &NewKeyboardLayoutWidget::isValidChanged);
    connect(m_pasteCurrentNameButton, &QAbstractButton::clicked, this, &NewKeyboardLayoutWidget::pasteCurrentName);

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
