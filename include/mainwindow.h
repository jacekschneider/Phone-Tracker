#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QNetworkInterface>
#include <QDateTime>
#include "tcpclient.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void gpsHandler(const QGeoPositionInfo&);
    void receiveHandler(const QString&);
    void consoleHandler(const QString&);


private slots:
    void timeoutSendingHandler();
    void ledHandler();
private:
    Ui::MainWindow *ui;
    QGeoPositionInfoSource *positionSource = QGeoPositionInfoSource::createDefaultSource(nullptr);
    QList<QString> findAddr ();
    QTimer timerSending;

    TcpClient tcpClient;



    QString lat;
    QString lon;
    QString alt;

};
#endif // MAINWINDOW_H
