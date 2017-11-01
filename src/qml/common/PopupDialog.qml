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

import QtQuick 2.4
import QtQuick.Controls 2.2
import ktouch 1.0
import QtGraphicalEffects 1.0

Dialog {
    id: root
    dim: true

    margins: {
        left: 40
        bottom: 40
        right: 40
        top: 40
    }

    width: parent.width - leftMargin - rightMargin
    height: parent.height - topMargin - bottomMargin
    opacity: 0
    scale: 0.9

    Component.onCompleted: {
        var candidate = root
        while (candidate.parent) {
            candidate = candidate.parent
        }
        if (candidate) {
            root.parent = candidate
        }
    }

    Item {
        id: dimOverlay
        parent: root.parent
        width: root.parent.width
        height: root.parent.height
        visible: root.visible && root.dim
        opacity: 0

        ShaderEffectSource {
            id: effectSource
            sourceItem: root.parent.appContent
            anchors.fill: parent
            hideSource: false
        }

        HueSaturation {
            id: desaturatedBackground
            source: effectSource
            anchors.fill: parent
            lightness: -0.3
            saturation: -0.5
            visible: false
        }

        FastBlur {
            anchors.fill: parent
            source: desaturatedBackground
            radius: 50
        }

        Rectangle {
            anchors.fill: parent
            color: "#55000000"
        }
    }

    enter: Transition {
        // grow_fade_in
        NumberAnimation { property: "scale"; to: 1.0; easing.type: Easing.OutQuint; duration: 220 }
        NumberAnimation { property: "opacity"; to: 1.0; easing.type: Easing.OutCubic; duration: 150 }
        NumberAnimation { target: dimOverlay;  property: "opacity"; to: 1.0; easing.type: Easing.OutCubic; duration: 220 }
    }

    exit: Transition {
        // shrink_fade_out
        NumberAnimation { property: "scale"; to: 0.9; easing.type: Easing.OutQuint; duration: 220 }
        NumberAnimation { property: "opacity"; to: 0.0; easing.type: Easing.OutCubic; duration: 150 }
        NumberAnimation { target: dimOverlay;  property: "opacity"; to: 0.0; easing.type: Easing.OutCubic; duration: 220 }
    }

    background: Rectangle {
        color: dialogColorScheme.normalBackground

        KColorScheme {
            id: dialogColorScheme
            colorGroup: KColorScheme.Active
            colorSet: KColorScheme.Window
        }
    }

    header: Rectangle {
        height: titleLabel.implicitHeight
        color: toolbarColorScheme.toolbarBackground

        KColorScheme {
            id: toolbarColorScheme
            colorGroup: KColorScheme.Active
            colorSet: KColorScheme.Complementary
            property color toolbarBackground: Qt.darker(toolbarColorScheme.shade(toolbarColorScheme.hoverDecoration, KColorScheme.MidShade, toolbarColorScheme.contrast, -0.2), 1.3)
        }

        Label {
            id: titleLabel
            text: root.title
            width: parent.width
            color: toolbarColorScheme.normalText
            font.bold: true
            padding: font.pixelSize
        }

    }

}
