// LottieHeart.qml

import QtQuick 2.0
import Qt.labs.lottieqt 1.0

Item {
     id: root
     property bool selected: false
     LottieAnimation {
         id: animation
         anchors.centerIn: parent
         source: "qrc:/images/images.json"
         scale: 0.4
         autoPlay: false
     }
     MouseArea {
         anchors.fill: parent
         onClicked: {
             if (root.selected) {
                animation.gotoAndStop(0)
             } else {
                animation.gotoAndPlay(0)
             }
             root.selected = !root.selected
         }
     }
 }
