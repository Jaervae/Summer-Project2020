import QtQuick 2.14
import QtQuick.Controls 2.14

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Scroll")


    Row {
        id: row
        width: parent.width
        height: parent.height

        Button{
            id: button
            width: parent.width
            height: 100
            text: "GET Data"

        }

        ScrollView {
            anchors.right: parent.right
            anchors.top: button.bottom
            anchors.left: parent.left

            ListView {
                width: parent.width
                model: 20
                delegate: ItemDelegate {
                    text: "Item " + (index + 1)
                    width: parent.width
                    onPressed: console.log(index +1 )
                    onPressAndHold: console.log('hlld')

                }
            }

        }

    }
}


