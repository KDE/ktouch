#include "viewcontext.h"

#include <kstandarddirs.h>
#include <kdebug.h>

#include "x11_helper.h"

ViewContext::ViewContext(QWidget* mainWindow, QObject* parent) :
    QObject(parent),
    m_mainWindow(mainWindow),
    m_XEventNotifier(new XEventNotifier(m_mainWindow))
{
    m_XEventNotifier->start();
    connect(m_XEventNotifier, SIGNAL(layoutChanged()), SIGNAL(keyboardLayoutNameChanged()));
}

ViewContext::~ViewContext()
{
    disconnect(this, SIGNAL(keyboardLayoutNameChanged()));
    m_XEventNotifier->stop();
}

QString ViewContext::keyboardLayoutName() const
{
    return X11Helper::getCurrentLayout().toString();
}

QString ViewContext::findImage(QString imageName) const
{
    QString relPath = QString("images/") + imageName;
    QString path = KGlobal::dirs()->findResource("appdata", relPath);
    if (path.isNull())
    {
        kWarning() << "can't find image resource:" << imageName;
    }
    return path;
}

QString ViewContext::formatTime(QTime time)
{
    return time.toString("mm:ss.zzz").left(7);
}

QString ViewContext::formatAccuracy(double accuracy)
{
    return QString("%1%").arg(100 * accuracy, 0, 'f', 1);
}

int ViewContext::getSeconds(QTime time)
{
    return time.second();
}

int ViewContext::getMinutes(QTime time)
{
    return time.minute();
}

void ViewContext::showMenu(int xPos, int yPos)
{
    emit menuRequested(xPos, yPos);
}
