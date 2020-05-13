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
        background: Rectangle{
            color: "transparent"
            border.color: "#21be2b"
            radius: 10
        }

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
                        onClicked: {
                            expandable_box.visible == true ?
                                        closeFrame.start() :
                                        openFrame.start()
                        }
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
                            onClicked: {
                                expandable_box.visible == true ?
                                            closeFrame.start() :
                                            openFrame.start()
                            }
                        }
                        PropertyAnimation{
                            id: openFrame
                            target: expandable_box
                            properties: "visible"
                            to: "true"
                            duration: 10
                        }
                        PropertyAnimation{
                            id: closeFrame
                            target: expandable_box
                            properties: "visible"
                            to: "false"
                            duration: 10
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
                                        if (nametxt.text != "" && nametxt.text != null && mobiletxt.text != "" && mobiletxt.text != null ){
                                            contactList.saveChanges(index, nametxt.text, mobiletxt.text, emailtxt.text);
                                            buttonEditRow.visible = false;
                                            buttonEnableEdit.visible = true;
                                            mousearea.enabled = true;
                                            mousearea2.enabled = true;
                                        }
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
            id: control
            onClicked: contactList.appendItem()
            Layout.fillWidth: true
            contentItem: Text {
                text: control.text
                font: control.font
                opacity: enabled ? 1.0 : 0.3
                color: control.down ? "#17a81a" : "#21be2b"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
            background: Rectangle {
                color: "transparent"
                implicitWidth: 100
                implicitHeight: 40
                opacity: enabled ? 1 : 0.3
                border.color: control.down ? "#17a81a" : "#21be2b"
                border.width: 1
                radius: 5
            }
        }
        Button {
            id: controlbtnTwo
            text: qsTr("Remove empties")
            onClicked: contactList.removeCompletedItems()
            Layout.fillWidth: true
            contentItem: Text {
                text: controlbtnTwo.text
                font: controlbtnTwo.font
                opacity: enabled ? 1.0 : 0.3
                color: controlbtnTwo.down ? "#17a81a" : "#21be2b"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
            background: Rectangle {
                color: "transparent"
                implicitWidth: 100
                implicitHeight: 40
                opacity: enabled ? 1 : 0.3
                border.color: controlbtnTwo.down ? "#17a81a" : "#21be2b"
                border.width: 1
                radius: 5
            }
        }
    }
}
