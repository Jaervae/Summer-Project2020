import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    id:rootWindow
    title: qsTr("QT mobile app")
    signal saveSignal()
    signal loadSignal()

    Loader {
      // position the Loader in the center
      // of the parent
      id: loader
      anchors.fill: parent
      sourceComponent: mainScreenArea
    }


    header: ToolBar{
        id:toolBar
        topPadding: Qt.platform.os === "ios" ? Screen.height - Screen.desktopAvailableHeight : 0
        contentItem: Text {
            text: "QT mobile app"
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
                Layout.alignment: Qt.AlignLeft
                hoverEnabled: false
                contentItem: Image {
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    id: backimage
                    source: "images/back2x.png"
                }
                onClicked: {
                    loader.active = false
                    rootStackLayout.visible = true
                }

                //onClicked: stack.pop()
            }


            ToolButton {
                Layout.alignment: Qt.AlignRight || Qt.AlignVCenter
                hoverEnabled: false
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
                        text: "Sync"
                        id: loadItem
                        onClicked: rootWindow.loadSignal()
                    }
                    MenuItem {
                        text: "Settings"
                        onClicked: {
                            loader.active = true
                            loader.source = "Settings.qml"
                            rootStackLayout.visible = false
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
            text: qsTr("Calendar")
        }
        TabButton {
            text: qsTr("Activity")
        }
    }

    Rectangle{
        id: mainScreenArea
        width: parent.width
        height: (parent.height - toolBar.height - bar.height)
        anchors.bottom: bar.top
        anchors.top: toolBar.bottom
        color: "transparent"
        StackLayout {
            id:rootStackLayout
            width: parent.width
            height: parent.height
            visible: true
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

}


