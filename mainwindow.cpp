#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QRegExp>
#include <QValidator>
#include <QTableWidgetItem>
#include <QList>
#include "serialthread.h"
#include <QObject>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    check_value = 0x00;
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(16);
    ui->tableWidget->setRowCount(16);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableWidget->setMouseTracking(true);
    QStringList Header;
    Header<<"0"<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"A"<<"B"<<"C"<<"D"<<"E"<<"F";
    ui->tableWidget->setHorizontalHeaderLabels(Header);
    ui->tableWidget->setVerticalHeaderLabels(Header);
    ui->tableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    action = new QAction("read", this);
    ui->tableWidget->addAction(action);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        ui->comboBox->addItem(info.portName());
    ui->comboBox->setCurrentIndex(0);
    Header.clear();
    Header<<"9600"<<"12800"<<"115200";
    ui->comboBox_rate->addItems(Header);
    ui->comboBox_rate->setCurrentIndex(0);

    ui->pushButton->setText("打开");

    //ui->lineEdit_Slave->inputMask();
    //ui->lineEdit_Slave->setInputMask("hh");
    //ui->lineEdit_Slave->home(true);
    QRegExp regx("[a-fA-F0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, ui->lineEdit_Slave);
    ui->lineEdit_Slave->setValidator(validator);
    ui->lineEdit_Slave->setMaxLength(2);            //限制输入格式

    //ui->comboBox->installEventFilter(this);


    connect(action, SIGNAL(triggered()), this,SLOT(slot_read()));

    //connect(ui->comboBox,SIGNAL(currentIndexChanged(const QString &)), this, SLOT(slot_update_serial_com(const QString &)));
    //connect(ui->comboBox,SIGNAL(highlighted(int index)), this, SLOT(slot_update_serial_com(const QString &)));
    //connect(ui->comboBox_rate, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(slot_serial_rate(const QString &)));

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

    connect(&ser_Thread, SIGNAL(signal_read()), this, SLOT(slot_show_recv_data()));

}

MainWindow::~MainWindow()
{
    if(ser_Thread.serial_port.isOpen())
        ser_Thread.serial_port.close();
    delete ui;
}

void MainWindow::slotItemEnter(QTableWidgetItem *item)
{
    Serial_Data send_data;
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
        send_data.End_Flag = 0xFF;
        send_data.Register = (xx<<4 |yy);
        send_data.Value = (uchar)num;
        if(ui->lineEdit_Slave->text() == NULL)
        {
            send_data.Slave_Add = 0x00;
        }
        else
        {
            send_data.Slave_Add = (uchar)ui->lineEdit_Slave->text().toInt();
        }
        Serial_Send(&send_data);
    }
    else
    {
        qDebug()<<"number error";
    }
}

void MainWindow::slot_update_serial_com(const QString &)
{
    qDebug()<<"click";
    ser_Thread.serial_port.setPortName(ui->comboBox->currentText());
    //foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        //ui->comboBox->addItem(info.portName());

}

void MainWindow::slot_serial_rate(const QString &)
{


    ser_Thread.serial_port.setBaudRate(ui->comboBox_rate->currentText().toInt());
}

void MainWindow::slot_serial_openclose()
{
    if(ser_Thread.serial_port.isOpen())
    {
        ui->pushButton->setText("打开");
        ser_Thread.serial_port.close();
        qDebug()<<"close";

    }
    else
    {
        if(ui->comboBox->currentText()!= NULL)
        {
            ser_Thread.serial_port.setBaudRate(ui->comboBox_rate->currentText().toInt());
            ser_Thread.serial_port.setPortName(ui->comboBox->currentText());

            if(ser_Thread.serial_port.open(QIODevice::ReadWrite))
            {
               ui->pushButton->setText("关闭");
               ser_Thread.start();
               qDebug()<<"open success";

            }
            else
            {
                ui->pushButton->setText("打开");
                qDebug()<<"fail";
            }
        }
        else
        {
            qDebug()<<"please set serial";
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
    ser_Thread.serial_port.write((char*)serialData, sizeof(Serial_Data));
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
#if 1
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
        serialData.Slave_Add = (uchar)(ui->lineEdit_Slave->text().toInt() | 0x01);
    }
    Serial_Send(&serialData);
#else

    Serial_Data serialData;
    int i =0;
    QList<QTableWidgetItem *> items = ui->tableWidget->selectedItems();
    qDebug()<<"List "<<items.count();
    if(!items.isEmpty())
    {
        i = items.length();
        foreach (QTableWidgetItem * item, items) {
            serialData.Register = (uchar)(item->column()<<4) | (uchar)(item->row());
            serialData.End_Flag = 0xFF;
            serialData.Value = 0;
            qDebug()<<"col"<<(item->column()<<4)<<"row"<<(item->row());
            if(ui->lineEdit_Slave->text() == NULL)
            {
                serialData.Slave_Add = 0x01;
            }
            else
            {
                serialData.Slave_Add = (uchar)ui->lineEdit_Slave->text().toInt();
            }

        }
    }
    else
    {
        qDebug()<<"0";
    }
#endif
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
    if(ser_Thread.serial_port.isOpen())
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

void MainWindow::slot_serial_read()
{
    static qint16 number = 0;
}

void MainWindow::slot_show_recv_data()
{
    Serial_Data *recv_data;
    QString str;
    uchar array[4] = {0};
    int i = 0;
    int j = 0;

    //while(ser_Thread.Data_Rx.length() > 0)
    {

        qDebug()<<ser_Thread.Data_Rx.length();
        for(j=0; j<(volatile)ser_Thread.Data_Rx.length()/4; j++)
        {
            for(i=0; i<4; i++)
            {
                array[i] = ser_Thread.Data_Rx.at(i+4*j);
            }
            recv_data = (Serial_Data *)(&array);
            if(recv_data->Slave_Add & 0x01 == 0x01 && recv_data->End_Flag == 0xFF)
            {
               // ui->tableWidget->setCurrentCell(recv_data->Register&0xF0, recv_data->Register&0x0F);
               // ui->tableWidget->cu
                //sprintf(str, "%02X", recv_data->Value);
                //str = QString().arg(recv_data->Value);
                str = QString::number(recv_data->Value,16);
                qDebug()<<str;


                qDebug()<<"row"<<((recv_data->Register&0xF0)>>4);
                qDebug()<<"line"<<(recv_data->Register&0x0F);
                disconnect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(slotItemEnter(QTableWidgetItem*)));
                ui->tableWidget->setItem(((recv_data->Register&0xF0)>>4), recv_data->Register&0x0F, new QTableWidgetItem(str));
                connect(ui->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(slotItemEnter(QTableWidgetItem*)));
           }
        }
        ser_Thread.Data_Rx.clear();
    }
}

