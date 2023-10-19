import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import "views/"

ApplicationWindow {

    visible: true

    width: 800
    height: 600
    property var active: 0

    Rectangle {
        id: moduleList
        width:200
        anchors.top:parent.top
        anchors.bottom:parent.bottom
        color: "#333"
        ColumnLayout {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 5
            anchors.rightMargin: 5

            spacing: 5
            Rectangle {
                Layout.preferredHeight:5
            }
            Text{
                text:"LabGluon\nRpi Controller"
                font.weight: Font.Bold
                color:"white"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                
                
            }
            Button {
                Layout.preferredHeight:40
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop
                text: "ADC"
                background: Rectangle {
                    color: (active==0)?"#ffb845":"#eee"
                }
                onClicked: active=0
            }
            Button {
                Layout.preferredHeight:40
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop
                text: "UART"
                background: Rectangle {
                    color: (active==1)?"#ffb845":"#eee"
                }
                onClicked: active=1
            }
            Rectangle {
                Layout.fillHeight:true
            }

        }
    }

    Rectangle {
        anchors.left: moduleList.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom


        StackLayout {
            anchors.fill:parent
            currentIndex: active
            Item {
                id: adcPage
                Adc{
                    anchors.fill:parent
                }
            }
            Item {
                id: discoverTab
                Uart{
                    anchors.fill:parent
                }
            }
            Item {
                id: activityTab
            }
        }
        
    }


}