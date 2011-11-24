import QtQuick 1.0
import ktouch 1.0 as KTouch

Rectangle
{
    property real scaleFactor: Math.min(width / (keyboardLayout.width + 50), height / (keyboardLayout.height + 50))

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
            model: keyboardLayout.keyCount
            Key {
                key: keyboardLayout.key(index)
                referenceKey: keyboardLayout.referenceKey
            }
        }
    }
}
