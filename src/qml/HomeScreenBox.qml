import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import ktouch 1.0

Item {
    id: item

    property alias title: titleLabel.text
    property Item content
    property int collapsedHeight: header.height + frame.margins.top + frame.margins.bottom
    signal activated

    clip: true

    onHeightChanged: {
        var isCollapsed = item.height == item.collapsedHeight;
        line.visible = !isCollapsed
        content.visible = !isCollapsed
    }

    PlasmaCore.FrameSvgItem  {
        id: frame
        anchors.fill: parent
        imagePath: "widgets/frame"
        prefix: "raised"

        Column {
            id: layout
            anchors {
                fill: parent
                topMargin: frame.margins.top
                rightMargin: frame.margins.right
                bottomMargin: frame.margins.bottom
                leftMargin: frame.margins.left
            }

            spacing: 0
            Item {
                id: header
                height: 20
                width: parent.width
                PlasmaComponents.Label {
                    id: titleLabel
                    anchors.centerIn: parent
                    height: paintedHeight
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: item.activated()
                }
            }

            PlasmaCore.SvgItem {
                id: line
                width: parent.width
                height: naturalSize.height
                elementId: "horizontal-line"
                svg: PlasmaCore.Svg {
                    imagePath: "widgets/line"
                }
            }

            Item {
                id: content
                width: parent.width
                height: parent.height - header.height - line.height

                Component.onCompleted: {
                    item.content.parent = content
                    item.content.anchors.fill = content
                }
            }
        }
    }
}
