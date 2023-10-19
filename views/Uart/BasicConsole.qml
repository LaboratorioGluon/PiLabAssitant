import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Rectangle{
    anchors.fill: parent
    property var newData: ""

    Component.onCompleted: _uartHandler.startThread()

    function scroll_to_bottom() {
        scrollView.ScrollBar.vertical.position = 1.0 - scrollView.ScrollBar.vertical.size
        return;
    }

    Connections {
        target: _uartHandler 
         function onNewDataInSerial(data)  {
            newData += data
         }
    }

    Timer{
        interval: 500; running: true; repeat: true
        onTriggered: {
            uartlog.text += newData
            uartlogrect.height = uartlog.height
            newData =""
            //print(scrollView.ScrollBar.vertical.position)
            scroll_to_bottom()
        }
    }

    Button{
        id:ch0
        text:"Clear"
        onClicked:{   
            uartlog.text = ""
        }
    }
    
    Rectangle{
        id: uartlogrect
        
        anchors.top: ch0.bottom
        anchors.left: parent.left
        anchors.right:parent.right
        anchors.bottom: parent.bottom

        

        color: "#222"
        ScrollView  {
            id: scrollView
            anchors.fill:parent
            padding: 10
            clip:true
            /*contentWidth: uartlog.width
            contentHeight: uartlog.height*/
            Text{
                id: uartlog
                font.pixelSize: 12
                color: "#eee"
            }
        }
        
    }
    
}