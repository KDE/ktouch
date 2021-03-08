/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef KEYBOARDLAYOUTEDITORVIEW_H
#define KEYBOARDLAYOUTEDITORVIEW_H

#include <QQuickWidget>

class KeyboardLayoutEditorView : public QQuickWidget
{
    Q_OBJECT
public:
    explicit KeyboardLayoutEditorView(QWidget* parent = nullptr);
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // KEYBOARDLAYOUTEDITORVIEW_H
