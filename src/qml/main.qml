import QtQuick 1.1
import ktouch 1.0

Item {

    id: main

    property DataIndex dataIndex: dataAccess.loadDataIndex()
    property string name: keyboardLayoutName
    property KeyboardLayout keyboardLayout: lookupKeyboardLayout(keyboardLayoutName)

    function lookupKeyboardLayout(name)
    {
        for (var i = 0; i < dataIndex.keyboardLayoutCount; i++)
        {
            var dataIndexLayout = dataIndex.keyboardLayout(i)
            if (name == dataIndexLayout.name)
                return dataAccess.loadResourceKeyboardLayout(dataIndexLayout.path)

        }
        return null;
    }

    DataAccess {
        id: dataAccess
    }

    TrainingScreen
    {
        id: trainingScreen
        anchors.fill: parent
        keyboardLayout: main.keyboardLayout
    }

}
