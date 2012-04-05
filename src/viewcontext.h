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
