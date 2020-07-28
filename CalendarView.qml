import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Private 1.0
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.2
import Event 1.0
import QtQuick.Window 2.2

Item {
    width: parent.width
    height: parent.height

    SystemPalette {
        id: systemPalette
    }

    function loadImages(currentDate){
        return sqlEventModel.eventsForDate(currentDate).length > 0;
    }
    function loadImagess(currentDate){
        return sqlEventModel.eventsForDate(currentDate).length;
    }

    Flow {
            id: row
            anchors.fill: parent
            anchors.margins: 20
            spacing: 10
            layoutDirection: Qt.RightToLeft
            Calendar {
                id: calendar
                width: (parent.width > parent.height ? parent.width * 0.6 - parent.spacing : parent.width)
                height: (parent.height > parent.width ? parent.height * 0.6 - parent.spacing : parent.height)
                frameVisible: true
                weekNumbersVisible: true
                selectedDate: new Date()
                focus: true

                style: CalendarStyle {
                    dayDelegate: Item {

                        readonly property color sameMonthDateTextColor: "#444"
                        readonly property color selectedDateColor: Qt.platform.os === "osx" ? "#3778d0" : systemPalette.highlight
                        readonly property color selectedDateTextColor: "white"
                        readonly property color differentMonthDateTextColor: "#bbb"
                        readonly property color invalidDatecolor: "#17a81a"

                        Rectangle {
                            anchors.fill: parent
                            border.color: "transparent"
                            color: styleData.date !== undefined && styleData.selected ? selectedDateColor : "transparent"
                            anchors.margins: styleData.selected ? -1 : 0
                        }

                        property alias valueImageVisible : valueImage.visible
                        Image {
                            id: valueImage
                            visible: loadImages(styleData.date)
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.margins: -1
                            width: 12
                            height: width
                            source: "images/eventindicator.png"


                        }

                        Label {
                            id: dayDelegateText
                            text: styleData.date.getDate()
                            anchors.centerIn: parent
                            color: {
                                var color = invalidDatecolor;
                                if (styleData.valid) {
                                    // Date is within the valid range.
                                    color = styleData.visibleMonth ? sameMonthDateTextColor : differentMonthDateTextColor;
                                    if (styleData.selected) {
                                        color = selectedDateTextColor;
                                    }
                                }
                                color;
                            }
                        }
                    }
                }
            }

            Component {
                id: eventListHeader

                Row {
                    id: eventDateRow
                    width: parent.width
                    height: eventDayLabel.height
                    spacing: 10

                    Label {
                        id: eventDayLabel
                        text: calendar.selectedDate.getDate()
                        font.pointSize: 35
                    }

                    Column {
                        height: eventDayLabel.height

                        Label {
                            readonly property var options: { weekday: "long" }
                            text: Qt.locale().standaloneDayName(calendar.selectedDate.getDay(), Locale.LongFormat)
                            font.pointSize: 18
                        }
                        Label {
                            text: Qt.locale().standaloneMonthName(calendar.selectedDate.getMonth())
                                  + calendar.selectedDate.toLocaleDateString(Qt.locale(), " yyyy")
                            font.pointSize: 12
                        }
                    }
                }
            }

            Rectangle {
                width: (parent.width > parent.height ? parent.width * 0.4 - parent.spacing : parent.width)
                height: (parent.height > parent.width ? parent.height * 0.4 - parent.spacing : parent.height)
                border.color: Qt.darker(color, 1.2)
                id: eventBox

                ListView {
                    id: eventsListView
                    spacing: 4
                    clip: true
                    header: eventListHeader
                    anchors.bottom: newButton.top
                    height: parent.height - newButton.height
                    anchors.fill: parent
                    anchors.margins: 10
                    //model: sqlEventModel.eventsForDate(calendar.selectedDate)
                    model: EventModel{
                        /*function myFunction(asd){
                            var mylist = asd;
                            console.log(asd);
                            return asd;
                        }*/
                        list: sqlEventModel.eventsForDate(calendar.selectedDate).length
                    }

                    delegate: Rectangle {
                        width: eventsListView.width
                        height: eventItemColumn.height
                        anchors.horizontalCenter: parent.horizontalCenter

                        Image {
                            anchors.top: parent.top
                            anchors.topMargin: 4
                            width: 12
                            height: width
                            source: "images/eventindicator.png"
                        }

                        Rectangle {
                            width: parent.width
                            height: 1
                            color: "#eee"
                        }

                        Column {
                            id: eventItemColumn
                            anchors.left: parent.left
                            anchors.leftMargin: 20
                            anchors.right: parent.right
                            height: timeLabel.height + nameLabel.height + 8

                            Label {
                                id: nameLabel
                                width: parent.width
                                wrapMode: Text.Wrap
                                text: model.eventName
                            }

                            Label {
                                id: timeLabel
                                width: parent.width
                                wrapMode: Text.Wrap
                                text: model.startDate.toLocaleTimeString(calendar.locale, Locale.ShortFormat) + " - " +
                                      model.endDate.toLocaleTimeString(calendar.locale, Locale.ShortFormat)
                                color: "#aaa"
                            }
                        }

                        Button {
                            width: deleteButton.height
                            text: qsTr("X")
                            id: deleteButton
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            onClicked: {
                                console.log("Delete " + model.id + " " + model.eventName)
                                sqlEventModel.removeOne(model.id);
                            }

                            style: ButtonStyle{
                                background: Rectangle {
                                    color: "transparent"
                                    opacity: enabled ? 1 : 0.3
                                    border.color: "red"
                                    border.width: 1
                                    radius: 20
                                }
                                label: Text {
                                    text: deleteButton.text
                                    //font: btn2.font
                                    opacity: enabled ? 1.0 : 0.3
                                    color: "red"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    elide: Text.ElideRight
                                }
                            }
                        }
                    }
                }

                Button{
                    id: newButton
                    anchors.bottom: eventBox.bottom
                    anchors.bottomMargin: 15
                    anchors.leftMargin: 15
                    anchors.rightMargin: 15
                    anchors.left: eventBox.left
                    anchors.right: eventBox.right
                    text: "Add new"
                    onClicked: dialog.visible = true
                    style: ButtonStyle{
                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 25
                            border.width: control.activeFocus ? 2 : 1
                            border.color: "#888"
                            radius: 4
                            gradient: Gradient {
                                GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
                                GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
                            }
                        }
                        label: Text {
                            text: newButton.text
                            //font: newButton.font
                            opacity: enabled ? 1.0 : 0.3
                            color: "#444"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }
                    }

                }

                Dialog {
                    visible: false
                    id:dialog
                    title: "Add new event"
                    contentItem: Rectangle{
                        implicitHeight: 200
                        implicitWidth: 300
                        color: "black"
                        opacity: enabled ? 1 : 0.3
                        border.color: "white"
                        border.width: 1
                        radius: 5
                            Row{
                                id:label
                                anchors.topMargin: 10
                                anchors.top: parent.top
                                anchors.horizontalCenter: parent.horizontalCenter

                                Text {
                                    id:labeltext
                                    text: "New event"
                                    color: "white"
                                    font.pointSize: 20
                                }
                            }

                            Row{
                                id:rowEventName
                                anchors.topMargin: 5
                                anchors.top: label.bottom
                                anchors.horizontalCenter: parent.horizontalCenter
                                Text {
                                    id: eventname
                                    text: qsTr("Event name  ")
                                    color: "white"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                TextField {
                                    id: txtEventName
                                    anchors.rightMargin: 10
                                    anchors.leftMargin: 5
                                    inputMethodHints: Qt.ImhNoPredictiveText
                                    placeholderText: "Enter event name"
                                    textColor: "white"
                                    style: TextFieldStyle{
                                        background: Rectangle{
                                            implicitWidth: 100
                                            implicitHeight: 30
                                            color: control.enabled ? "transparent" : "#353637"
                                            border.color: "white"
                                        }
                                    }

                                }
                            }

                            Row{
                                id:rowStartDate
                                anchors.topMargin: 5
                                anchors.top: rowEventName.bottom
                                anchors.horizontalCenter: parent.horizontalCenter
                                Text {
                                    id: txtStartDate
                                    text: qsTr("Start Date  ")
                                    color: "white"
                                    anchors.verticalCenter: parent.verticalCenter
                                }

                                TextField {
                                    id: startDateTXT
                                    anchors.rightMargin: 10
                                    anchors.leftMargin: 5
                                    text: calendar.selectedDate.getDate()+"-"+
                                          (calendar.selectedDate.getMonth() + 1) + "-" +
                                          calendar.selectedDate.toLocaleDateString(Qt.locale(), "yyyy")
                                    inputMethodHints: Qt.ImhNoPredictiveText
                                    placeholderText: "dd-mm-yyyy"
                                    textColor: "white"
                                    style: TextFieldStyle{
                                        background: Rectangle{
                                            implicitWidth: 90
                                            implicitHeight: 30
                                            color: control.enabled ? "transparent" : "#353637"
                                            border.color: "white"
                                        }
                                    }

                                }
                                TextField {
                                    id: startTimeTXT
                                    anchors.rightMargin: 10
                                    anchors.leftMargin: 5
                                    text: "9:00"
                                    textColor: "white"
                                    inputMethodHints: Qt.ImhNoPredictiveText
                                    placeholderText: "hh:mm"
                                    style: TextFieldStyle{
                                        background: Rectangle{
                                            implicitWidth: 50
                                            implicitHeight: 30
                                            color: control.enabled ? "transparent" : "#353637"
                                            border.color: "white"
                                        }
                                    }

                                }
                            }
                            Row{
                                id:rowEndDate
                                anchors.topMargin: 5
                                anchors.top: rowStartDate.bottom
                                anchors.horizontalCenter: rowStartDate.horizontalCenter
                                Text {
                                    id: txtEndDate
                                    text: qsTr("  End Date  ")
                                    color: "white"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                TextField {
                                    id: endDateTXT
                                    anchors.rightMargin: 10
                                    anchors.leftMargin: 5
                                    text: calendar.selectedDate.getDate()+"-"+
                                          (calendar.selectedDate.getMonth() + 1) + "-" +
                                          calendar.selectedDate.toLocaleDateString(Qt.locale(), "yyyy")
                                    inputMethodHints: Qt.ImhNoPredictiveText
                                    placeholderText: "dd-mm-yyyy"
                                    textColor: "white"
                                    style: TextFieldStyle{
                                        background: Rectangle{
                                            implicitWidth: 90
                                            implicitHeight: 30
                                            color: control.enabled ? "transparent" : "#353637"
                                            border.color: "white"
                                        }
                                    }
                                }
                                TextField {
                                    id: endTimeTXT
                                    anchors.rightMargin: 10
                                    anchors.leftMargin: 5
                                    text: "16:00"
                                    inputMethodHints: Qt.ImhNoPredictiveText
                                    placeholderText: "hh:mm"
                                    textColor: "white"
                                    style: TextFieldStyle{
                                        background: Rectangle{
                                            implicitWidth: 50
                                            implicitHeight: 30
                                            color: control.enabled ? "transparent" : "#353637"
                                            border.color: "white"
                                        }
                                    }
                                }
                            }
                            Row{
                                id:rowButtons
                                anchors.topMargin: 10
                                anchors.top: rowEndDate.bottom
                                anchors.horizontalCenter: rowEndDate.horizontalCenter

                                Button {
                                    text: qsTr("Cancel")
                                    id: btn1
                                    onClicked: dialog.visible = false
                                    style: ButtonStyle{
                                        background: Rectangle {
                                            color: "transparent"
                                            opacity: enabled ? 1 : 0.3
                                            border.color: btn1.down ? "#17a81a" : "#21be2b"
                                            border.width: 1
                                            radius: 5
                                        }
                                        label: Text {
                                            text: btn1.text
                                            //font: btn1.font
                                            opacity: enabled ? 1.0 : 0.3
                                            color: btn1.down ? "#17a81a" : "#21be2b"
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                            elide: Text.ElideRight
                                        }
                                    }
                                }
                                Button {
                                    text: qsTr("Save")
                                    id: btn2
                                    onClicked: {
                                        sqlEventModel.newEvent(txtEventName.text,
                                                               startDateTXT.text,
                                                               startTimeTXT.text,
                                                               endDateTXT.text,
                                                               endTimeTXT.text);
                                        dialog.close();
                                        calendar.update();
                                    }
                                    style: ButtonStyle{
                                        background: Rectangle {
                                            color: "transparent"
                                            opacity: enabled ? 1 : 0.3
                                            border.color: btn2.down ? "#17a81a" : "#21be2b"
                                            border.width: 1
                                            radius: 5
                                        }
                                        label: Text {
                                            text: btn2.text
                                            //font: btn2.font
                                            opacity: enabled ? 1.0 : 0.3
                                            color: btn2.down ? "#17a81a" : "#21be2b"
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                            elide: Text.ElideRight
                                        }
                                    }
                                }
                            }
                    }

                }
            }
        }
}
