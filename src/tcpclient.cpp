#include "include/tcpclient.h"
#include "include/config.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent), socket(new QTcpSocket(this))
{

    connected = false;
    connect(socket, &QTcpSocket::connected, this, &TcpClient::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);

    timerReconnect.setInterval(RECONNECT_COOLDOWN_S * 1000);
    QObject::connect(&timerReconnect, &QTimer::timeout, this, &TcpClient::onTimeout);
    timerReconnect.start();
}

void TcpClient::connectToServer(const QString &host, quint16 port)
{
    socket->connectToHost(host, port);
}

void TcpClient::sendMessage(const QString &message)
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->write(message.toUtf8());
        socket->flush();
        emit consoleMessage("Attempt to send data");
    }
}

void TcpClient::onConnected()
{
    emit consoleMessage("Connected to the server!");
    connected = true;
}

void TcpClient::onDisconnected()
{
    emit consoleMessage("Disconnected from the server!");
    connected = false;
}

void TcpClient::onReadyRead()
{
    QString message = QString::fromUtf8(socket->readAll());
    emit consoleMessage("S>" + message);
}

void TcpClient::onTimeout()
{
    emit timeoutSignal();
    if (not (socket->state() == QAbstractSocket::ConnectedState))
    {
        emit consoleMessage("Trying to reconnect...");
        connectToServer(SERVER_IP, SERVER_PORT);
    }
    timerReconnect.start();
}

bool TcpClient::isConnected()
{
    return connected;
}

