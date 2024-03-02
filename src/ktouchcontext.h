/*
 *  SPDX-FileCopyrightText: 2016 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef KTOUCHCONTEXT_H
#define KTOUCHCONTEXT_H

#include <QObject>

class QMenu;
class QQuickWidget;

class KActionCollection;
class KMainWindow;

class DataIndex;
class KeyboardLayout;
class KeyboardLayoutMenu;
class Lesson;
class XEventNotifier;

Q_DECLARE_OPAQUE_POINTER(DataIndex *)

class KTouchContext : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString keyboardLayoutName READ keyboardLayoutName NOTIFY keyboardLayoutNameChanged)
    Q_PROPERTY(DataIndex* globalDataIndex READ dataIndex CONSTANT)
    Q_PROPERTY(bool keyboardKCMAvailable READ keyboardKCMAvailable CONSTANT)
public:
    explicit KTouchContext(KMainWindow* mainWindow, QQuickWidget* widget, QObject* parent = nullptr);
    ~KTouchContext() override;
    QString keyboardLayoutName() const;
    DataIndex* dataIndex();
    bool keyboardKCMAvailable();
    Q_INVOKABLE void showMenu(int xPos, int yPos);
    Q_INVOKABLE Lesson* createLesson();
public Q_SLOTS:
    void showResourceEditor();
    void showKeyboardKCM();
    bool showCustomLessonDialog(Lesson* lesson, KeyboardLayout* keyboardLayout);
private Q_SLOTS:
    void showConfigDialog();
    void configureShortcuts();
    void setFullscreen(bool fullscreen);
Q_SIGNALS:
    void keyboardLayoutNameChanged();
private:
    void init();
    bool testKCMAvailibility(const QString& name);
    KActionCollection* m_actionCollection;
    QMenu* m_menu;
    KMainWindow* m_mainWindow;
    QQuickWidget* m_widget;
#ifdef KTOUCH_BUILD_WITH_X11
    XEventNotifier* m_XEventNotifier;
#endif
};

#endif // KTOUCHCONTEXT_H
