#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QAction>
#include <QMenu>
#include <QSerialPort>
namespace Ui {
class MainWindow;
}
typedef struct Serial_Data_
{
    uchar Slave_Add;    //从机地址
    uchar Register;     //寄存器
    uchar Value;        //值
    uchar End_Flag;    //结束标志，0xFF
}Serial_Data;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QAction *action;
    QMenu *popMenu;
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


private:
    Ui::MainWindow *ui;
    unsigned char xx;
    unsigned char yy;
    QSerialPort serial;
};

#endif // MAINWINDOW_H
