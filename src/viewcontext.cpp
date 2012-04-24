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
    m_XEventNotifier(new XEventNotifier())
{
    m_XEventNotifier->start();
    connect(m_XEventNotifier, SIGNAL(layoutChanged()), SIGNAL(keyboardLayoutNameChanged()));
}

ViewContext::~ViewContext()
{
    disconnect(this, SIGNAL(keyboardLayoutNameChanged()));
    m_XEventNotifier->stop();
    delete m_XEventNotifier;
}

QString ViewContext::keyboardLayoutName() const
{
    return X11Helper::getCurrentLayout().toString();
}

QString ViewContext::findImage(const QString& imageName) const
{
    QString relPath = QString("images/") + imageName;
    QString path = KGlobal::dirs()->findResource("appdata", relPath);
    if (path.isNull())
    {
        kWarning() << "can't find image resource:" << imageName;
    }
    return path;
}

QString ViewContext::formatTime(const QTime& time)
{
    return time.toString("mm:ss.zzz").left(7);
}

QString ViewContext::formatTimeDiff(const QTime& from, const QTime& to)
{
    const int diff = from.msecsTo(to);
    const QTime diffTime = QTime(0, 0).addMSecs(diff > 0? diff: -diff);
    return QString("%1 %2").arg(formatSign(diff), formatTime(diffTime));
}

QString ViewContext::formatAccuracy(qreal accuracy)
{
    return QString("%1%").arg(100 * accuracy, 0, 'f', 1);
}

QString ViewContext::formatAccuracyDiff(qreal from, qreal to)
{
    const qreal diff = to - from;
    const QString accuracy = formatAccuracy(diff > 0? diff: -diff);
    return QString("%1 %2").arg(formatSign(diff), accuracy);
}

QString ViewContext::formatSign(qreal value)
{
    if (value > 0)
        return "+";
    if (value < 0)
        return "-";
    return QString::fromUtf8("\u00B1");
}

int ViewContext::getSeconds(const QTime& time)
{
    return time.second();
}

int ViewContext::getMinutes(const QTime& time)
{
    return time.minute();
}

void ViewContext::showMenu(int xPos, int yPos)
{
    emit menuRequested(xPos, yPos);
}
