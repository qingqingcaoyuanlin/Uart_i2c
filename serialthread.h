#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QSerialPort>
#include <stdio.h>
#include <QtGlobal>
#define TimeOut 200

class SerialThread:public QThread
{
    Q_OBJECT
public:

    //SerialThread(QObject *parent = 0);

    QByteArray Data_Rx;
    QByteArray Data_Tx;

    QString PortName;
    QString BaudRate;
    QSerialPort serial_port;
    void SetPortName(QString &name);
    void SetBaudRate(QString &baud);
    void EmitSignal();
protected:
    void run();
signals:
    void signal_read(void);

};

#endif // SERIALTHREAD_H
