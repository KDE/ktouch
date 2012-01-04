import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import ktouch 1.0

Column {
    id: item

    property variant courses;
    signal lessonSelected(variant lesson)

    property variant boxes: [];
    property int lastActiveIndex: -1
    property int activeIndex: 0

    function cellHeight(index)
    {
        if (index == -1)
            return 0
        var box = item.boxes[index]
        if (index == item.activeIndex) {
            var height = item.height;
            for (var i = 0; i < item.boxes.length; i++)
                if (i != index)
                height -= item.boxes[i].collapsedHeight
            return height
        }
        else {
            return box.collapsedHeight
        }
    }

    function activateBox(index) {
        item.lastActiveIndex = item.activeIndex
        item.activeIndex = index
        boxSwitchAnimation.start()
    }

    function updateBoxes() {
        var boxes = []
        for (var i = 0; i < repeater.count; i++) {
            boxes.push(repeater.itemAt(i))
        }
        boxes.push(ownLessonsBox);
        item.boxes = boxes;
    }

    function setBoxHeights() {
        for (var i = 0; i < item.boxes.length; i++) {
            item.boxes[i].height = cellHeight(i)
        }
    }

    onHeightChanged: setBoxHeights()
    onCoursesChanged: updateBoxes()

    Repeater {
        id: repeater
        model: courses.length

        HomeScreenBox {
            id: box
            width: parent.width
            title: i18n(courses[index].title)
            active: activeIndex == index
            onActivated: activateBox(index)
            content: LessonSelector {
                course: courses[index]
                onLessonSelected: item.lessonSelected(lesson)
            }
        }
    }

    HomeScreenBox {
        id: ownLessonsBox
        width: parent.width
        title: i18n("Own lessons")
        active: activeIndex == item.boxes.length - 1
        onActivated: activateBox(item.boxes.length - 1)
        content: Item {
            Text {
                anchors.centerIn: parent
                font.pixelSize: 50
                color: "#888"
                text: "TODO"
            }
        }
    }

    ParallelAnimation {
        id: boxSwitchAnimation
        NumberAnimation {
            target: item.lastActiveIndex != -1? item.boxes[item.lastActiveIndex]: null
            property: "height"
            to: cellHeight(item.lastActiveIndex)
            duration: 250
        }
        NumberAnimation {
            target: item.boxes[item.activeIndex]
            property: "height"
            to: cellHeight(item.activeIndex)
            duration: 250
        }
    }

    Component.onCompleted: updateBoxes()
}
