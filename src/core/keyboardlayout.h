/*
 *  SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "keyboardlayoutbase.h"

#include <QString>
#include <QVariant>

class AbstractKey;
class DataIndexKeyboardLayout;

class KeyboardLayout : public KeyboardLayoutBase
{
    Q_OBJECT
    Q_PROPERTY(DataIndexKeyboardLayout* associatedDataIndexKeyboardLayout READ associatedDataIndexKeyboardLayout WRITE setAssociatedDataIndexKeyboardLayout NOTIFY associatedDataIndexKeyboardLayoutChanged)
    Q_PROPERTY(AbstractKey* referenceKey READ referenceKey NOTIFY referenceKeyChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int keyCount READ keyCount NOTIFY keyCountChanged)

public:
    explicit KeyboardLayout(QObject* parent = 0);
    DataIndexKeyboardLayout* associatedDataIndexKeyboardLayout() const;
    void setAssociatedDataIndexKeyboardLayout(DataIndexKeyboardLayout* dataIndexKeyboardLayout);
    void setId(const QString& id);
    void setTitle(const QString& title);
    void setName(const QString& name);
    int width() const ;
    void setWidth(int width);
    int height() const;
    void setHeight(int height);
    int keyCount() const;
    Q_INVOKABLE AbstractKey* key(int index) const;
    Q_INVOKABLE int keyIndex(AbstractKey* key) const;
    Q_INVOKABLE void addKey(AbstractKey* key);
    Q_INVOKABLE void insertKey(int index, AbstractKey* key);
    Q_INVOKABLE void removeKey(int index);
    Q_INVOKABLE void clearKeys();
    AbstractKey* referenceKey();
    Q_INVOKABLE void copyFrom(KeyboardLayout* source);
    Q_INVOKABLE QString allCharacters() const;

    QSize size() const;
    void setSize(const QSize& size);

signals:
    void associatedDataIndexKeyboardLayoutChanged();
    void widthChanged();
    void heightChanged();
    void referenceKeyChanged();
    void keyCountChanged();

private slots:
    void onKeyGeometryChanged(int keyIndex);

private:
    void updateReferenceKey(AbstractKey* newKey=0);
    bool compareKeysForReference(const AbstractKey* testKey, const AbstractKey* compareKey) const;
    DataIndexKeyboardLayout* m_associatedDataIndexKeyboardLayout;
    QString m_title;
    QString m_name;
    int m_width;
    int m_height;
    QList<AbstractKey*> m_keys;
    AbstractKey* m_referenceKey;

};

#endif // KEYBOARD_H
