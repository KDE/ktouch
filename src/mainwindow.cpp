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

#include <QVariant>
#include <QStandardPaths>
#include <QQmlContext>
#include <QQmlError>
#include <QMessageBox>

#include <KAboutData>
#include <KLocalizedString>

#include "application.h"
#include "ktouchcontext.h"

MainWindow::MainWindow(QWidget* parent):
    KMainWindow(parent),
    m_view(new QQuickView()),
    m_context(new KTouchContext(this, m_view, this))
{
    init();
}

MainWindow::~MainWindow()
{
}


void MainWindow::init()
{
    QWidget* viewWidget = QWidget::createWindowContainer(m_view, this);
    viewWidget->setMinimumSize(1000, 700);
    viewWidget->setFocusPolicy(Qt::StrongFocus);
    setCentralWidget(viewWidget);

    Application::setupDeclarativeBindings(m_view->engine());

    m_view->connect(m_view, &QQuickView::statusChanged, this, &MainWindow::onViewStatusChanged);
    m_view->rootContext()->setContextProperty(QStringLiteral("ktouch"), m_context);
    m_view->setResizeMode(QQuickView::SizeRootObjectToView);
    m_view->setSource(QUrl("qrc:/qml/main.qml"));
}

void MainWindow::onViewStatusChanged(QQuickView::Status status)
{
    if (status == QQuickView::Error)
    {
        QStringList errorMessages;
        foreach (auto error, m_view->errors())
        {
            errorMessages.append(error.toString());
        }
        QMessageBox qmlErrorMsgBox;
        qmlErrorMsgBox.setText(i18n("%1 has encountered a runtime error and has to be closed.", KAboutData::applicationData().displayName()));
        qmlErrorMsgBox.setDetailedText(errorMessages.join("\n"));
        qmlErrorMsgBox.setStandardButtons(QMessageBox::Close);
        qmlErrorMsgBox.setIcon(QMessageBox::Critical);
        qmlErrorMsgBox.exec();
        exit(1);
    }
}
