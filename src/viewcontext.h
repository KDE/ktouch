#ifndef VIEWCONTEXT_H
#define VIEWCONTEXT_H

#include <QObject>

class QWidget;

class ViewContext : public QObject
{
    Q_OBJECT
public:
    explicit ViewContext(QWidget* mainWindow, QObject *parent = 0);
    Q_INVOKABLE QString findImage(QString imageName) const;
private:
    QWidget* m_mainWindow;
};

#endif // VIEWCONTEXT_H
