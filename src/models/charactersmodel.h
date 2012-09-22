/*
 * Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CHARACTERSMODEL_H
#define CHARACTERSMODEL_H

#include <QAbstractTableModel>

class QSignalMapper;
class Key;
class KeyChar;

class CharactersModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(Key* key READ key WRITE setKey NOTIFY keyChanged)
public:
    explicit CharactersModel(QObject *parent = 0);
    Key* key() const;
    void setKey(Key* key);
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
signals:
    void keyChanged();
private slots:
    void onKeyCharAboutToBeAdded(KeyChar* keyChar, int index);
    void onKeyCharAdded();
    void onKeyCharsAboutToBeRemoved(int first, int last);
    void onKeyCharsRemoved();
    void emitCharacterChanged(int row);
private:
    void updateMappings();
    QVariant characterData(KeyChar* keyChar, int role) const;
    QVariant modifierIdData(KeyChar* keyChar, int role) const;
    QVariant positionData(KeyChar* keyChar, int role) const;
    Key* m_key;
    QSignalMapper* m_signalMapper;
};

#endif // CHARACTERSMODEL_H
