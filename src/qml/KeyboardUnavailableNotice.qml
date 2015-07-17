/*
 *  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *  Copyright 2015  Sebastian Gottfried <sebastiangottfried@web.de>
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

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import org.kde.kquickcontrolsaddons 2.0
import ktouch 1.0

Item {
    id: root

    height: row.height + 20

    Row {
        id: row
        anchors.centerIn: parent
        width: parent.width - 60
        height: Math.max(icon.height, label.height)
        spacing: 3

        QIconItem {
            id: icon
            anchors.verticalCenter: parent.verticalCenter
            width: 22
            height: 22
            icon: "dialog-warning"
        }

        Label {
            id: label
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width - icon.width - parent.spacing
            text: i18n("No visualization available for your keyboard layout.")
            color: "white"
        }
    }
}
