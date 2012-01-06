import QtQuick 1.1
import ktouch 1.0

Item
{
    id: keyboard

    property KeyboardLayout keyboardLayout
    property int margin: 50
    property real aspectRatio: (keyboardLayout.width + margin) / (keyboardLayout.height + margin)
    property real scaleFactor: Math.min(width / (keyboardLayout.width + margin), height / (keyboardLayout.height + margin))

    function findKey(keyChar)
    {
        for (var i = 0; i < keys.count; i++)
        {
            var key = keys.itemAt(i);
            if (key.match(keyChar))
                return key
        }
        return null

    }

    function handleKeyPress(event)
    {
        var key = findKey(event)
        if (key)
            key.state = "pressed"
    }

    function handleKeyRelease(event)
    {
        var key = findKey(event)
        if (key)
            key.state = "normal"
    }

    Item {
        id: keyContainer

        width: childrenRect.width
        height: childrenRect.height
        anchors.centerIn: parent

        Repeater {
            id: keys
            model: keyboard.visible? keyboard.keyboardLayout.keyCount: 0

            KeyItem {
                keyboardLayout: keyboard.keyboardLayout;
                keyIndex: index
            }
        }
    }
}
