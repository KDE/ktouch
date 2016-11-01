/*
 *  Copyright 2016  Sebastian Gottfried <sebastian.gottfried@posteo.de>
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

#ifndef KTOUCHCONTEXT_H
#define KTOUCHCONTEXT_H

#include <QObject>

class QMenu;
class QQuickView;

class KActionCollection;
class KMainWindow;

class DataIndex;
class KeyboardLayout;
class KeyboardLayoutMenu;
class Lesson;
class XEventNotifier;

class KTouchContext : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString keyboardLayoutName READ keyboardLayoutName NOTIFY keyboardLayoutNameChanged)
    Q_PROPERTY(DataIndex* globalDataIndex READ dataIndex CONSTANT)
public:
    explicit KTouchContext(KMainWindow* mainWindow, QQuickView* view, QObject* parent = 0);
    ~KTouchContext();
    QString keyboardLayoutName() const;
    DataIndex* dataIndex();
    Q_INVOKABLE void showMenu(int xPos, int yPos);
public slots:
    void showResourceEditor();
    bool showCustomLessonDialog(Lesson* lesson, KeyboardLayout* keyboardLayout);
private slots:
    void showConfigDialog();
    void configureShortcuts();
    void configureKeyboard();
    void setFullscreen(bool fullscreen);
signals:
    void keyboardLayoutNameChanged();
private:
    void init();
    bool testKCMAvailibility(const QString& name);
    KActionCollection* m_actionCollection;
    QMenu* m_menu;
    KMainWindow* m_mainWindow;
    QQuickView* m_view;
#ifdef KTOUCH_BUILD_WITH_X11
    XEventNotifier* m_XEventNotifier;
#else
    KeyboardLayoutMenu* m_keyboardLayoutMenu;
#endif
};

#endif // KTOUCHCONTEXT_H
