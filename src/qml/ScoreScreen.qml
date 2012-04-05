/*
 *  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
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

import QtQuick 1.1
import ktouch 1.0
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

Item {
    id: screen

    function start() {}
    function reset() {
        internal.nextLesson = null
        for (var i = 0; i < course.lessonCount - 1; i++) {
            if (lesson === course.lesson(i)) {
                internal.nextLesson = course.lesson(i + 1)
                break
            }
        }
    }

    property Lesson lesson
    property Course course

    signal homeScreenRequested
    signal nextLessonRequested(variant lesson)
    signal lessonRepetionRequested

    QtObject {
        id: internal
        property Lesson nextLesson: null
    }

    Column {
        anchors.fill: parent
        spacing: 5
        anchors.margins: 5

        PlasmaCore.FrameSvgItem  {
            id: header
            width: parent.width
            height: headerLabel.height + margins.top + margins.bottom
            imagePath: "widgets/frame"
            prefix: "raised"


            PlasmaComponents.Label {
                id: headerLabel
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    right: parent.right
                    leftMargin: header.margins.left
                    rightMargin: header.margins.right
                }
                text: i18n("Lesson finished")
                height: paintedHeight
                font.pointSize: 1.5 * theme.defaultFont.pointSize
            }
        }

        PlasmaComponents.TabBar {
            id: tabBar
            width: parent.width
            PlasmaComponents.TabButton {
                tab: summaryTab
                text: i18n("Summary")
            }
            PlasmaComponents.TabButton {
                tab: speedTab
                text: i18n("Speed")
            }
            PlasmaComponents.TabButton {
                tab: accuracyTab
                text: i18n("Accuracy")
            }
        }

        PlasmaComponents.TabGroup {
            id: tabGroup
            width: parent.width
            height: parent.height - ((parent.children.length - 1) * parent.spacing) - header.height - tabBar.height - footer.height

            PlasmaComponents.Page {
                id: summaryTab
                Item {
                    anchors.fill: parent
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 50
                        color: "#888"
                        text: "TODO"
                    }
                }
            }
            PlasmaComponents.Page {
                id: speedTab
                Item {
                    anchors.fill: parent
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 50
                        color: "#888"
                        text: "TODO"
                    }
                }
            }
            PlasmaComponents.Page {
                id: accuracyTab
                Item {
                    anchors.fill: parent
                    Text {
                        anchors.centerIn: parent
                        font.pixelSize: 50
                        color: "#888"
                        text: "TODO"
                    }
                }
            }
        }

        Row {
            id: footer
            width: parent.width
            height: childrenRect.height
            spacing: 5

            PlasmaComponents.Button {
                id: homeScreenButton
                text: i18n("Return to home screen")
                iconSource: "go-home"
                onClicked: screen.homeScreenRequested()
            }
            Item {
                width: parent.width - ((parent.children.length - 1) * parent.spacing) - nextLessonButton.width - repeatLessonButton.width - homeScreenButton.width
                height: homeScreenButton.height
            }
            PlasmaComponents.Button {
                id: repeatLessonButton
                iconSource: "view-refresh"
                text: i18n("Repeat lesson")
                onClicked: screen.lessonRepetionRequested()
            }
            PlasmaComponents.Button {
                id: nextLessonButton
                iconSource: "go-next-view"
                text: i18n("Next lesson")
                enabled: !!internal.nextLesson
                onClicked: screen.nextLessonRequested(internal.nextLesson)
            }
        }
    }
}
