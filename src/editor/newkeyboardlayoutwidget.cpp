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
#include "x11_helper.h"

NewKeyboardLayoutWidget::NewKeyboardLayoutWidget(QWidget* parent) :
    QWidget(parent),
    Ui::NewKeyboardLayoutWidget()
{
    setupUi(this);

    connect(m_nameLineEdit, SIGNAL(textChanged(QString)), SIGNAL(isValidChanged()));
    connect(m_titleLineEdit, SIGNAL(textChanged(QString)), SIGNAL(isValidChanged()));
    connect(m_pasteCurrentNameButton, SIGNAL(clicked()), SLOT(pasteCurrentName()));
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

    return true;
}

void NewKeyboardLayoutWidget::pasteCurrentName()
{
    const LayoutUnit currentKeyboardLayout = X11Helper::getCurrentLayout();
    m_nameLineEdit->setText(currentKeyboardLayout.toString());
    m_nameLineEdit->setFocus();
}
