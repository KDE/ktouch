/*
 *  Copyright 2018  Sebastian Gottfried <sebastian.gottfried@posteo.de>
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

import QtQuick 2.9
import ktouch 1.0

import '../common'

Item {
    id: root

    property KColorScheme colorScheme: KColorScheme {}

    height: label.height + 20

    IconLabel {
        id: label
        anchors.centerIn: parent
        width: parent.width - 60
        iconName: "dialog-warning"
        text: i18n("No visualization available for your keyboard layout.")
        color: colorScheme.normalText
    }
}
