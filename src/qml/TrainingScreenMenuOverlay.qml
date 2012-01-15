import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

Item {
    id: item

    signal restartRequested()
    signal abortRequested()

    visible: false
    opacity: 0

    function show() {
        focusScope.forceActiveFocus()
        fadeInAnimation.start()
    }

    function hide() {
        fadeOutAnimation.start()
    }

    Rectangle {
        anchors.fill: parent
        color: "#55000000"
    }

    /* swallow all mouse events */
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
    }

    FocusScope {
        id: focusScope
        anchors.centerIn: parent

        Column {
            anchors.centerIn: parent
            spacing: 15
            width: Math.max(resumeButton.implicitWidth, restartButton.implicitWidth, returnButton.implicitWidth)

            PlasmaComponents.Button {
                focus: true
                id: resumeButton
                iconSource: "go-next-view"
                text: i18n("Resume training")
                width: parent.width
                onClicked: hide()
            }

            PlasmaComponents.Button {
                id: restartButton
                iconSource: "view-refresh"
                text: i18n("Restart lesson")
                width: parent.width
                onClicked: {
                    restartRequested()
                    hide()
                }
            }

            PlasmaComponents.Button {
                id: returnButton
                iconSource: "go-home"
                text: i18n("Return to home screen")
                width: parent.width
                onClicked: {
                    abortRequested()
                    hide()
                }
            }
        }
    }

    SequentialAnimation {
        id: fadeInAnimation
        PropertyAction {
            target: item
            property: "visible"
            value: true
        }
        NumberAnimation {
            target: item
            property: "opacity"
            to: 1
            duration: 300
            easing.type: Easing.InOutQuad
        }
    }

    SequentialAnimation {
        id: fadeOutAnimation
        NumberAnimation {
            target: item
            property: "opacity"
            to: 0
            duration: 300
            easing.type: Easing.InOutQuad
        }
        PropertyAction {
            target: item
            property: "visible"
            value: false
        }
    }
}
