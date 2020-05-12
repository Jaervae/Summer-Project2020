import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import Contact 1.0

ColumnLayout {
    width: parent.width - 10
    height: parent.height - 10
    Frame {
        Layout.fillWidth: true
        Layout.fillHeight: true
        ListView {
            implicitWidth: parent.width
            implicitHeight: parent.height
            clip: true

            model: ContactModel {
                list: contactList
            }
            spacing: 5
            delegate: Frame{
                spacing: 10
                width: parent.width
                ColumnLayout {
                    width: parent.width
                    RowLayout{
                        TextField {
                            id: nametxt
                            text: model.description
                            onEditingFinished: model.description = text
                            Layout.fillWidth: true
                        }
                    }
                    MouseArea{
                        id: mousearea
                        anchors.fill: parent
                        onClicked: expandable_box.visible = !expandable_box.visible
                    }
                    ColumnLayout{
                        width: parent.width
                        id: expandable_box
                        visible: false
                        RowLayout{
                            TextField {
                                id: mobiletxt
                                text: model.mobile
                                onEditingFinished: model.mobile = text
                                Layout.fillWidth: true
                            }
                            TextField {
                                id: emailtxt
                                text: model.email
                                onEditingFinished: model.email = text
                                Layout.fillWidth: true
                            }
                        }
                        MouseArea{
                            id: mousearea2
                            anchors.fill: parent
                            onClicked: expandable_box.visible = !expandable_box.visible
                        }
                        RowLayout{
                            Button {
                                text: qsTr("Delete")
                                onClicked: contactList.removeOne(index);
                                Layout.fillWidth: true
                            }
                            RowLayout{
                                id: buttonEditRow
                                visible: false
                                Button {
                                    text: qsTr("Cancel")
                                    onClicked: function(){
                                        buttonEditRow.visible = false;
                                        buttonEnableEdit.visible = true;
                                        mousearea.enabled = true;
                                        mousearea2.enabled = true;
                                    }
                                    Layout.fillWidth: true
                                }
                                Button {
                                    text: qsTr("Save changes")
                                    onClicked: function(){
                                        contactList.saveChanges(index, nametxt.text, mobiletxt.text, emailtxt.text);
                                        buttonEditRow.visible = false;
                                        buttonEnableEdit.visible = true;
                                        mousearea.enabled = true;
                                        mousearea2.enabled = true;
                                    }
                                    Layout.fillWidth: true
                                }
                            }

                            Button {
                                text: qsTr("Enable editing")
                                id: buttonEnableEdit
                                visible: true
                                 onClicked: function(){
                                    buttonEditRow.visible = true;
                                    buttonEnableEdit.visible = false;
                                    mousearea.enabled = false;
                                    mousearea2.enabled = false;
                                }
                                Layout.fillWidth: true
                            }

                        }
                    }

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
            text: qsTr("Remove empties")
            onClicked: contactList.removeCompletedItems()
            Layout.fillWidth: true
        }
    }
}
