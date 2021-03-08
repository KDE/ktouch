/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KMainWindow>

#include <QWeakPointer>
#include <QQuickView>

class KTouchContext;

#ifdef KTOUCH_BUILD_WITH_X11
#else
#endif

class MainWindow : public KMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
private:
    void init();
    void onViewStatusChanged(QQuickView::Status status);
    QQuickView* m_view;
    KTouchContext* m_context;
};

#endif // MAINWINDOW_H
