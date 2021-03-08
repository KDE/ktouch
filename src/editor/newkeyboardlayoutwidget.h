/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef NEWKEYBOARDLAYOUTWIDGET_H
#define NEWKEYBOARDLAYOUTWIDGET_H

#include <QWidget>
#include "ui_newkeyboardlayoutwidget.h"

class ResourceModel;

class NewKeyboardLayoutWidget : public QWidget, private Ui::NewKeyboardLayoutWidget
{
    Q_OBJECT
public:
    explicit NewKeyboardLayoutWidget(ResourceModel* resourceModel, QWidget* parent = 0);
    QString name() const;
    QString title() const;
    bool isValid() const;
signals:
    void isValidChanged();
private slots:
    void pasteCurrentName();
    void checkName();
private:
    bool m_nameIsValid;
    ResourceModel* m_resourceModel;
};

#endif // NEWKEYBOARDLAYOUTWIDGET_H
