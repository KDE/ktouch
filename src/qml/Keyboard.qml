import QtQuick 1.0
import ktouch 1.0

Rectangle
{
    property real scaleFactor: height / 600

    color: "#4f4f4f"

    function handleKeyPress(event)
    {
        for (var i = 0; i < keys.count; i++)
        {
            var key = keys.itemAt(i);
            if (key.match(event))
                key.state = "pressed"
        }
    }

    function handleKeyRelease(event)
    {
        for (var i = 0; i < keys.count; i++)
        {
            var key = keys.itemAt(i);
            if (key.match(event))
                key.state = "normal"
        }
    }

    Item {
        id: keyContainer

        width: childrenRect.width
        height: childrenRect.height
        anchors.centerIn: parent

        Repeater {
            id: keys
            model: keyboardModel.keyCount()
            Key {
                keyModel: keyboardModel.key(index)
                referenceKeyModel: keyboardModel.referenceKey
            }
        }
    }
}
