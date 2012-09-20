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

class QDeclarativeView;
class QMenu;
class KActionCollection;
class ResourceEditor;
class XEventNotifier;

class MainWindow : public KMainWindow
{
    Q_OBJECT
    Q_PROPERTY(QString keyboardLayoutName READ keyboardLayoutName NOTIFY keyboardLayoutNameChanged)
public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
    QString keyboardLayoutName() const;
    bool useOpenGLViewport() const;
    void setUseOpenGLViewport(bool useOpenGLViewport);
    Q_INVOKABLE void showMenu(int xPos, int yPos);
signals:
    void keyboardLayoutNameChanged();
private slots:
    void showConfigDialog();
    void configureShortcuts();
    void setFullscreen(bool fullscreen);
    void showResourceEditor();
private:
    void init();
    QDeclarativeView* m_view;
    KActionCollection* m_actionCollection;
    QMenu* m_menu;
    QWeakPointer<ResourceEditor> m_resourceEditorRef;
    bool m_useOpenGLViewport;
    XEventNotifier* m_XEventNotifier;
};

#endif // MAINWINDOW_H
