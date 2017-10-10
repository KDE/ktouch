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
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import org.kde.kquickcontrolsaddons 2.0
import ktouch 1.0


Item {
    id: root

    width: content.width
    height: content.height

    property color glowColor: "#ffffff"

    KColorScheme {
        id: palette
        colorGroup: KColorScheme.Active
        colorSet: KColorScheme.Window
    }

    Column {
        id: content

        anchors.centerIn: parent
        width: Math.max(icon.width, text.width)
        spacing: 10

        QIconItem {
            id: icon
            anchors.horizontalCenter: parent.horizontalCenter
            icon: "object-locked"
            width: 128
            height: 128
        }

        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            width: text.width + 2 * text.font.pixelSize
            height: text.height + text.font.pixelSize
            radius: text.font.pixelSize
            color: palette.neutralBackground

            Label {
                anchors.centerIn: parent
                id: text
                text: i18n("Complete Previous Lessons to Unlock")
                horizontalAlignment: Text.AlignHCenter
                font.weight: Font.Bold
                wrapMode: Text.Wrap
            }
        }
    }

    Glow {
        anchors.fill: content
        source: content
        color: root.glowColor
        samples: 25
    }
}
