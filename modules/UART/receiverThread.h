#ifndef RECEIVERTHREAD_H
#define RECEIVERTHREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>


class ReceiverThread : public QThread
{
    Q_OBJECT

public:
    explicit ReceiverThread(QObject *parent = nullptr);
    ~ReceiverThread();

    void startReceiver(const QString &portName);

signals:
    void newDataInSerial(const QString &s);

private:
    void run() override;

    int uart0_fd;

    QString m_portName;
    QString m_response;
    int m_waitTimeout = 0;
    QMutex m_mutex;
    bool m_quit = false;
};


#endif // RECEIVERTHREAD_H