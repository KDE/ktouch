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
        if (!lesson)
            return;
        updateAnimation.restart()
    }


    Rectangle {
        id: sheet
        anchors.centerIn: parent

        opacity: 0
        width: lessonPainter.width
        height: lessonPainter.height

        border {
            width: 1
            color: "#000"
        }

        LessonPainter {
            id: lessonPainter
            maximumWidth: item.width
            maximumHeight: item.height
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
        ScriptAction {
            script: lessonPainter.lesson = lesson
        }
        NumberAnimation {
            target: sheet
            property: "opacity"
            to: 1
            duration: 100
        }
    }
}
