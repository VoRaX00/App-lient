#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QStyle>
#include "devicecontroller.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_InIPAddress_textChanged(const QString &arg1);

    void on_btnConnect_clicked();

    void device_connected();
    void device_disconnected();
    void device_stateChanged(QAbstractSocket::SocketState);
    void device_errorOccurred(QAbstractSocket::SocketError);
    void on_btnClear_clicked();
    void device_dataReady(QByteArray data);

    void on_btnSend_clicked();

private:
    void setDeviceController();

private:
    Ui::MainWindow *ui;
    DeviceController controller;
};
#endif // MAINWINDOW_H
