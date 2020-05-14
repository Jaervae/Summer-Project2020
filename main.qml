import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    id:window
    title: qsTr("Scroll")

    header: ToolBar{
        id:toolBar
        topPadding: Qt.platform.os === "ios" ? Screen.height - Screen.desktopAvailableHeight : 0
        contentItem: Text {
            text: "Contactlist"
            font.pointSize: 22
            opacity: enabled ? 1.0 : 0.3
            color: "#21be2b"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }
        background: Rectangle {
            color: "transparent"
            implicitWidth: 100
            implicitHeight: 40
            opacity: enabled ? 1 : 0.3
        }
        RowLayout{
            anchors.fill: parent
            ToolButton {
                anchors.left: parent.left
                text: qsTr("‹")
                onClicked: stack.pop()
            }
            ToolButton {
                anchors.right: parent.right
                id: menubutton
                text: qsTr("⋮")
                onClicked: {
                    if(menu.visible) menu.close()
                    else menu.open()
                }
                Menu {
                    id: menu
                    y: menubutton.height
                    MenuItem {
                        text: "New..."
                    }
                    MenuItem {
                        text: "Open..."
                    }
                    MenuItem {
                        text: "Settings"
                        onClicked: {

                        }
                    }
                    MenuSeparator{}
                    MenuItem {
                        text: "Quit"
                        onClicked: Qt.callLater(Qt.quit)
                    }
                }
            }

        }

    }
    footer: TabBar {
        id: bar
        width: parent.width
        Material.accent: Material.color("21be2b",Material.Shade50)
        TabButton {
            text: qsTr("Contacts")

        }
        TabButton {
            text: qsTr("Discover")
        }
        TabButton {
            text: qsTr("Activity")
        }
    }

    StackLayout {
        width: parent.width
        height: parent.height
        anchors.bottom: bar.top
        currentIndex: bar.currentIndex
        Item {
            id: contactTab
            ContactList{
            }
        }
        Item {
            id: discoverTab
        }
        Item {
            id: activityTab
        }
    }


    /*
    ContactList{
        anchors.centerIn: parent
    }
    */


}


