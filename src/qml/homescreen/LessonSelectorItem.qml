/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Layouts 1.3
import ktouch 1.0
import '../common'

Item {
    id: root
    property Lesson lesson
    property bool selected
    property bool editable: false
    property alias background: background

    signal clicked
    signal doubleClicked
    signal deleteButtonClicked
    signal editButtonClicked
    signal statButtonClicked

    clip: true

    KColorScheme {
        id: selectionColorScheme
        colorGroup: KColorScheme.Active
        colorSet: KColorScheme.Selection
    }

    Rectangle {
        id: background
        anchors.fill: parent
        color: selected? Qt.tint(selectionColorScheme.normalBackground, "#a0ffffff"): "#ffffff"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.clicked()
        }
        onDoubleClicked: {
            root.doubleClicked()
        }
    }

    GridLayout {
        id: content
        anchors.fill: parent
        anchors.margins: Units.largeSpacing

        Label {
            id: titleLabel
            Layout.column: 0
            Layout.row: 0
            Layout.fillWidth: true
            Layout.preferredHeight: buttonRow.implicitHeight
            text: lesson? lesson.title: ""
            color: "#000000"
            font.bold: true
            elide: Label.ElideRight
            verticalAlignment: Qt.AlignVCenter

            ToolTip {
                parent: titleLabel
                text: titleLabel.text
                visible: titleMouseArea.containsMouse
            }

            MouseArea {
                id: titleMouseArea
                anchors.fill: parent
                acceptedButtons: Qt.NoButton
                hoverEnabled: titleLabel.truncated
            }
        }

        Row {
            id: buttonRow
            Layout.column: 1
            Layout.row: 0
            visible: root.selected

            IconToolButton {
                id: editButton
                visible: root.editable
                iconName: 'edit-entry'
                color: "#000000"
                backgroundColor: "#c0c0c0c0"
                onClicked: {
                    root.editButtonClicked();
                }
            }

            IconToolButton {
                id: deleteButton
                visible: root.editable
                iconName: 'edit-delete'
                color: "#000000"
                backgroundColor: "#c0c0c0c0"
                onClicked: {
                    root.deleteButtonClicked();
                }
            }

            IconToolButton {
                iconName: 'view-statistics'
                color: "#000000"
                backgroundColor: "#c0c0c0c0"
                onClicked: {
                    root.statButtonClicked();
                }
            }
        }

        Item {
            Layout.column: 0
            Layout.row: 1
            Layout.columnSpan: 2
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true

            Label {
                id: textLabel
                anchors.fill: parent
                text: lesson? lesson.text: ""
                color: "#000000"
                font.family: 'monospace'
                lineHeight: 1.5
                scale: Math.min(1, width / implicitWidth)
                transformOrigin: Item.TopLeft
            }

            Rectangle {
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#00000000" }
                    GradientStop { position: 0.8; color: Qt.rgba(background.color.r, background.color.g, background.color.b, 0) }
                    GradientStop { position: 1.0; color: background.color }
                }
            }
        }
    }
}

