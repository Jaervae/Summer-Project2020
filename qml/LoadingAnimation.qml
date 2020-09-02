import QtQuick 2.14

Item {
    id:root
    width: parent.width; height: parent.height - 20
    Rectangle {
        width: animation.width; height: animation.height
        anchors.centerIn: parent

        AnimatedImage { id: animation; source: "qrc:/images/loading_anim_2.gif" }
    }
}
