import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import Contact 1.0

ColumnLayout {
    Frame {
        Layout.fillWidth: true

        ListView {
            implicitWidth: 250
            implicitHeight: 250
            anchors.fill: parent
            clip: true

            model: ContactModel {
                list: contactList
            }

            delegate: RowLayout {
                width: parent.width

                CheckBox {
                    checked: model.done
                    onClicked: model.done = checked
                }
                TextField {
                    text: model.description
                    onEditingFinished: model.description = text
                    Layout.fillWidth: true
                }
            }
        }
    }

    RowLayout {
        Button {
            text: qsTr("Add new item")
            onClicked: contactList.appendItem()
            Layout.fillWidth: true
        }
        Button {
            text: qsTr("Remove completed")
            onClicked: contactList.removeCompletedItems()
            Layout.fillWidth: true
        }
    }
}
