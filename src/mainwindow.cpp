#include "include/mainwindow.h"
#include "./ui_mainwindow.h"
#include "include/config.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (positionSource)
    {
        QObject::connect(positionSource, &QGeoPositionInfoSource::positionUpdated, this, &MainWindow::gpsHandler);
        positionSource->startUpdates();
    }

    tcpClient.connectToServer(SERVER_IP, SERVER_PORT);
    QObject::connect(&tcpClient, &TcpClient::consoleMessage, this, &MainWindow::consoleHandler);
    QObject::connect(&tcpClient, &TcpClient::timeoutSignal, this, &MainWindow::ledHandler);

    timerSending.setInterval(MSG_COOLDOWN_S*1000);
    QObject::connect(&timerSending, &QTimer::timeout, this, &MainWindow::timeoutSendingHandler);
    timerSending.start();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::gpsHandler(const QGeoPositionInfo &info)
{
    QGeoCoordinate coordinate = info.coordinate();
    lon = QString::number(coordinate.longitude());
    lat = QString::number(coordinate.latitude());
    alt = QString::number(coordinate.altitude());
    ui->lb_lon->setText(lon);
    ui->lb_lat->setText(lat);
    ui->lb_alt->setText(alt);

}

QList<QString> MainWindow::findAddr()
{
    QList<QString> ip_list;
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    for (const QNetworkInterface& interface : interfaces)
    {
        for (const QNetworkAddressEntry& entry: interface.addressEntries())
        {
            ip_list.append(entry.ip().toString());
        }
    }
    return ip_list;
}


void MainWindow::receiveHandler(const QString &msg)
{
    ui->te_console->insertPlainText("C> " + msg + "\n");
}

void MainWindow::consoleHandler(const QString &msg)
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentTimeString = currentDateTime.toString("hh:mm:ss");
    ui->te_console->insertPlainText(currentTimeString + "  " + msg + "\n");
    ui->te_console->ensureCursorVisible();
}

void MainWindow::timeoutSendingHandler()
{
    if (tcpClient.isConnected())
    {
        tcpClient.sendMessage('|'+lon+'|'+lat+'|'+alt+'|');
    }
    else
    {
        ui->te_console->insertPlainText("C> Cannot send data - Not connected\n");
    }
    timerSending.start();
}

void MainWindow::ledHandler()
{
    ui->led->setOn(tcpClient.isConnected());
}
