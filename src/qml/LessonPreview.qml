/*
 *  Copyright 2013  Sebastian Gottfried <sebastiangottfried@web.de>
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

    onLessonChanged: {
        if (!lesson) {
            sheet.lessonPainter.lesson = null
            return;
        }

        swapPreviewAnimation.stop()
        swapPreviewAnimation.from = sheet.lessonPainter == lessonPainter1? lessonPainter1: lessonPainter2
        swapPreviewAnimation.to = sheet.lessonPainter == lessonPainter1? lessonPainter2: lessonPainter1
        swapPreviewAnimation.start()
    }

    Rectangle {
        id: sheet

        property LessonPainter lessonPainter: lessonPainter1

        anchors.centerIn: parent
        width: lessonPainter.width
        height: lessonPainter.height

        Behavior on width {
            NumberAnimation {
                duration: 100
            }
        }

        Behavior on height {
            NumberAnimation {
                duration: 100
            }
        }

        border {
            width: 1
            color: "#000"
        }

        LessonPainter {
            id: lessonPainter1
            opacity: 1
            maximumWidth: item.width
            maximumHeight: item.height
        }

        LessonPainter {
            id: lessonPainter2
            maximumWidth: item.width
            maximumHeight: item.height
            opacity: 0
        }
    }

    SequentialAnimation {
        id: swapPreviewAnimation
        property LessonPainter from: lessonPainter1
        property LessonPainter to: lessonPainter2
        NumberAnimation {
            target: swapPreviewAnimation.from
            property: "opacity"
            to: 0
            duration: 100
        }
        ScriptAction {
            script: {
                swapPreviewAnimation.to.lesson = lesson
                sheet.lessonPainter = swapPreviewAnimation.to
            }
        }
        PauseAnimation { duration: 100 }
        NumberAnimation {
            target: swapPreviewAnimation.to
            property: "opacity"
            to: 1
            duration: 100
        }
    }
}
