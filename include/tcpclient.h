#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);

    void connectToServer(const QString &host, quint16 port);
    void sendMessage(const QString &message);
    bool isConnected();

signals:
    void consoleMessage(const QString &message);
    void timeoutSignal();

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onTimeout();

private:
    QTcpSocket *socket;
    QTimer timerReconnect;
    bool connected;
};

#endif // TCPCLIENT_H
