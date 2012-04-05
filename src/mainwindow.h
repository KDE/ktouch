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

class QDeclarativeView;
class QMenu;
class KActionCollection;

class MainWindow : public KMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = 0);
    bool useOpenGLViewport() const;
    void setUseOpenGLViewport(bool useOpenGLViewport);
private slots:
    void showMenu(int xPos, int yPos);
    void showConfigDialog();
    void configureShortcuts();
    void setFullscreen(bool fullscreen);
private:
    void init();
    QDeclarativeView* m_view;
    KActionCollection* m_actionCollection;
    QMenu* m_menu;
    bool m_useOpenGLViewport;
};

#endif // MAINWINDOW_H
