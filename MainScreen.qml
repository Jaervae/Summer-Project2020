import QtQuick 2.14
import QtQuick.Controls 2.14

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Scroll")

    function getData() {
        var xmlhttp = new XMLHttpRequest();
        var url = "https://samplechat.firebaseio-demo.com/users/jack/name.json";

        xmlhttp.onreadystatechange=function() {
            if (xmlhttp.readyState === 4 && xmlhttp.status === 200) {
                myFunction(xmlhttp.responseText);
            }
        }
        xmlhttp.open("GET", url, true);
        xmlhttp.send();
    }

    function myFunction(json) {
        var obj = JSON.parse(json);
        listview.model.append( {jsondata: obj.first +" "+ obj.last })
    }

    Row {
        id: row
        width: parent.width
        height: parent.height

        Button{
            id: button
            width: parent.width
            height: 100
            text: "GET Data"
            onClicked: getData()

        }

        ScrollView {
            anchors.right: parent.right
            anchors.top: button.bottom
            anchors.left: parent.left



            ListView {
                width: parent.width
                model: 20
                delegate: Text {
                    text: jsondata
                }
            }
        }
    }
}
