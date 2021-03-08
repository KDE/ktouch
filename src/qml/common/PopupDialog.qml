/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import ktouch 1.0
import QtGraphicalEffects 1.0

Dialog {
    id: root
    dim: true

    opacity: 0
    scale: 0.9
    leftMargin: Math.floor((parent.width - width) / 2)
    topMargin: Math.floor((parent.height - height) / 2)

    Component.onCompleted: {
        var candidate = root
        while (candidate.parent) {
            candidate = candidate.parent
        }
        if (candidate) {
            root.parent = candidate
        }
    }

    onOpened: {
        contentItem.forceActiveFocus()
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
        NumberAnimation { property: "scale"; to: 1.0; easing.type: Easing.OutQuint; duration: Units.longDuration }
        NumberAnimation { property: "opacity"; to: 1.0; easing.type: Easing.OutCubic; duration: Units.shortDuration }
        NumberAnimation { target: dimOverlay;  property: "opacity"; to: 1.0; easing.type: Easing.OutCubic; duration: Units.longDuration }
    }

    exit: Transition {
        // shrink_fade_out
        NumberAnimation { property: "scale"; to: 0.9; easing.type: Easing.OutQuint; duration: Units.longDuration }
        NumberAnimation { property: "opacity"; to: 0.0; easing.type: Easing.OutCubic; duration: Units.shortDuration }
        NumberAnimation { target: dimOverlay;  property: "opacity"; to: 0.0; easing.type: Easing.OutCubic; duration: Units.longDuration }
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
        implicitHeight: Math.max(titleLabel.implicitHeight, closeButton.implicitHeight)
        color: toolbarColorScheme.toolbarBackground

        KColorScheme {
            id: toolbarColorScheme
            colorGroup: KColorScheme.Active
            colorSet: KColorScheme.Complementary
            property color toolbarBackground: Qt.darker(toolbarColorScheme.shade(toolbarColorScheme.hoverDecoration, KColorScheme.MidShade, toolbarColorScheme.contrast, -0.2), 1.3)
        }

        RowLayout {
            anchors.fill: parent

            Label {
                id: titleLabel
                text: root.title
                width: parent.width
                color: toolbarColorScheme.normalText
                font.bold: true
                padding: font.pixelSize
                Layout.fillWidth: true
            }

            IconToolButton {
                id: closeButton
                iconName: "window-close-symbolic"
                color: toolbarColorScheme.normalText
                visible: root.closePolicy & Popup.CloseOnEscape
                backgroundColor: toolbarColorScheme.normalBackground
                activeFocusOnTab: false
                Layout.preferredWidth: titleLabel.implicitHeight
                Layout.preferredHeight: titleLabel.implicitHeight
                onClicked: root.close()
            }
        }
    }
}
