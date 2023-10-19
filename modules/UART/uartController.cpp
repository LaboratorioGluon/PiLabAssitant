#include "uartController.h"


uartController::uartController()
{
    connect(&receiverTh, SIGNAL(newDataInSerial(QString)), this, SIGNAL(newDataInSerial(QString)));
}


void uartController::startThread()
{
    receiverTh.startReceiver("/dev/ttyS0");
}