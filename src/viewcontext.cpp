/*
 *  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
