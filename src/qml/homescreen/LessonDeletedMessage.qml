/*
 *  SPDX-FileCopyrightText: 2017 Sebastian Gottfried <sebastian.gottfried@posteo.de>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.9
import QtQuick.Layouts 1.3
import ktouch 1.0

import "../common"

Collapsable {
    id: root

    collapsed: true

    property Lesson deletedLesson: Lesson {}
    signal undeleteRequested()
    property bool completed: false

    onVisibleChanged: {
        if (!visible && completed) {
            destroy();
        }
    }

    Component.onCompleted: {
        root.collapsed = false;
        root.completed = true;
    }

    Item {
        implicitWidth: root.width
        implicitHeight: layout.implicitHeight + 2 * Units.largeSpacing

        RowLayout {
            id: layout
            width: root.width - 2 * Units.gridUnit
            anchors.centerIn: parent

            Label {
                text: root.deletedLesson.title?
                          i18n("Lesson <b>%1</b> deleted.", root.deletedLesson.title):
                          i18n("Lesson without title deleted.")
                wrapMode: Text.Wrap
                Layout.fillWidth: true
            }

            IconButton {
                iconName: "edit-undo"
                text: i18n("Undo")
                onClicked: {
                    root.undeleteRequested();
                    root.collapsed = true;
                }
            }

            AutoTriggerButton {
                id: okButton
                iconName: "dialog-ok"
                text: i18n("Confirm")
                onClicked: {
                    root.collapsed = true;
                }
            }
        }
    }
}
