import QtQuick 1.0
import ktouch 1.0

Text
{
    property KeyChar keyChar

    color: key.state == "normal"? "#333": "#222"
    smooth: true
    font.pixelSize: referenceKey.height * scaleFactor / 3
    text: keyChar && String.fromCharCode(keyChar.value) || ""
}
