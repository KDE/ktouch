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

#include "utils.h"

#include <QScriptContext>
#include <QTime>
#include <QUuid>

#include <KStandardDirs>
#include <KDebug>

QScriptValue findImage(QScriptContext *context, QScriptEngine *engine)
{
    Q_UNUSED(engine);

    if (context->argumentCount() == 0)
    {
        kWarning() << "got no arguments, expected one";
        return QScriptValue("");
    }

    if (context->argumentCount() > 1)
    {
        kWarning() << "expected one argument, got more";
    }

    const QString imageName = context->argument(0).toString();
    const QString relPath = QString("images/") + imageName;
    const QString path = KGlobal::dirs()->findResource("appdata", relPath);

    if (path.isNull())
    {
        kWarning() << "can't find image resource:" << imageName;
    }

    return QScriptValue(path);
}


QScriptValue getSecondsOfQTime(QScriptContext *context, QScriptEngine *engine)
{
    Q_UNUSED(engine);

    if (context->argumentCount() == 0)
    {
        kWarning() << "got no arguments, expected one";
        return QScriptValue("");
    }

    if (context->argumentCount() > 1)
    {
        kWarning() << "expected one argument, got more";
    }

    const QTime time = context->argument(0).toVariant().toTime();

    if (!time.isValid())
    {
        kWarning() << "invalid QTime passed";
        return QScriptValue(0);
    }

    return QScriptValue(time.second());
}

QScriptValue getMinutesOfQTime(QScriptContext *context, QScriptEngine *engine)
{
    Q_UNUSED(engine);

    if (context->argumentCount() == 0)
    {
        kWarning() << "got no arguments, expected one";
        return QScriptValue("");
    }

    if (context->argumentCount() > 1)
    {
        kWarning() << "expected one argument, got more";
    }

    const QTime time = context->argument(0).toVariant().toTime();

    if (!time.isValid())
    {
        kWarning() << "invalid QTime passed";
        return QScriptValue(0);
    }

    return QScriptValue(time.minute());
}

QScriptValue uuid(QScriptContext *context, QScriptEngine *engine)
{
    Q_UNUSED(engine)

    if (context->argumentCount() > 0)
    {
        kWarning() << "uuid() expects no arguments, got more";
    }

    const QString uuid = QUuid::createUuid().toString();

    return QScriptValue(uuid);
}
