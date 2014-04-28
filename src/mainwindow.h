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
class DataIndex;
class ResourceEditor;
class KeyboardLayout;
class Lesson;

#ifdef KTOUCH_BUILD_WITH_X11
class XEventNotifier;
#else
class KeyboardLayoutMenu;
#endif

class MainWindow : public KMainWindow
{
    Q_OBJECT
    Q_PROPERTY(QString keyboardLayoutName READ keyboardLayoutName NOTIFY keyboardLayoutNameChanged)
    Q_PROPERTY(DataIndex* globalDataIndex READ dataIndex CONSTANT)
public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
    QString keyboardLayoutName() const;
    DataIndex* dataIndex();
    bool useOpenGLViewport() const;
    void setUseOpenGLViewport(bool useOpenGLViewport);
    Q_INVOKABLE void showMenu(int xPos, int yPos);
public slots:
    void showResourceEditor();
    bool showCustomLessonDialog(Lesson* lesson, KeyboardLayout* keyboardLayout);
signals:
    void keyboardLayoutNameChanged();
private slots:
    void showConfigDialog();
    void configureShortcuts();
    void configureKeyboard();
    void setFullscreen(bool fullscreen);
private:
    void init();
    bool testKCMAvailibility(const QString& name);
    QDeclarativeView* m_view;
    KActionCollection* m_actionCollection;
    QMenu* m_menu;
    bool m_useOpenGLViewport;
#ifdef KTOUCH_BUILD_WITH_X11
    XEventNotifier* m_XEventNotifier;
#else
    KeyboardLayoutMenu* m_keyboardLayoutMenu;
#endif
};

#endif // MAINWINDOW_H
