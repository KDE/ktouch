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
