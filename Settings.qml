import QtQuick 2.14
import QtQuick.Controls 2.14

import Setting 1.0

Item {
    id: settingsRoot
    width: parent.width
    height: parent.height

    function swapSyncPref(valueOfRD1,valueOfRD2){
        rd1.checked = valueOfRD1;
        rd2.checked = valueOfRD2;
        settings.changeSettingsBool("sync-cloud", rd1.checked);
        settings.changeSettingsBool("sync-local", rd2.checked);
    }

    Rectangle{
        id:rootRec
        width: parent.width
        height: parent.height
        color: "transparent"
        Flow {
                id: flow
                anchors.fill: parent
                anchors.margins: 20
                spacing: 10
                layoutDirection: Qt.LeftToRight

                Column{
                    id:firstColumn
                    SwitchDelegate {
                        anchors.topMargin: 10
                        id: control
                        text: qsTr("Sync with cloud")
                        checked: settings.loadSetting("sync-enabled")
                        onClicked: {
                            settings.changeSettingsBool("sync-enabled", control.checked)
                            console.log(control.checked)
                        }

                        contentItem: Text {
                            rightPadding: control.indicator.width + control.spacing
                            text: control.text
                            font: control.font
                            opacity: enabled ? 1.0 : 0.3
                            color: control.down ? "#17a81a" : "#21be2b"
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                        }

                        indicator: Rectangle {
                            implicitWidth: 48
                            implicitHeight: 26
                            x: control.width - width - control.rightPadding
                            y: parent.height / 2 - height / 2
                            radius: 13
                            color: control.checked ? "#17a81a" : "transparent"
                            border.color: control.checked ? "#17a81a" : "#cccccc"

                            Rectangle {
                                x: control.checked ? parent.width - width : 0
                                width: 26
                                height: 26
                                radius: 13
                                color: control.down ? "#cccccc" : "#ffffff"
                                border.color: control.checked ? (control.down ? "#17a81a" : "#21be2b") : "#999999"
                            }
                        }

                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 40
                            visible: control.down || control.highlighted
                            color: control.down ? "#bdbebf" : "#eeeeee"
                        }
                    }



                    Column{
                        enabled: control.checked
                        anchors.leftMargin: 30
                        anchors.left: parent.left
                        RadioButton {
                            onClicked: {
                                swapSyncPref(rd1.checked,rd2.checked)
                            }

                            id: rd1
                            text: qsTr("Prefer cloud over local")
                            checked: settings.loadSetting("sync-cloud")

                            indicator: Rectangle {
                                implicitWidth: 26
                                implicitHeight: 26
                                x: rd1.leftPadding
                                y: parent.height / 2 - height / 2
                                radius: 13
                                border.color: rd1.down ? "#17a81a" : "#21be2b"

                                Rectangle {
                                    width: 14
                                    height: 14
                                    x: 6
                                    y: 6
                                    radius: 7
                                    color: rd1.down ? "#17a81a" : "#21be2b"
                                    visible: rd1.checked
                                }
                            }

                            contentItem: Text {
                                text: rd1.text
                                font: rd1.font
                                opacity: enabled ? 1.0 : 0.3
                                color: rd1.down ? "#17a81a" : "#21be2b"
                                verticalAlignment: Text.AlignVCenter
                                leftPadding: rd1.indicator.width + rd1.spacing
                            }
                        }
                        RadioButton {
                            onClicked: {
                                swapSyncPref(rd1.checked,rd2.checked)

                            }
                            id: rd2
                            text: qsTr("Prefer local over cloud")
                            checked: settings.loadSetting("sync-local")

                            indicator: Rectangle {
                                implicitWidth: 26
                                implicitHeight: 26
                                x: rd2.leftPadding
                                y: parent.height / 2 - height / 2
                                radius: 13
                                border.color: rd2.down ? "#17a81a" : "#21be2b"

                                Rectangle {
                                    width: 14
                                    height: 14
                                    x: 6
                                    y: 6
                                    radius: 7
                                    color: rd2.down ? "#17a81a" : "#21be2b"
                                    visible: rd2.checked
                                }
                            }

                            contentItem: Text {
                                text: rd2.text
                                font: rd2.font
                                opacity: enabled ? 1.0 : 0.3
                                color: rd2.down ? "#17a81a" : "#21be2b"
                                verticalAlignment: Text.AlignVCenter
                                leftPadding: rd2.indicator.width + rd2.spacing
                            }
                        }
                    }

                    Frame {
                        background: Rectangle {
                            anchors.left: parent.left
                            anchors.margins: 10
                            width: firstColumn.width
                            height: (parent.height - (parent.height - 1))
                            color: "transparent"
                            border.color: "#595959"
                            radius: 2
                        }

                    }
                }
        }
    }

}
