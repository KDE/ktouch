/*
 * SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef CHARACTERSMODEL_H
#define CHARACTERSMODEL_H

#include <QAbstractTableModel>

class QUndoStack;
class KeyboardLayout;
class Key;
class KeyChar;

class CharactersModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(KeyboardLayout* keyboardLayout READ keyboardLayout WRITE setKeyboardLayout NOTIFY keyboardLayoutChanged)
    Q_PROPERTY(int keyIndex READ keyIndex WRITE setKeyIndex NOTIFY keyIndexChanged)
public:
    explicit CharactersModel(QObject *parent = nullptr);
    KeyboardLayout* keyboardLayout() const;
    void setKeyboardLayout(KeyboardLayout* keyboardLayout);
    int keyIndex() const;
    void setKeyIndex(int keyIndex);
    QUndoStack* undoStack() const;
    void setUndoStack(QUndoStack* undoStack);
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
signals:
    void keyboardLayoutChanged();
    void keyIndexChanged();
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
    KeyboardLayout* m_keyboardLayout;
    int m_keyIndex;
    Key* m_key;
    QUndoStack* m_undoStack;
};

#endif // CHARACTERSMODEL_H
