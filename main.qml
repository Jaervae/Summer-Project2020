import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    id:rootWindow
    title: qsTr("Scroll")
    signal saveSignal()
    signal loadSignal()

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
                hoverEnabled: false
                anchors.left: parent.left
                contentItem: Image {
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    id: backimage
                    source: "images/back2x.png"
                }
                onClicked: stack.pop()
            }

            ToolButton {
                hoverEnabled: false
                anchors.right: parent.right
                id: menubutton
                contentItem: Image {
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    id: menuimage
                    source: "images/menu2x.png"
                }

                onClicked: {
                    if(menu.visible) menu.close()
                    else menu.open()
                }
                Menu {
                    id: menu
                    y: menubutton.height
                    MenuItem {
                        text: "Save"
                        id: saveItem
                        onClicked: rootWindow.saveSignal()
                    }
                    MenuItem {
                        text: "Load"
                        id: loadItem
                        onClicked: rootWindow.loadSignal()
                    }
                    MenuItem {
                        text: "Settings"
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
                id: contactListTab
                Component.onCompleted: {
                    rootWindow.saveSignal.connect(contactListTab.saveSlot)
                    rootWindow.loadSignal.connect(contactListTab.loadSlot)
                }


            }

        }

        Item {
            id: discoverTab
           CalendarView{
               id: discoverListTab
           }
        }
        Item {
            id: activityTab
        }
    }


}


