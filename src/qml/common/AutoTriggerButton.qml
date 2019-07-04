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
import QtQuick.Controls 2.2
import ktouch 1.0

IconButton {
    id: control

    property alias duration: progressAnimation.duration
    property alias running: progressAnimation.running

    background.children: [
        ProgressBar {
            id: progressBar
            anchors.fill: parent
            value: 0.0
            background: Item {}
            contentItem: Item {
                Rectangle {
                    color: control.colorScheme.focusDecoration
                    opacity: 0.4
                    width: parent.width * progressBar.visualPosition
                    height: parent.height
                }
            }

            NumberAnimation {
                id: progressAnimation
                target: progressBar
                property: "value"
                duration: 10000
                to: 1.0
                running: true
                onStopped: {
                    control.clicked();
                }
            }
        }
    ]
}
