#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
class SerialThread:public QThread
{
public:
    SerialThread();
    ~SerialThread();
protected:
    void run();
private:
    QWaitCondition ser_cond;
    QMutex ser_mutex;

};

#endif // SERIALTHREAD_H
