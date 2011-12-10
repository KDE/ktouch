import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore
import ktouch 1.0

Item
{
    id: meter

    property variant ellapsedTime

    width: childrenRect.width
    height: childrenRect.height

    Row {
        AnalogEllapsedTimeMeter {
            ellapsedTime: meter.ellapsedTime
        }
        DigitalMeter {
            label: i18n("Ellapsed time")
            value: formatTime(meter.ellapsedTime)
            oldValue: "+/- 00:00.0"
        }
    }
}
