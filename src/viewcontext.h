#ifndef VIEWCONTEXT_H
#define VIEWCONTEXT_H

#include <QObject>
#include <QString>
#include <QTime>

class QWidget;

class ViewContext : public QObject
{
    Q_OBJECT
public:
    explicit ViewContext(QWidget* mainWindow, QObject *parent = 0);
    Q_INVOKABLE QString findImage(QString imageName) const;
    Q_INVOKABLE QString formatTime(QTime time);
    Q_INVOKABLE QString formatAccuracy(double accuracy);
private:
    QWidget* m_mainWindow;
};

#endif // VIEWCONTEXT_H
