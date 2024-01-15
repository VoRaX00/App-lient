#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMetaEnum>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setDeviceController();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_InIPAddress_textChanged(const QString &arg1)
{
    QString state= "0";
    if(arg1 == "..."){
        state = "";
    }else{
        QHostAddress address(arg1);
        if(QAbstractSocket::IPv4Protocol == address.protocol()){
            state="1";
        }
    }
    ui->InIPAddress->setProperty("state", state);
    style()->polish(ui->InIPAddress);

}


void MainWindow::on_btnConnect_clicked()
{
    if(controller.isConnected()){
        controller.disconnect();
    }
    else{
        QString ip = ui->InIPAddress->text();
        int port = ui->spnPort->value();
        controller.connectToDevice(ip, port);
    }
}

void MainWindow::device_connected()
{
    ui->lstConsole->addItem("Connected to device");
    ui->btnConnect->setText("Disconnect");
}

void MainWindow::device_disconnected()
{
    ui->lstConsole->addItem("Disconnected from device");
    ui->btnConnect->setText("Connect");
}

void MainWindow::device_stateChanged(QAbstractSocket::SocketState state)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketState>();
    ui->lstConsole->addItem(metaEnum.valueToKey(state));
}

void MainWindow::device_errorOccurred(QAbstractSocket::SocketError error)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketError>();
    ui->lstConsole->addItem(metaEnum.valueToKey(error));
}

void MainWindow::setDeviceController()
{
    connect(&controller, SIGNAL(connected()), this, SLOT(device_connected()));
    connect(&controller, SIGNAL(disconnected()), this, SLOT(device_disconnected()));
    connect(&controller, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(device_stateChanged(QAbstractSocket::SocketState)));
    connect(&controller, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(device_errorOccurred(QAbstractSocket::SocketError)));
    connect(&controller, SIGNAL(dataReady(QByteArray)), this, SLOT(device_dataReady(QByteArray)));
}

void MainWindow::on_btnClear_clicked()
{
    ui->lstConsole->clear();
}

void MainWindow::device_dataReady(QByteArray data)
{
    ui->lstConsole->addItem(QString(data));
}

