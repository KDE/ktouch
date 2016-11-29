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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KMainWindow>

#include <QWeakPointer>
#include <QQuickView>

class KTouchContext;

#ifdef KTOUCH_BUILD_WITH_X11
class XEventNotifier;
#else
class KeyboardLayoutMenu;
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
