import QtQuick 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.13

Item {
    width: 400
    height: 400
    property alias element: element

    Row {
        id: row
        x: 100
        y: 8
        width: 200
        height: 400
    }

    Switch {
        id: element
        x: 8
        y: 8
        text: qsTr("Switch")
    }
}
