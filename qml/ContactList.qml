import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.0
import Contact 1.0

ColumnLayout {
    width: parent.width
    height: parent.height
    id: contactlistLayout

    function saveSlot(){
        contactList.saveToFile()
    }

    function loadSlot(){
        contactList.loadList()
    }



    property bool loadingVisible: true

    Rectangle{
        id:loadingAnimationRec
        width: parent.width; height: parent.height
        visible:{
             var done = (contactList.dataFetchingDone() === 'true');
             console.log(done + "loool")
             done;
        }
        color: "transparent"
        LoadingAnimation{
            id:loadingAnimation
            visible: updateLoader()
        }
    }

    Frame {
        Layout.fillWidth: true
        Layout.fillHeight: true
        background: Rectangle{
            color: "transparent"
            border.color: "#21be2b"
            radius: 10
        }
        RowLayout{
            width: parent.width
            id:searchRow
            visible: true
            TextField {
                Material.accent: Material.color("21be2b",Material.Shade50)
                id: searchText
                Layout.alignment: Qt.AlignCenter
                //anchors.left: searchRow.left
                //anchors.right: btnsa.left
                anchors.left: parent.left
                anchors.right: btnsa.left
                anchors.rightMargin: 10
                anchors.leftMargin: 5
                width: parent.fillWidth
                text: ""
                inputMethodHints: Qt.ImhNoPredictiveText
                onTextChanged: {
                    contactList.searchContacts(searchText.text)
                }
                Layout.fillWidth: true
            }
            Button {
                id: btnsa
                text: qsTr("Search")
                contentItem: Text {
                    text: btnsa.text
                    font: btnsa.font
                    opacity: enabled ? 1.0 : 0.3
                    color: btnsa.down ? "#17a81a" : "#21be2b"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
                background: Rectangle {
                    color: "transparent"
                    opacity: enabled ? 1 : 0.3
                    border.color: btnsa.down ? "#17a81a" : "#21be2b"
                    border.width: 1
                    radius: 5
                }
            }

        }

        ListView {
            anchors.top: searchRow.bottom
            id: contactListView
            width: parent.width
            implicitHeight: parent.height
            anchors.bottom: parent.bottom
            clip: true

            model: ContactModel {
                list: contactList
            }
            spacing: 5
            delegate: Frame{
                spacing: 10
                width: contactListView.width
                ColumnLayout {
                    width: parent.width
                    RowLayout{
                        TextField {
                            id: nametxt
                            text: model.fullname
                            onEditingFinished: model.fullname = text
                            Layout.fillWidth: true
                        }
                        Rectangle{
                            id: idRec
                            width: parent.width
                            height: parent.height
                            color: "transparent"
                            MouseArea{
                                id: mousearea
                                anchors.fill: parent
                                onClicked: {
                                    expandable_box.visible == true ?
                                                closeFrame.start() :
                                                openFrame.start()
                                }

                            }
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
                            Rectangle{
                                id: idRec2
                                width: parent.width
                                height: parent.height
                                color: "transparent"
                                MouseArea{
                                    id: mousearea2
                                    anchors.fill: parent
                                    onClicked: {
                                        expandable_box.visible == true ?
                                                    closeFrame.start() :
                                                    openFrame.start()
                                    }

                                }
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
                                onClicked: dialog.visible = true;
                                Layout.fillWidth: true
                            }
                            Dialog {
                                visible: false
                                id:dialog
                                title: "Delete"
                                contentItem: Rectangle {
                                    implicitHeight: 100
                                    implicitWidth: 300
                                    color: "black"
                                    opacity: enabled ? 1 : 0.3
                                    border.color: "#21be2b"
                                    border.width: 1
                                    radius: 5
                                    RowLayout{
                                        anchors.topMargin: 15
                                        anchors.top: parent.top
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        Text {
                                            text: "Do you want to delete " + model.fullname
                                            color: "#21be2b"
                                        }
                                    }
                                    RowLayout {
                                        anchors.topMargin: 10
                                        anchors.bottomMargin: 15
                                        anchors.bottom: parent.bottom
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        Button {
                                            text: qsTr("Cancel")
                                            id: btn1
                                            Layout.fillWidth: true
                                            onClicked: dialog.visible = false
                                            contentItem: Text {
                                                text: btn1.text
                                                font: btn1.font
                                                opacity: enabled ? 1.0 : 0.3
                                                color: btn1.down ? "#17a81a" : "#21be2b"
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                                elide: Text.ElideRight
                                            }
                                            background: Rectangle {
                                                color: "transparent"
                                                implicitWidth: 100
                                                implicitHeight: 40
                                                opacity: enabled ? 1 : 0.3
                                                border.color: btn1.down ? "#17a81a" : "#21be2b"
                                                border.width: 1
                                                radius: 5
                                            }
                                        }
                                        Button {
                                            id: btn2
                                            text: qsTr("Remove")
                                            Layout.fillWidth: true
                                            onClicked: {
                                                contactList.removeOne(index, true)
                                                dialog.visible = false
                                            }
                                            contentItem: Text {
                                                text: btn2.text
                                                font: btn2.font
                                                opacity: enabled ? 1.0 : 0.3
                                                color: btn2.down ? "#17a81a" : "#21be2b"
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                                elide: Text.ElideRight
                                            }
                                            background: Rectangle {
                                                color: "transparent"
                                                implicitWidth: 100
                                                implicitHeight: 40
                                                opacity: enabled ? 1 : 0.3
                                                border.color: btn2.down ? "#17a81a" : "#21be2b"
                                                border.width: 1
                                                radius: 5
                                            }
                                        }
                                    }

                                }
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
                                            contactList.saveChanges(index, nametxt.text, mobiletxt.text, emailtxt.text, model.id);
                                            console.log(index + " " + model.id);
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
            onClicked: {
                contactList.appendItem();
                contactListView.flick(0,-1000);
            }
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

