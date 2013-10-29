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
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import ktouch 1.0

Item {
    id: root

    property Profile profile
    property DataIndexCourse dataIndexCourse
    property KeyboardLayout keyboardLayout
    property string keyboardLayoutName
    signal lessonSelected(variant course, variant lesson)

    property Course course: lessonSelector.visible? lessonSelector.course: customLessonSelector.course

    function getCourse() {
        return !!dataIndexCourse? lessonSelector.course: customLessonSelector.course
    }

    width: parent.width
    height: parent.height
    visible: false
    opacity: 0

    function showImmediately() {
        root.x = 0
        root.opacity = 1
        root.visible = 1
    }

    function show(direction) {
        appearAnimation.stop()
        disappearAnimation.stop()
        appearAnimation.direction = direction
        appearAnimation.start()
    }

    function hide(direction) {
        appearAnimation.stop()
        disappearAnimation.stop()
        disappearAnimation.direction = direction
        disappearAnimation.start()
    }


    QtObject {
        id: priv
        property int duration: 250
    }

    LessonSelector {
        id: lessonSelector

        anchors.fill: parent
        visible: !!dataIndexCourse

        profile: root.profile
        dataIndexCourse: root.dataIndexCourse
        onLessonSelected: root.lessonSelected(course, lesson)
    }

    CustomLessonSelector {
        id: customLessonSelector
        anchors.fill: parent
        visible: !root.dataIndexCourse
        profile: root.profile
        keyboardLayout: root.keyboardLayout
        keyboardLayoutName: root.keyboardLayoutName
        onLessonSelected: root.lessonSelected(course, lesson)
    }

    SequentialAnimation {
        id: appearAnimation
        property int direction: Item.Left
        PropertyAction {
            target: root
            property: "visible"
            value: true
        }
        ParallelAnimation {
            NumberAnimation {
                target: root
                property: "x"
                from: appearAnimation.direction === Item.Left? root.width: -root.width
                to: 0
                easing.type: Easing.InQuad
                duration: priv.duration
            }
            NumberAnimation {
                target: root
                property: "opacity"
                from: 0
                to: 1
                easing.type: Easing.InQuad
                duration: priv.duration
            }
        }
    }

    SequentialAnimation {
        id: disappearAnimation
        property int direction: Item.Left
        ParallelAnimation {
            NumberAnimation {
                target: root
                property: "x"
                to: disappearAnimation.direction === Item.Left? -root.width: root.width
                easing.type: Easing.InQuad
                duration: priv.duration
            }
            NumberAnimation {
                target: root
                property: "opacity"
                to: 0
                easing.type: Easing.InQuad
                duration: priv.duration
            }
        }
        PropertyAction {
            target: root
            property: "visible"
            value: false
        }
    }
}
