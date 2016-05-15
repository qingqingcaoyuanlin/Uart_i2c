#include "serialthread.h"
#include <QDebug>


void SerialThread::run()
{
    while(serial_port.isOpen())
    {
        if(serial_port.waitForReadyRead(TimeOut))
        {
            Data_Rx = serial_port.readAll();
            qDebug()<<"get "<<Data_Rx;
            EmitSignal();
            usleep(100);
            /*
            while(serial_port.waitForReadyRead(10))
            {
                qDebug()<<"emit";
                Data_Rx += serial_port.readAll();

            }
            */
        }

    }



}


void SerialThread::SetPortName(QString &name)
{
    PortName = name;

}

void SerialThread::SetBaudRate(QString &baud)
{
    BaudRate = baud;
}

void SerialThread::EmitSignal()
{
    emit signal_read();
}
