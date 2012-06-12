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

Item {
    id: item

    property Lesson lesson
    property int margin: 30
    property string lessonText
    property string lessonTitle

    onLessonChanged: {
        if (!lesson)
            return;
        updateAnimation.stop()
        item.lessonText = item.lesson.text
        item.lessonTitle = item.lesson.title
        updateAnimation.start()
    }


    Rectangle {
        id: sheet
        anchors.centerIn: parent
        border {
            width: 1
            color: "#000"
        }
        opacity: 0
        width: Math.round(content.width * wrapper.scale) + 30
        height: Math.round(content.height * wrapper.scale) + 30

        Item {
            id: wrapper
            x: 15
            y: 15
            scale: Math.min(1, Math.min((item.width - 2 * margin) / content.width, (item.height - 2 * margin) / content.height))

            Column {
                id: content
                property real aspectRatio: width / height

                Text {
                    id: caption
                    smooth: true
                    font.pixelSize: text.font.pixelSize * 2
                    lineHeight: 1.5
                }

                Text {
                    id: text
                    smooth: true
                    font.family: "mono"
                    font.pixelSize: theme.smallestFont.pixelSize
                    lineHeight: 1.5
                }
            }
        }
    }

    SequentialAnimation {
        id: updateAnimation
        NumberAnimation {
            target: sheet
            property: "opacity"
            to: 0
            duration: 100
        }
        PropertyAction {
            target: caption
            property: "text"
            value: item.lessonTitle
        }
        PropertyAction {
            target: text
            property: "text"
            value: item.lessonText
        }
        NumberAnimation {
            target: sheet
            property: "opacity"
            to: 1
            duration: 100
        }
    }
}
