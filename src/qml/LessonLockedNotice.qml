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
import QtGraphicalEffects 1.0
import org.kde.kquickcontrolsaddons 2.0
import ktouch 1.0


Item {
    id: root

    width: content.width + 40
    height: content.height + 40

    property alias blurSource: effectSource.sourceItem

    SystemPalette {
        id: palette
        colorGroup: SystemPalette.Active
    }

    ShaderEffectSource {
        id: effectSource
        anchors.fill: parent
        hideSource: false
        sourceRect: Qt.rect(root.x, root.y, root.width, root.height)
    }

    FastBlur {
        anchors.fill: parent
        source: effectSource
        radius: 25
    }

    Rectangle {
        id: background
        anchors.fill: parent
        color: palette.base
        opacity: 0.3
        radius: 15
    }

    Behavior on opacity {
        NumberAnimation {
            duration: 200
        }
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

        Label {
            id: text
            text: i18n("Complete Previous Lessons to Unlock")
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            font.weight: Font.Bold
            wrapMode: Text.Wrap
        }
    }
}
