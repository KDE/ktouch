#ifndef VIEWCONTEXT_H
#define VIEWCONTEXT_H

#include <QObject>
#include <QString>
#include <QTime>

class QWidget;
class XEventNotifier;

class ViewContext : public QObject
{
    Q_OBJECT
    Q_PROPERTY (QString keyboardLayoutName READ keyboardLayoutName NOTIFY keyboardLayoutNameChanged)
public:
    ViewContext(QWidget* mainWindow, QObject *parent = 0);
    ~ViewContext();
    QString keyboardLayoutName() const;
    Q_INVOKABLE QString findImage(QString imageName) const;
    Q_INVOKABLE QString formatTime(QTime time);
    Q_INVOKABLE QString formatAccuracy(double accuracy);
    Q_INVOKABLE int getSeconds(QTime time);
    Q_INVOKABLE int getMinutes(QTime time);
    Q_INVOKABLE void showMenu(int xPos, int yPos);
signals:
    void keyboardLayoutNameChanged();
    void menuRequested(int xPos, int yPos);
private:
    QWidget* m_mainWindow;
    XEventNotifier* m_XEventNotifier;
};

#endif // VIEWCONTEXT_H
