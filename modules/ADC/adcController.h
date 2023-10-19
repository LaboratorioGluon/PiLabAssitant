#ifndef __ADCCONTROLLER__
#define __ADCCONTROLLER__

#include <QObject>
#include <QThread>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>


class adcWorker : public QThread
{
    Q_OBJECT
    void run() override
    {
        uint8_t tx[] = {
                0x18 | 0 << 2,
                0,
                0};

        struct spi_ioc_transfer tr = {
                .tx_buf = (unsigned long)tx,
                .rx_buf = (unsigned long)rx,
                .len = 3,
                .speed_hz = 500000,
                .delay_usecs = 100,
                .bits_per_word = 8};
        keepReading = true;
        while (keepReading)
        {
            tx[0] = 0x18 | 0 << 2;

            int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);

            if (ret < 1)
            {
                printf("Error ADC reading\n");
                return;
            }

            //uint32_t data = ((rx[2] & 0x1f) << 8) + (rx[1]<<);
            uint32_t data = (rx[2]>> 4) + (rx[1]<<4);
            *adc0 = data;

            tx[0] = 0x18 | 1 << 2;
            ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
            data = (rx[2]>> 4) + (rx[1]<<4);
            *adc1 = data;
            adcChanged();
            msleep(50);
        }
    }

signals:
    // void gpioTriggered(int gpioNum);
    void adcChanged();

public slots:
    void readStop()
    {
        keepReading = false;
    }

public:
    void setAdc0(int *destAdc0){ adc0 = destAdc0;};
    void setAdc1(int *destAdc1){ adc1 = destAdc1;};
    void setFd(int *_fd){ fd = *_fd;}

private:

    uint8_t tx[3];
    uint8_t rx[3];
    bool keepReading = true;
    int *adc0 = nullptr;
    int *adc1 = nullptr;
    int fd;
};

class adcController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int adc0 MEMBER lastAdc0 NOTIFY adcChanged)
    Q_PROPERTY(int adc1 MEMBER lastAdc1 NOTIFY adcChanged)
public:
    adcController() : lastAdc0(0), lastAdc1(1){};

    // void run() override;

signals:
    // void gpioTriggered(int gpioNum);
    void adcChanged();

public slots:
    void Init();

    void ReadRaw(int channel);

    void startReading() { adcW.start(); };
    void stopReading() { adcW.readStop(); };

private:
    int lastAdc0;
    int lastAdc1;

    adcWorker adcW;

    int fd; // File descriptor
};

#endif
