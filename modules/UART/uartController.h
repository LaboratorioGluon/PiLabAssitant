#ifndef __UARTCONTROLLER_H__
#define __UARTCONTROLLER_H__

#include <QObject>
#include <QThread>
#include "receiverThread.h"

class uartController: public QObject
{
    Q_OBJECT
public:
    uartController();

signals:
    // void gpioTriggered(int gpioNum);
    void newDataInSerial(QString);

public slots:

    void startThread();


private:
    ReceiverThread receiverTh;
};


#endif //__UARTCONTROLLER_H__