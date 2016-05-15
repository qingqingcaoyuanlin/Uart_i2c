#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QAction>
#include <QMenu>
#include <QSerialPort>
#include "serialthread.h"
namespace Ui {
class MainWindow;
}
#define send_buff_len 256
#define recv_buff_len 256
typedef struct Serial_Data_
{
    uchar Slave_Add;    //从机地址
    uchar Register;     //寄存器
    uchar Value;        //值
    uchar End_Flag;    //结束标志，0xFF
}Serial_Data;
typedef struct Serial_Send_List_
{
    int send_deal_pos;
    int send_add_pos;
    Serial_Data send_buff[send_buff_len];

}Serial_Send_List;
typedef struct Serial_Recv_List_
{
    int recv_deal_pos;
    int recv_add_pos;
    Serial_Data recv_buff[recv_buff_len];

}Serial_Recv_List;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QAction *action;
    QMenu *popMenu;
    SerialThread ser_Thread;
    void Serial_Send(Serial_Data *serialData);
protected slots:
    void slot_update_serial_com(const QString &text);
    void slot_serial_rate(const QString &text);
    void slot_serial_openclose();
   // void slot_send_read(QTableWidgetItem *item);
    void slot_send_read(int x, int y);
    void slotItemEnter(QTableWidgetItem *item);
    void slot_right_click();
    void slot_read();
    void mousePressEvent(QMouseEvent * event);
    void slot_Checkbox();
    void slot_button_write();
    void slot_serial_read();
    void slot_show_recv_data();
signals:
    void testsignal();
private:
    Ui::MainWindow *ui;
    unsigned char xx;
    unsigned char yy;
    uchar check_value;
    //QSerialPort serial;

};

#endif // MAINWINDOW_H
