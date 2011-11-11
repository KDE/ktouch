import QtQuick 1.0
import ktouch 1.0

Rectangle
{
    property real scaleFactor: 1;

    color: "#4f4f4f"

    Item {
        id: keyContainer

        width: childrenRect.width
        height: childrenRect.height
        anchors.centerIn: parent

        Repeater {
            model: keyboardModel.keyCount()
            Key {
                keyModel: keyboardModel.key(index)
                referenceKeyModel: keyboardModel.referenceKey
            }
        }
    }
}
