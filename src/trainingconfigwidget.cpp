/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "trainingconfigwidget.h"

TrainingConfigWidget::TrainingConfigWidget(QWidget *parent) :
    QWidget(parent),
    Ui::TrainingConfigWidget()
{
    setupUi(this);
}
