import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

Item {
    id: item

    property bool trainingStarted: false
    property bool trainingFinished: true
    property Item menuOverlayItem

    function setMessage() {
        if (!stats.timeIsRunning && !trainingFinished) {
            messageBox.showMessage(
                        trainingStarted?
                            i18n("Training session started. Time begins running with the first key stroke."):
                            i18n("Training session interrupted. Time begins running again with next key stroke."))
        }
        else {
            messageBox.clearMessage()
        }
    }

    function reset() {
        messageBox.clearMessageImmediately()
    }

    onTrainingStartedChanged: setMessage()
    onTrainingFinishedChanged: setMessage()

    height: 29
    width: 400

    PlasmaCore.FrameSvgItem {
        anchors.fill: parent
        imagePath: findImage("trainingscreen.svgz")
        prefix: "toolbar"
    }

    Row {
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: parent.right
            leftMargin: 3
            rightMargin: 3
        }

        spacing: 3
        height: pauseButton.height

        PlasmaComponents.ToolButton {
            id: menuButton
            iconSource: "go-home"
            onClicked: item.menuOverlayItem.show()
        }

        PlasmaComponents.ToolButton {
            id: pauseButton
            iconSource: "media-playback-pause"
            checked: !stats.timeIsRunning
            onClicked: stats.stopTraining()
            onCheckedChanged: setMessage()
        }

        MessageBox {
            id: messageBox
        }
    }
}
