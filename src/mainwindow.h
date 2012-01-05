#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KMainWindow>

class QDeclarativeView;

class MainWindow : public KMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = 0);
    bool useOpenGLViewport() const;
    void setUseOpenGLViewport(bool useOpenGLViewport);
private:
    void init();
    QDeclarativeView* m_view;
    bool m_useOpenGLViewport;
};

#endif // MAINWINDOW_H
