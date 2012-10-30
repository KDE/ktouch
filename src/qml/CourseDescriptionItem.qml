import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import ktouch 1.0

Rectangle {
    id: root

    property string description
    property bool active: false

    height: active || content.opacity > 0? childrenRect.height: 0
    visible: height > 0
    color: "white"

    Behavior on height {
        NumberAnimation {
            duration: 150
            easing.type: Easing.InOutQuad
        }
    }

    onDescriptionChanged: {
        if (content.opacity === 0) {
            descriptionLabel.text = description
        }
        else {
            content.needsUpdate = true
        }
    }

    Item {
        id: content

        property bool needsUpdate: false

        width: parent.width
        height: descriptionLabel.height + 6
        opacity: root.active && !content.needsUpdate && root.height === root.childrenRect.height? 1: 0

        Behavior on opacity {
            NumberAnimation {
                duration: 150
            }
        }

        onOpacityChanged: {
            if (needsUpdate && opacity === 0) {
                descriptionLabel.text = root.description
                needsUpdate = false
            }
        }

        PlasmaComponents.Label {
            id: descriptionLabel
            anchors.centerIn: parent
            width: parent.width - 10
            font.pointSize: theme.smallestFont.pointSize
        }
    }

}
