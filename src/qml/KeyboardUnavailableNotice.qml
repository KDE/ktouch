import QtQuick 1.1
import org.kde.qtextracomponents 0.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import ktouch 1.0

Item {
    id: root

    height: row.height + 20

    Row {
        id: row
        anchors.centerIn: parent
        width: parent.width - 20
        height: Math.max(icon.height, label.height)
        spacing: 3

        QIconItem {
            id: icon
            anchors.verticalCenter: parent.verticalCenter
            width: theme.smallIconSize
            height: theme.smallIconSize
            icon: QIcon("dialog-warning")
        }

        PlasmaComponents.Label {
            id: label
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width - icon.width - parent.spacing
            height: paintedHeight
            text: i18n("No visualization available for your keyboard layout.")
            color: "white"
        }
    }
}
