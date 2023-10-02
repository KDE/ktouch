/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
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
    m_view(new QQuickWidget()),
    m_context(new KTouchContext(this, m_view, this))
{
    init();
}

MainWindow::~MainWindow()
{
}


void MainWindow::init()
{
    const int unit = fontMetrics().height();
    m_view->setMinimumSize(56 * unit, 39 * unit);
    m_view->setFocusPolicy(Qt::StrongFocus);
    setCentralWidget(m_view);

    Application::setupDeclarativeBindings(m_view->engine());

    m_view->connect(m_view, &QQuickWidget::statusChanged, this, &MainWindow::onViewStatusChanged);
    m_view->rootContext()->setContextProperty(QStringLiteral("ktouch"), m_context);
    m_view->setResizeMode(QQuickWidget::SizeRootObjectToView);
    m_view->setSource(QUrl(QStringLiteral("qrc:/ktouch/qml/main.qml")));
}

void MainWindow::onViewStatusChanged(QQuickWidget::Status status)
{
    if (status == QQuickWidget::Error)
    {
        QStringList errorMessages;
        foreach (auto error, m_view->errors())
        {
            errorMessages.append(error.toString());
        }
        QMessageBox qmlErrorMsgBox;
        qmlErrorMsgBox.setText(i18n("%1 has encountered a runtime error and has to be closed.", KAboutData::applicationData().displayName()));
        qmlErrorMsgBox.setDetailedText(errorMessages.join(QLatin1Char('\n')));
        qmlErrorMsgBox.setStandardButtons(QMessageBox::Close);
        qmlErrorMsgBox.setIcon(QMessageBox::Critical);
        qmlErrorMsgBox.exec();
        exit(1);
    }
}

#include "moc_mainwindow.cpp"
