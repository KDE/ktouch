/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */


#include "keyboardlayouteditorview.h"

#include <QMouseEvent>

KeyboardLayoutEditorView::KeyboardLayoutEditorView(QWidget* parent) :
    QQuickWidget(parent)
{
}

void KeyboardLayoutEditorView::mousePressEvent(QMouseEvent* event)
{
    QQuickWidget::mousePressEvent(event);

    if (!event->isAccepted() && event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
}
