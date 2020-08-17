import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Private 1.0
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2
import Event 1.0
import CurrentDayEvents 1.0

Item {
    width: parent.width
    height: parent.height

    SystemPalette {
        id: systemPalette
    }

    function loadImages(currentDate){
        return sqlEvent.eventsForDate(currentDate).length > 0;
    }

    readonly property color blackbg: "#191919"
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
                onClicked: eventsListView.model = sqlEvent.eventsForDate(calendar.selectedDate)
                focus: true
                 style: CalendarStyle {
                     background: Rectangle {
                             color: "#fff"
                             implicitWidth: Math.max(250, Math.round(TextSingleton.implicitHeight * 14))
                             implicitHeight: Math.max(250, Math.round(TextSingleton.implicitHeight * 14))
                    }
                    dayDelegate: Rectangle {
                        ///Color values
                        readonly property color sameMonthDateTextColor: "white" //#444
                        readonly property color selectedDateColor: Qt.platform.os === "osx" ? "#3778d0" : selectedDateBG
                        readonly property color selectedDateTextColor: "white"
                        readonly property color differentMonthDateTextColor: "#444"
                        readonly property color invalidDatecolor: "red"
                        readonly property color selectedDateBG: "green" //"#21be2b"

                        color: styleData.selected ? "#FF2E7BD2" : (styleData.visibleMonth && styleData.valid ? "#191919" : "#191919");

                        Label{
                            font.pixelSize: 8
                            anchors.right: parent.right
                            anchors.top: parent.top
                            width: 12
                            height: 10
                            horizontalAlignment: Text.AlignHCenter
                            text: sqlEvent.eventcounter(styleData.date) > 0 ? sqlEvent.eventcounter(styleData.date) : ""
                            color: "orange"
                        }

                        //BG of dates
                        Rectangle {
                            anchors.fill: parent
                            border.color: "transparent"

                            color: styleData.date !== undefined && styleData.selected ? selectedDateColor : "transparent"
                            anchors.margins: styleData.selected ? -1 : 0
                        }

                        //Event image
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

                        //Day indicators
                        Label {
                            id: dayDelegateText
                            text: styleData.date.getDate()
                            anchors.centerIn: parent
                            color: {
                                var color = invalidDatecolor;
                                if (styleData.valid) {
                                    color = styleData.visibleMonth ? sameMonthDateTextColor : differentMonthDateTextColor;
                                    if (styleData.selected) {
                                        color = selectedDateTextColor;
                                    }
                                }
                                color;
                            }
                        }
                    }

                    weekNumberDelegate: Item {
                        //Hide week number
                    }

                    dayOfWeekDelegate: Rectangle {
                            color: blackbg
                            implicitHeight: Math.round(TextSingleton.implicitHeight * 2.25)
                            Label {
                                id:dayofweeklabel
                                text: control.locale.dayName(styleData.dayOfWeek, control.dayOfWeekFormat)
                                anchors.centerIn: parent
                                color: {
                                    var color = "yellow";
                                    if(dayofweeklabel.text == "su" || dayofweeklabel.text == "Sun") {
                                        color= "red";
                                    }
                                    color;
                                }
                            }
                    }

                    navigationBar: Rectangle {
                        height: Math.round(TextSingleton.implicitHeight * 2.73)
                        color: blackbg
                        Rectangle {
                            color: Qt.rgba(1,1,1,0.6)
                            height: 1
                            width: parent.width
                        }

                        Rectangle {
                            anchors.bottom: parent.bottom
                            height: 1
                            width: parent.width
                            color: "#ddd"
                        }
                        HoverButton {
                            id: previousMonth
                            width: parent.height
                            height: width
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            source: "images/leftarrow2x.png"
                            onClicked: calendar.showPreviousMonth()
                        }
                        Label {
                            id: dateText
                            text: styleData.title
                            color: "green"
                            elide: Text.ElideRight
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: TextSingleton.implicitHeight * 1.25
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: previousMonth.right
                            anchors.leftMargin: 2
                            anchors.right: nextMonth.left
                            anchors.rightMargin: 2
                        }
                        HoverButton {
                            id: nextMonth
                            width: parent.height
                            height: width
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            source: "images/rightarrow2x.png"
                            onClicked: calendar.showNextMonth()
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
                        color: "green"
                    }

                    Column {
                        height: eventDayLabel.height

                        Label {
                            readonly property var options: { weekday: "long" }
                            text: Qt.locale().standaloneDayName(calendar.selectedDate.getDay(), Locale.LongFormat)
                            font.pointSize: 18
                            color: "green"
                        }
                        Label {
                            text: Qt.locale().standaloneMonthName(calendar.selectedDate.getMonth())
                                  + calendar.selectedDate.toLocaleDateString(Qt.locale(), " yyyy")
                            font.pointSize: 12
                            color: "green"
                        }
                    }
                }
            }
            Rectangle {
                width: (parent.width > parent.height ? parent.width * 0.4 - parent.spacing : parent.width)
                height: (parent.height > parent.width ? parent.height * 0.4 - parent.spacing : parent.height)
                border.color: Qt.darker(color, 1.2)
                id: eventBox
                color: blackbg

                ListView {
                    id: eventsListView
                    spacing: 4
                    clip: true
                    header: eventListHeader
                    anchors.bottom: newButton.top
                    height: parent.height - newButton.height
                    anchors.fill: parent
                    anchors.margins: 10
                    model: sqlEvent.eventsForDate(calendar.selectedDate)

                    delegate: Rectangle {
                        width: eventsListView.width
                        height: eventItemColumn.height
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: blackbg

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
                                text: modelData.name
                                color: "white"
                            }

                            Label {
                                id: timeLabel
                                width: parent.width
                                wrapMode: Text.Wrap
                                text: modelData.startDate.toLocaleTimeString(calendar.locale, Locale.ShortFormat) + " - " +
                                      modelData.endDate.toLocaleTimeString(calendar.locale, Locale.ShortFormat)
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
                                console.log("Delete " + modelData.dataId + " " + modelData.name)
                                console.log(modelData.dataId)
                                sqlEvent.removeOne(modelData.dataId);
                                calendar.showPreviousMonth();
                                calendar.showNextMonth();
                                eventsListView.model = sqlEvent.eventsForDate(calendar.selectedDate);
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
                            border.color: "green"
                            border.width: 2
                            radius: 4
                            gradient: Gradient {
                                GradientStop { position: 0 ; color: calendar.pressed ? "white" : "#ccc" }
                                GradientStop { position: 1 ; color: calendar.pressed ? "#ccc" : "white" }
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
                                    color: "green"
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
                                            color: calendar.enabled ? "transparent" : "#353637"
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
                                            color: calendar.enabled ? "transparent" : "#353637"
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
                                            color: calendar.enabled ? "transparent" : "#353637"
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
                                            color: calendar.enabled ? "transparent" : "#353637"
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
                                            color: calendar.enabled ? "transparent" : "#353637"
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
                                    onClicked: {
                                        txtEventName.text = "";
                                        dialog.visible = false
                                    }
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
                                        sqlEvent.newEvent(txtEventName.text,
                                                               startDateTXT.text,
                                                               startTimeTXT.text,
                                                               endDateTXT.text,
                                                               endTimeTXT.text);
                                        txtEventName.text = "";
                                        dialog.close();
                                        eventsListView.model = sqlEvent.eventsForDate(calendar.selectedDate);
                                        calendar.showNextMonth();
                                        calendar.showPreviousMonth();
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
