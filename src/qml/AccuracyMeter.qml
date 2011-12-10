import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore

Item
{
    id: meter

    property real accuracy: 1.0

    width: childrenRect.width
    height: childrenRect.height

    Row {
        AnalogAccuracyMeter {
            accuracy: meter.accuracy
        }
        DigitalMeter {
            label: i18n("Hit ratio")
            value: formatAccuracy(meter.accuracy)
            oldValue: "+/- 0.0%"
        }
    }

}
