#include "viewcontext.h"

#include <kstandarddirs.h>
#include <kdebug.h>

ViewContext::ViewContext(QWidget* mainWindow, QObject* parent) :
    QObject(parent),
    m_mainWindow(mainWindow)
{
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
