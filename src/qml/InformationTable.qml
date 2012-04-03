import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

Item {
    property alias model: repeator.model
    width: parent.width
    height: childrenRect.height

    Column {
        id: column
        spacing: 3
        width: parent.width
        height: childrenRect.height

        Repeater {
            id: repeator
            Row {
                spacing: 5
                height: Math.max(titleLabel.height, valueLabel.height)
                width: column.width
                PlasmaComponents.Label {
                    id: titleLabel
                    width: Math.round((parent.width - parent.spacing) / 2)
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignTop
                    text: title
                    wrapMode: Text.Wrap
                    height: Math.max(paintedHeight, valueLabel.paintedHeight)
                }
                PlasmaComponents.Label {
                    id: valueLabel
                    width: parent.width - titleLabel.width - parent.spacing
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignBottom
                    text: value
                    font.weight: Font.Bold
                    wrapMode: Text.Wrap
                    height: Math.max(paintedHeight, titleLabel.paintedHeight)
                }
            }
        }
    }
}
