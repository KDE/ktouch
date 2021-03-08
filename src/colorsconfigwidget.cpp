/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "colorsconfigwidget.h"

ColorsConfigWidget::ColorsConfigWidget(QWidget *parent) :
    QWidget(parent),
    Ui::ColorsConfigWidget()
{
    setupUi(this);
}
