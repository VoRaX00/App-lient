#include "devicecontroller.h"

DeviceController::DeviceController(QObject *parent)
    : QObject{parent}
{
    connect(&socket, &QTcpSocket::connected, this, &DeviceController::connected);
    connect(&socket, &QTcpSocket::disconnected, this, &DeviceController::disconnected);
    connect(&socket, &QTcpSocket::stateChanged, this, &DeviceController::socket_stateChanged);
    connect(&socket, &QTcpSocket::errorOccurred, this, &DeviceController::errorOccurred);
    connect(&socket, SIGNAL(readyRead()), this, SLOT(socket_readyRead()));
}

void DeviceController::connectToDevice(QString _ip, int _port)
{
    if(socket.isOpen()){
        if(ip == _ip && port == _port){
            return;
        }
        socket.close();
    }
    ip = _ip;
    port = _port;
    socket.connectToHost(ip, port);
}

void DeviceController::disconnect()
{
    socket.close();
}

QAbstractSocket::SocketState DeviceController::state()
{
    return socket.state();
}

bool DeviceController::isConnected()
{
    return socket.state() == QAbstractSocket::ConnectedState;
}

void DeviceController::send(QString message)
{
    socket.write(message.toUtf8());
}

void DeviceController::socket_stateChanged(QAbstractSocket::SocketState state)
{
    if(state == QAbstractSocket::UnconnectedState){
        socket.close();
    }
    emit stateChanged(state);
}

void DeviceController::socket_readyRead()
{
    auto data = socket.readAll();
    emit dataReady(data);
}
