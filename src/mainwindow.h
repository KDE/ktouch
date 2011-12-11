#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KMainWindow>

class QDeclarativeView;

class MainWindow : public KMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = 0);
private:
    void init();
    QDeclarativeView* m_view;
};

#endif // MAINWINDOW_H
