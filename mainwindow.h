#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothSocket>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class LocationHandler;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*MainWindow(QWidget *parent = nullptr);
    ~MainWindow();*/
    explicit MainWindow(QWidget *parent = nullptr);


private slots:
    void on_pushButtonSearch_clicked();

    void on_pushButtonConnect_clicked();

    void on_pushButtonDisconnect_clicked();


    void on_pushButtonClear_clicked();

    void captureDeviceProperties(const QBluetoothDeviceInfo &device);

    void searchingFinished();

    void connectionEstablished();
    void connectionInterrupted();
    void socketReadyToRead();

    void on_pushButtonGo_clicked();
    void on_pushButtonStop_clicked();
    void on_pushButtonBack_clicked();
    void on_pushButtonRight_clicked();
    void on_pushButtonLeft_clicked();
    void on_pushButtonFinder_clicked();

    void on_pushButtonFollow_clicked();
    void onCoordinatesUpdated(double lat, double lng);


private:
    Ui::MainWindow *ui;

    QBluetoothDeviceDiscoveryAgent *discoveryAgent;

    void addToLogs(QString message);

    QBluetoothSocket *socket;

    void sendMessageToDevice(QString message);

};

#endif // MAINWINDOW_H

