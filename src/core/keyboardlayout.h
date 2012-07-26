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

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "keyboardlayoutbase.h"

#include <QString>
#include <QVariant>

class QSignalMapper;
class QXmlSchema;
class QDomDocument;
class QIODevice;
class AbstractKey;

class KeyboardLayout : public KeyboardLayoutBase
{
    Q_OBJECT
    Q_PROPERTY(AbstractKey* referenceKey READ referenceKey NOTIFY referenceKeyChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int keyCount READ keyCount NOTIFY keyCountChanged)

public:
    KeyboardLayout(QObject* parent = 0);
    int width() const ;
    void setWidth(int width);
    int height() const;
    void setHeight(int height);
    int keyCount() const;
    Q_INVOKABLE AbstractKey* key(int index) const;
    Q_INVOKABLE int keyIndex(AbstractKey* key) const;
    Q_INVOKABLE void addKey(AbstractKey* key);
    Q_INVOKABLE void removeKey(int index);
    Q_INVOKABLE void clearKeys();
    AbstractKey* referenceKey();
    Q_INVOKABLE void copyFrom(KeyboardLayout* source);

    QSize size() const;
    void setSize(const QSize& size);

signals:
    void widthChanged();
    void heightChanged();
    void referenceKeyChanged();
    void keyCountChanged();

private slots:
    void onKeyGeometryChanged(int keyIndex);

private:
    void updateReferenceKey(AbstractKey* newKey=0);
    bool compareKeysForReference(const AbstractKey* testKey, const AbstractKey* compareKey) const;

    QString m_title;
    QString m_name;
    int m_width;
    int m_height;
    QList<AbstractKey*> m_keys;
    AbstractKey* m_referenceKey;
    QSignalMapper* m_signalMapper;

};

#endif // KEYBOARD_H
