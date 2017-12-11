import QtQuick 2.0
/*
 *  Copyright 2017  Sebastian Gottfried <sebastian.gottfried@posteo.de>
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
import QtQuick.Controls 2.2 as Controls
import ktouch 1.0

Controls.ScrollBar {
    id: control

    property alias colorScheme: colorScheme

    KColorScheme {
        id: colorScheme
        colorGroup: control.enabled? KColorScheme.Active: KColorScheme.Disabled
        colorSet: KColorScheme.Window
    }

    contentItem: Rectangle {
        implicitWidth: 6
        implicitHeight: 100
        radius: width / 2
        color: control.pressed ? colorScheme.focusDecoration: colorScheme.alternateBackground
        opacity: control.active? 1: 0.3
        Behavior on opacity {
            NumberAnimation {
                duration: 150
            }
        }
        Behavior on color {
            ColorAnimation {
                duration: 150
            }
        }
   }
}
