import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import 'Adc/'

Rectangle {
        
        color:"#ddd"

        TabBar {
            id: bar
            width: parent.width
            TabButton {
                text:"First"
            }
            TabButton {
                text:"Second"
            }
            TabButton {
                text:""
            }

        }
        StackLayout {
            width: parent.width
            anchors.top: bar.bottom
            anchors.bottom: parent.bottom
            currentIndex: bar.currentIndex
            Item {
                id: homeTab
                Graph{
                    anchors.fill:parent
                }
            }
            Item {
                id: discoverTab
                Text{
                    text:"Prueba 2"
                }
            }
            Item {
                id: activityTab
            }
        }
}