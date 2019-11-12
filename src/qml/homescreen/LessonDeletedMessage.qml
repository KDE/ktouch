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
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
        implicitHeight: layout.implicitHeight + 20

        RowLayout {
            id: layout
            width: root.width - 40
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
