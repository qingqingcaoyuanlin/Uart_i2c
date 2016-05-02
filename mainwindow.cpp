#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(16);
    ui->tableWidget->setRowCount(16);
    ui->tableWidget->setMouseTracking(true);
    QStringList Header;
    Header<<"0"<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"A"<<"B"<<"C"<<"D"<<"E"<<"F";
    ui->tableWidget->setHorizontalHeaderLabels(Header);
    ui->tableWidget->setVerticalHeaderLabels(Header);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        ui->comboBox->addItem(info.portName());
    Header.clear();
    Header<<"9600"<<"12800"<<"115200";
    ui->comboBox_rate->addItems(Header);
    ui->pushButton->setText("打开");


    ui->tableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    popMenu = new QMenu(ui->tableWidget);
    this->action = new QAction("copy", this);
    popMenu->addAction(action);

    connect(ui->comboBox,SIGNAL(currentIndexChanged(const QString &)), this, SLOT(slot_update_serial_com(const QString &)));
    connect(ui->comboBox_rate, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(slot_serial_rate(const QString &)));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(slot_serial_openclose()));
    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(slotItemEnter(QTableWidgetItem*)));
    //connect(ui->tableWidget, SIGNAL(itemActivated(QTableWidgetItem*)), this, SLOT(slot_send_read(QTableWidgetItem*)));
    connect(ui->tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(slot_send_read(int, int)));
    connect(action, SIGNAL(triggered(bool)), this, SLOT(slot_right_click()));



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotItemEnter(QTableWidgetItem *item)
{
    qDebug()<<"slotItemEnter";
    if(!item)
        return;
    QString str = item->text();
    if (str.isEmpty())
        return;
    qDebug()<<str;
    unsigned int num = 0;
    bool ok;
    num = str.toInt(&ok, 16);
    if(ok)
    {
        qDebug()<<"number "<<num;
    }
    else
    {
        qDebug()<<"number error";
    }
}

void MainWindow::slot_update_serial_com(const QString &)
{
    qDebug()<<"click";


}

void MainWindow::slot_serial_rate(const QString &)
{

}

void MainWindow::slot_serial_openclose()
{


}

void MainWindow::slot_send_read(int x, int y)
{
    qDebug()<<"read";

}

void MainWindow::slot_right_click()
{
    qDebug()<<"right click";

}

void MainWindow::mousePressEvent(QMouseEvent * event)
{
    qDebug()<<"mouse event";

}
