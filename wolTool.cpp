#include "wolTool.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QUdpSocket>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setWindowTitle(tr("wolTool"));
    this->setWindowIcon(QIcon(":/gu.jpg"));
    qDebug() << this->windowTitle();

    readConfigFile();
    ui->setupUi(this);
    ui->lineEdit_mac->setInputMask("HH:HH:HH:HH:HH:HH");
    ui->lineEdit_mac->setText(mac);
    ui->lineEdit_doman->setText(domain);
    ui->lineEdit_ip->setText(ip);
    ui->lineEdit_port->setText(QString::number(port));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_send_clicked()
{

    mac = ui->lineEdit_mac->text();
    domain = ui->lineEdit_doman->text();

    QHostInfo info;
    if(domain.isEmpty() == false)
         info = QHostInfo::fromName(domain);

    if(info.addresses().isEmpty() == false)
    {
        ip = info.addresses().first().toString();
        ui->lineEdit_ip->setText(ip);
    }
    else
    {
        qDebug()<<"domain canot parse";
    }

    ip = ui->lineEdit_ip->text();
//    ip = "172.16.42.199";

    port = ui->lineEdit_port->text().toInt();

    qDebug() << "mac is " << mac;
    qDebug() << "domain is " << domain;
    qDebug() << "ip address is " << ip;
    qDebug() << "port is " << port;


    QString data;
    data.append("FFFFFFFFFFFF");

    mac.remove(':');
    for(int i=0;i<16;i++)
        data.append(mac);
    qDebug() << data;

    QByteArray senddata;
    StringToHex(data,senddata);

    qDebug() << senddata;

    QUdpSocket mSocket;
    int ret;

    if(info.addresses().isEmpty() == false)
        ret = mSocket.writeDatagram(senddata, QHostAddress(ip), port);
    else
        ret = mSocket.writeDatagram(senddata, QHostAddress::Broadcast, port);

    if(ret)
    {
        QString info;
        info = QString("send a magic packet to %1:%2 with %3 bytes via UDP ").arg(ip.toLatin1().data()).arg(port).arg(ret);
        info.append('\n');
//        info.append(data);
        qDebug() << "send magic packet to" << ip.toLatin1().data() << ":" << port << ret << "bytes via UDP";
        QMessageBox::about(NULL, NULL, info);
    }
    else
    {

    }

}

void MainWindow::StringToHex(QString str, QByteArray &senddata)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}

char MainWindow::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
            return ch-0x30;
        else if((ch >= 'A') && (ch <= 'F'))
            return ch-'A'+10;
        else if((ch >= 'a') && (ch <= 'f'))
            return ch-'a'+10;
        else return (-1);
}

void MainWindow::readConfigFile()
{
    configFile = new QSettings("config.ini", QSettings::IniFormat);
    qDebug() << configFile->fileName();
    mac = configFile->value("/sys/mac").toString();
    domain = configFile->value("/sys/domain").toString();
    ip = configFile->value("/sys/ip").toString();
    port = configFile->value("/sys/port").toInt();
}

void MainWindow::on_pushButton_send_2_clicked()
{
    exit(0);
}
