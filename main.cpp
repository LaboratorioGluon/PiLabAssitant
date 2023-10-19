// keep existing includes here
// include qml context, required to add a context property
#include <QQmlContext>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QCursor>
#include <QtQml>

#include "globals.h"
#include "irc.h"
#include "modules/ADC/adcController.h"
#include "modules/UART/uartController.h"

gpioHandler *hGpio;
IRC *hIRC;
adcController *adc;
uartController *uart;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    

    // QQmlApplicationEngine is the preferred way to start qml projects since Qt 5.2
    QQmlApplicationEngine engine;

    //qmlRegisterSingletonType(QUrl("qrc:///Style.qml"), "App", 1, 0, "Style");

    hIRC = new IRC();
    hIRC->Init();
    //test_irc();
    QThread workerThread;
    app.setOverrideCursor( QCursor( Qt::BlankCursor ) );
    hGpio = new gpioHandler();
    hGpio->start();

    adc = new adcController();
    adc->Init();

    uart = new uartController();

    
    engine.rootContext()->setContextProperty("_gpioHandler", hGpio);
    engine.rootContext()->setContextProperty("_ircHandler", hIRC);
    engine.rootContext()->setContextProperty("_adcHandler", adc);
    engine.rootContext()->setContextProperty("_uartHandler", uart);

    engine.load(QUrl("main.qml"));
    return app.exec();
}
