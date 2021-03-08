/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "newcoursewidget.h"

#include "core/dataindex.h"
#include "models/resourcemodel.h"

NewCourseWidget::NewCourseWidget(ResourceModel* resourceModel, QWidget* parent) :
    QWidget(parent),
    Ui::NewCourseWidget()
{
    setupUi(this);

    connect(m_titleLineEdit, &QLineEdit::textChanged, this, &NewCourseWidget::isValidChanged);
    connect(m_keyboardLayoutComboBox, SIGNAL(currentIndexChanged(int)), SIGNAL(isValidChanged()));

    m_keyboardLayoutComboBox->setResourceModel(resourceModel);
    m_keyboardLayoutComboBox->setCurrentIndex(0);
}

bool NewCourseWidget::isValid() const
{
    if (m_titleLineEdit->text().isEmpty())
        return false;

    if (m_keyboardLayoutComboBox->currentIndex() == -1)
        return false;

    return true;
}

QString NewCourseWidget::title() const
{
    return m_titleLineEdit->text();
}

QString NewCourseWidget::keyboardLayoutName() const
{
    DataIndexKeyboardLayout* const layout = m_keyboardLayoutComboBox->selectedKeyboardLayout();
    return layout? layout->name(): QLatin1String("");
}

QString NewCourseWidget::description() const
{
    return m_descriptionEdit->toPlainText();
}
