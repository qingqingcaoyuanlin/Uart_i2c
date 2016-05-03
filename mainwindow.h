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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QAction *action;
    QMenu *popMenu;
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
    int xx;
    int yy;
    QSerialPort serial;
};

#endif // MAINWINDOW_H
