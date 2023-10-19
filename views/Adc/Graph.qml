import QtQuick 2.7
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtCharts 2.6

Rectangle{
    anchors.fill:parent

    property var isStarted: false
    property var lastX: 0
    property var isRaw: true

    

    Connections {
        target: _adcHandler 
         function onAdcChanged()  {
            var localAdc0 = _adcHandler.adc0
            var localAdc1 = _adcHandler.adc1

            if (!isRawCheck.checked)
            {
                var op = 3.3/4095.0
                localAdc0 = localAdc0 * op
                localAdc1 = localAdc1 * op

                var srt0 = localAdc0.toFixed(3).toString().padStart(5, '0')
                var srt1 = localAdc1.toFixed(3).toString().padStart(5, '0')
            }else{
                var srt0 = localAdc0.toString().padStart(5, '0')
                var srt1 = localAdc1.toString().padStart(5, '0')
            }
            
            lineSeriesAdc0.append(lastX,localAdc0)
            lineSeriesAdc1.append(lastX,localAdc1)

            lineSeriesAdc0.axisX.max = lastX
            lineSeriesAdc1.axisX.max = lastX

            lineSeriesAdc0.name = "Adc 0: " + srt0
            lineSeriesAdc1.name = "Adc 1: " + srt1
            if (localAdc0 > lineSeriesAdc0.axisY.max)
            {
                lineSeriesAdc0.axisY.max = localAdc0
            }
            if (localAdc1 > lineSeriesAdc1.axisY.max)
            {
                lineSeriesAdc1.axisY.max = localAdc1
            }
            
            lastX += 1
        }
    }

    function refreshGraphValues()
    {
        if (isRawCheck.checked)
        {
            var op = 4095.0/3.3
        }
        else
        { 
            var op = 3.3/4095.0
        }
        
        for (let i = 0; i < lineSeriesAdc0.count; i++) {
            var point = lineSeriesAdc0.at(i);
            lineSeriesAdc0.replace(point.x, point.y, point.x, point.y*op)

            var point = lineSeriesAdc1.at(i);
            lineSeriesAdc1.replace(point.x, point.y, point.x, point.y*op)
        }
        var max = lineSeriesAdc0.axisY.max
        lineSeriesAdc0.axisY.max = max*op
        lineSeriesAdc1.axisY.max = max*op
        
    }

    Button{
        id:ch0
        text:"Update"
        onClicked:{   
            _adcHandler.ReadRaw(0)
            _adcHandler.ReadRaw(1)
        }

    }

    Button{
        id:adcStart
        anchors.left:ch0.right
        text:(isStarted)?"Stop":"Start"
        onClicked:{
            if(isStarted)
            {
                _adcHandler.stopReading();
                isStarted=false
            }else{
                _adcHandler.startReading();
                isStarted=true     
                lineSeriesAdc0.clear()
                lineSeriesAdc1.clear()
                lastX = 0
                lineSeriesAdc0.axisX.max = 0
                lineSeriesAdc1.axisX.max = 0
            }
        }
    }

    CheckBox {
        id: isRawCheck
        anchors.right: parent.right
        anchors.top: parent.top
        checked: true
        text: qsTr("Raw")
        onToggled:refreshGraphValues()
    }

    ChartView {
        anchors.top:ch0.bottom
        anchors.left: parent.left
        anchors.right:parent.right
        anchors.bottom : parent.bottom
        anchors.margins: 0

        margins.top: 0
        margins.bottom: 0
        margins.left: 0
        margins.right: 0
        antialiasing: true

        

        //backgroundColor: "#333"

        legend.font.pixelSize: 15
        LineSeries {
            id:lineSeriesAdc0
            Component.onCompleted:{
                axisX.labelsFont.pixelSize = 15
                axisY.labelsFont.pixelSize = 15
            }
            
            name: "ADC0"
            XYPoint { x: 0; y: 0 }
            XYPoint { x: 1.1; y: 2.1 }
            XYPoint { x: 1.9; y: 3.3 }
            XYPoint { x: 2.1; y: 2.1 }
            XYPoint { x: 2.9; y: 4.9 }
            XYPoint { x: 3.4; y: 3.0 }
            XYPoint { x: 4.1; y: 3.3 }
        }

        LineSeries {
            id:lineSeriesAdc1
            name: "ADC1"
            XYPoint { x: 0; y: 0 }
            XYPoint { x: 1.1; y: 2.1 }
            XYPoint { x: 1.9; y: 3.3 }
            XYPoint { x: 2.1; y: 2.1 }
            XYPoint { x: 2.9; y: 4.9 }
            XYPoint { x: 3.4; y: 3.0 }
            XYPoint { x: 4.1; y: 3.3 }
        }
        
    }
    


}