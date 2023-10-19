#include "receiverThread.h"

#include <QTime>

#include <linux/spi/spidev.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <fcntl.h>
#include <stdio.h>

#include <wiringSerial.h>
#include <termios.h>

#include <QString>
#include <QDebug>

ReceiverThread::ReceiverThread(QObject *parent) :
    QThread(parent)
{
}

ReceiverThread::~ReceiverThread()
{
    m_mutex.lock();
    m_quit = true;
    m_mutex.unlock();
    wait();
}

void ReceiverThread::startReceiver(const QString &portName)
{
    const QMutexLocker locker(&m_mutex);

    struct termios options ;
    uart0_fd = open (portName.toLocal8Bit(), O_RDWR | O_NOCTTY);
    if(uart0_fd == -1)
        return;

    fcntl (uart0_fd, F_SETFL, O_RDWR) ;

    // Get and modify current options:

    tcgetattr (uart0_fd, &options) ;

        cfmakeraw   (&options) ;
        cfsetispeed (&options, B115200) ;
        cfsetospeed (&options, B115200) ;

        options.c_cflag |= (CLOCAL | CREAD) ;
        options.c_cflag &= ~PARENB ;
        options.c_cflag &= ~CSTOPB ;
        options.c_cflag &= ~CSIZE ;
        options.c_cflag |= CS8 ;
        options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG) ;
        options.c_oflag &= ~OPOST ;

        options.c_cc [VMIN]  =   0 ;
        options.c_cc [VTIME] = 100 ;	// Ten seconds (100 deciseconds)

    tcsetattr (uart0_fd, TCSANOW, &options) ;

    /*ioctl (uart0_fd, TIOCMGET, &status);

    status |= TIOCM_DTR ;
    status |= TIOCM_RTS ;

    ioctl (uart0_fd, TIOCMSET, &status);*/



    if (!isRunning() && uart0_fd)
        start();
}

void ReceiverThread::run()
{

    char buffer[200];
    QString data;

    while (!m_quit) {
        int rcount = read(uart0_fd, &buffer, sizeof(buffer));
        if (rcount < 0) {
            perror("Read");
            return;
        }
        else {
            buffer[rcount] = 0;
            data.clear();
            data = buffer;
            emit newDataInSerial(data);
        }
    }

}
