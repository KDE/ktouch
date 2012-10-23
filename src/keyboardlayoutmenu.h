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

#ifndef KEYBOARDLAYOUTMENU_H
#define KEYBOARDLAYOUTMENU_H

#include <QMenu>

class DataIndex;

class KeyboardLayoutMenu : public QMenu
{
    Q_OBJECT
public:
    explicit KeyboardLayoutMenu(QWidget *parent = 0);
    DataIndex* dataIndex() const;
    void setDataIndex(DataIndex* dataIndex);
    QString keyboardLayoutName() const;
    void setKeyboardLayoutName(const QString& keyboardLayoutName);
signals:
    void keyboardLayoutNameChanged();
private slots:
    void populate();
    void onActionTriggerd(QAction* action);
private:
    DataIndex* m_dataIndex;
    QString m_keyboardLayoutName;
};

#endif // KEYBOARDLAYOUTMENU_H
