#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QRegExp>
#include <QValidator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    check_value = 0x00;
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

    //ui->lineEdit_Slave->inputMask();
    //ui->lineEdit_Slave->setInputMask("hh");
    //ui->lineEdit_Slave->home(true);
    QRegExp regx("[a-fA-F0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, ui->lineEdit_Slave);
    ui->lineEdit_Slave->setValidator(validator);
    ui->lineEdit_Slave->setMaxLength(2);            //限制输入格式

    ui->comboBox->installEventFilter(this);
    action = new QAction("read", this);
    ui->tableWidget->addAction(action);
    ui->tableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(action, SIGNAL(triggered()), this,SLOT(slot_read()));

    connect(ui->comboBox,SIGNAL(currentIndexChanged(const QString &)), this, SLOT(slot_update_serial_com(const QString &)));
    //connect(ui->comboBox,SIGNAL(highlighted(int index)), this, SLOT(slot_update_serial_com(const QString &)));
    connect(ui->comboBox_rate, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(slot_serial_rate(const QString &)));

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(slot_serial_openclose()));
    connect(ui->pushButton_Write, SIGNAL(clicked()), this, SLOT(slot_button_write()));
    connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(slotItemEnter(QTableWidgetItem*)));
    //connect(ui->tableWidget, SIGNAL(itemActivated(QTableWidgetItem*)), this, SLOT(slot_send_read(QTableWidgetItem*)));
    connect(ui->tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(slot_send_read(int, int)));
    connect(ui->checkBox_0, SIGNAL(toggled(bool)), this, SLOT(slot_Checkbox()));

    connect(ui->checkBox_1, SIGNAL(toggled(bool)), this, SLOT(slot_Checkbox()));
    connect(ui->checkBox_2, SIGNAL(toggled(bool)), this, SLOT(slot_Checkbox()));
    connect(ui->checkBox_3, SIGNAL(toggled(bool)), this, SLOT(slot_Checkbox()));
    connect(ui->checkBox_4, SIGNAL(toggled(bool)), this, SLOT(slot_Checkbox()));
    connect(ui->checkBox_5, SIGNAL(toggled(bool)), this, SLOT(slot_Checkbox()));
    connect(ui->checkBox_6, SIGNAL(toggled(bool)), this, SLOT(slot_Checkbox()));
    connect(ui->checkBox_7, SIGNAL(toggled(bool)), this, SLOT(slot_Checkbox()));

}

MainWindow::~MainWindow()
{
    if(serial.isOpen())
        serial.close();
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
    serial.setPortName(ui->comboBox->currentText());
    //foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        //ui->comboBox->addItem(info.portName());

}

void MainWindow::slot_serial_rate(const QString &)
{


    serial.setBaudRate(ui->comboBox_rate->currentText().toInt());
}

void MainWindow::slot_serial_openclose()
{
    if(serial.isOpen())
    {
        ui->pushButton->setText("打开");
        serial.close();
        qDebug()<<"close";

    }
    else
    {

        if(serial.open(QIODevice::ReadWrite))
        {
           ui->pushButton->setText("关闭");
           qDebug()<<"open success";

        }
        else
        {
            ui->pushButton->setText("打开");
            qDebug()<<"fail";
        }

    }

}

void MainWindow::slot_send_read(int x, int y)
{
    //qDebug()<<"read"<<x<<y;
    xx = (unsigned char)x;
    yy = (unsigned char)y;
    int k = x<<4 | y;
    //qDebug()<<"k "<<k;

}

void MainWindow::slot_right_click()
{
    qDebug()<<"right click";

}

void MainWindow::mousePressEvent(QMouseEvent * event)
{
    qDebug()<<"mouse event";

}

void MainWindow::Serial_Send(Serial_Data *serialData)
{
    serial.write((char*)serialData, sizeof(Serial_Data));
    //qDebug()<<"serial send";

}

void read()
{


}

void write()
{

}
void MainWindow::slot_read()
{
    qDebug()<<"read register"<<xx<<yy;
    uchar Register = xx<<4 |yy;
    Serial_Data serialData;
    serialData.Register = Register;
    serialData.Value = 0;
    serialData.End_Flag = 0xFF;
    if(ui->lineEdit_Slave->text()==NULL)
    {
        serialData.Slave_Add = 0x01;
    }
    else
    {
        serialData.Slave_Add = (uchar)ui->lineEdit_Slave->text().toInt();
    }
    Serial_Send(&serialData);

}

void MainWindow::slot_Checkbox()
{
    if(ui->checkBox_0->isChecked())
        check_value |= 1<<0;
    else
        check_value &= ~(1<<0);

    if(ui->checkBox_1->isChecked())
        check_value |= 1<<1;
    else
        check_value &= ~(1<<1);

    if(ui->checkBox_2->isChecked())
        check_value |= 1<<2;
    else
        check_value &= ~(1<<2);

    if(ui->checkBox_3->isChecked())
        check_value |= 1<<3;
    else
        check_value &= ~(1<<3);

    if(ui->checkBox_4->isChecked())
        check_value |= 1<<4;
    else
        check_value &= ~(1<<4);

    if(ui->checkBox_5->isChecked())
        check_value |= 1<<5;
    else
        check_value &= ~(1<<5);

    if(ui->checkBox_6->isChecked())
        check_value |= 1<<6;
    else
        check_value &= ~(1<<6);

    if(ui->checkBox_7->isChecked())
        check_value |= 1<<7;
    else
        check_value &= ~(1<<7);

    qDebug()<<"check"<<check_value;
    QString s = QString::number(check_value);
    ui->lineEdit_Value->setText(s);
}
void MainWindow::slot_button_write()
{
    if(serial.isOpen())
    {
        uchar Register = xx<<4 |yy;
        Serial_Data serialData;
        serialData.Register = Register;
        serialData.Value = check_value;
        serialData.End_Flag = 0xFF;
        if(ui->lineEdit_Slave->text()==NULL)
        {
            serialData.Slave_Add = 0x00;
        }
        else
        {
            serialData.Slave_Add = (uchar)ui->lineEdit_Slave->text().toInt();
        }
        Serial_Send(&serialData);
    }
    else
    {

    }

}
