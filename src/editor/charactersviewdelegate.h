/*
 * SPDX-FileCopyrightText: 2012 Sebastian Gottfried <sebastiangottfried@web.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef CHARACTERSVIEWDELEGATE_H
#define CHARACTERSVIEWDELEGATE_H

#include <QStyledItemDelegate>

class KeyboardLayout;

class CharactersViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CharactersViewDelegate(QObject* parent = nullptr);
    KeyboardLayout* keyboardLayout() const;
    void setKeyboardLayout(KeyboardLayout* keyboardLayout);
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const  override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel* model, const QModelIndex& index) const override;
private:
    KeyboardLayout* m_keyboardLayout;
};

#endif // CHARACTERSVIEWDELEGATE_H
