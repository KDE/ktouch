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
    void showPreferences();
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
