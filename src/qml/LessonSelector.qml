import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import ktouch 1.0

Item {
    id: item
    property variant course
    signal lessonSelected(variant lesson)

    Row {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 20

        Item {
            id: listContainer
            height: parent.height
            width: Math.round((parent.width - parent.spacing) / 2)

            ListView {
                id: list
                anchors .fill: parent
                model: course.lessonCount
                clip: true
                delegate: ListItem {
                    width: list.width - scrollBar.width
                    onSelected: list.currentIndex = index
                    title: item.course.lesson(index).title
                }
            }

            PlasmaComponents.ScrollBar {
                id: scrollBar
                flickableItem: list
            }
        }

        Column {
            width: parent.width - listContainer.width - parent.spacing
            height: parent.height

            LessonPreview {
                width: parent.width
                height: parent.height - startButtonContainer.height
                lesson: list.currentIndex !== -1? item.course.lesson(list.currentIndex): null
            }

            Item {
                id: startButtonContainer
                width: parent.width
                height: Math.round(1.5 * startButton.height)

                PlasmaComponents.Button {
                    id: startButton
                    anchors {
                        horizontalCenter: startButtonContainer.horizontalCenter
                        bottom: parent.bottom
                    }
                    text: i18n("Start training")
                    enabled: list.currentItem !== null
                    iconSource: "go-next-view"
                    onClicked: lessonSelected(item.course.lesson(list.currentIndex))
                }
            }
        }
    }
}
