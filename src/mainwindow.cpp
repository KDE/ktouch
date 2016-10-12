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

#include "mainwindow.h"

#include <QQuickWidget>
#include <QVariant>
#include <QStandardPaths>
#include <QQmlContext>

#include <KLocalizedString>

#include "application.h"
#include "ktouchcontext.h"

MainWindow::MainWindow(QWidget* parent):
    KMainWindow(parent),
    m_view(new QQuickWidget(this)),
    m_context(new KTouchContext(this, m_view, this))
{
    init();
}

MainWindow::~MainWindow()
{
}


void MainWindow::init()
{

    setCentralWidget(m_view);

    Application::setupDeclarativeBindings(m_view->engine());

    m_view->setMinimumSize(1000, 700);
    m_view->rootContext()->setContextProperty(QStringLiteral("ktouch"), m_context);
    m_view->setResizeMode(QQuickWidget::SizeRootObjectToView);
    m_view->setSource(QUrl::fromLocalFile(QStandardPaths::locate(QStandardPaths::DataLocation, "qml/main.qml")));
}
