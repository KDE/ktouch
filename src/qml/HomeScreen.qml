import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import ktouch 1.0

Item {
    id: screen

    property variant courses;
    signal lessonSelected(variant lesson)

    anchors.fill: parent

    Column {
        anchors.fill: parent
        PlasmaComponents.ToolBar {
            id: header
            width: parent.width
            tools: PlasmaComponents.ToolBarLayout {
                spacing: 5
                PlasmaComponents.ToolButton {
                    text: "Your Name"
                }
                PlasmaComponents.ToolButton {
                    iconSource: "configure"
                }
            }
        }

        Item {
            id: content
            width: parent.width
            height: parent.height - header.height

            HomeScreenAccordion {
                courses: screen.courses
                anchors.fill: parent
                anchors.margins: 5
                onLessonSelected: screen.lessonSelected(lesson)
            }
        }
    }
}
