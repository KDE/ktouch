/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef COURSEBASE_H
#define COURSEBASE_H

#include "resource.h"

#include <QString>

class CourseBase : public Resource
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString keyboardLayoutName READ keyboardLayoutName WRITE setKeyboardLayoutName NOTIFY keyboardLayoutNameChanged)

public:
    explicit CourseBase(QObject* parent = 0);

    QString title() const;
    void setTitle(const QString& title);
    QString description() const;
    void setDescription(const QString& description);
    QString keyboardLayoutName() const;
    void setKeyboardLayoutName(const QString& keyboardLayoutName);

signals:

    void titleChanged();
    void descriptionChanged();
    void keyboardLayoutNameChanged();

private:
    QString m_title;
    QString m_description;
    QString m_keyboardLayoutName;

};

#endif // COURSEBASE_H
