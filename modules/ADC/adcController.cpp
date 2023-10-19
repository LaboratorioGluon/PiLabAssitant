#include <linux/spi/spidev.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <fcntl.h>
#include <stdio.h>

#include "adcController.h"



void adcController::Init()
{
    int ret;
    const char *dev = "/dev/spidev1.0";
    uint8_t mode=0b00;

    fd = open(dev, O_RDWR);
    if( fd <0 ){
            printf("ERROR! opening ADC fd\n");
            return;
    }
    
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);

    connect(&adcW, SIGNAL(adcChanged()), this, SIGNAL(adcChanged()));
    adcW.setAdc0(&lastAdc0);
    adcW.setAdc1(&lastAdc1);
    adcW.setFd(&fd);
}


void adcController::ReadRaw(int channel)
{
    uint8_t tx[] = {
        0x18 | channel << 2,
        0,
        0
    };
    uint8_t rx[3] = {};
    struct spi_ioc_transfer tr = {
            .tx_buf = (unsigned long) tx,
            .rx_buf = (unsigned long) rx,
            .len = 3,
            .speed_hz = 500000,
            .delay_usecs = 100,
            .bits_per_word = 8
    };

    int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);

    if(ret < 1)
    {
        printf("Error ADC reading\n");
        return;
    }
    
    uint32_t data = (rx[2]>> 4) + (rx[1]<<4);
    if (channel == 0)
    {
        lastAdc0 = data;
    }else{
        lastAdc1 = data;
    }
    emit adcChanged();
}


