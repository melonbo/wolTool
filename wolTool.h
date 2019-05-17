#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostInfo>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void StringToHex(QString str, QByteArray &senddata);
    char ConvertHexChar(char ch);
    void readConfigFile();

private slots:
    void on_pushButton_send_clicked();


    void on_pushButton_send_2_clicked();

private:
    Ui::MainWindow *ui;
    QString mac;
    QString ip;
    QString domain;
    QString netmask;
    int port;
    QString configFileName;
    QSettings *configFile;

    void getIPAddress(QString domain, int ip);
};

#endif // MAINWINDOW_H
