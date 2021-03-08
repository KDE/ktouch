/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef ABSTRACTKEY_H
#define ABSTRACTKEY_H

#include <QObject>
#include <QRect>

class AbstractKey : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int left READ left WRITE setLeft NOTIFY leftChanged)
    Q_PROPERTY(int top READ top WRITE setTop NOTIFY topChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)

public:
    explicit AbstractKey(QObject *parent = 0);

    Q_INVOKABLE virtual QString keyType() const;
    int left() const;
    void setLeft(int left);
    int top() const;
    void setTop(int top);
    int width() const;
    void setWidth(int width);
    int height() const;
    void setHeight(int height);
    Q_INVOKABLE void copyFrom(AbstractKey* source);

    QRect rect() const;
    void setRect(const QRect& rect);

signals:
    void leftChanged();
    void topChanged();
    void widthChanged();
    void heightChanged();

private:
    int m_left;
    int m_top;
    int m_width;
    int m_height;
};

#endif // ABSTRACTKEY_H
