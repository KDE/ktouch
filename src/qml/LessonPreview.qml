import QtQuick 1.1
import ktouch 1.0

Item {
    id: item

    property Lesson lesson
    property int margin: 30
    property string lessonText
    property string lessonTitle

    onLessonChanged: {
        var lines = []
        for (var i = 0; i < item.lesson.lineCount; i++)
            lines.push(item.lesson.line(i).value)
        item.lessonText = lines.join("\n")
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
