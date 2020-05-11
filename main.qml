import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Scroll")

    ContactList{
        anchors.centerIn: parent
    }

}


