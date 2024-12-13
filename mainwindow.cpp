#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    //, ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(this->discoveryAgent, SIGNAL(finished()),this, SLOT(searchingFinished()));
    this->socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(this->socket, SIGNAL(connected()),this, SLOT(connectionEstablished()));
    connect(this->socket, SIGNAL(disconnected()),this, SLOT(connectionInterrupted()));
    connect(this->socket, SIGNAL(readyRead()),this, SLOT(socketReadyToRead()));

    locationHandler = new LocationHandler(this);
    connect(locationHandler, &LocationHandler::coordinatesUpdated, this, &MainWindow::onCoordinatesUpdated); }

MainWindow::~MainWindow()
{
    delete ui;
}

LocationHandler::LocationHandler(QObject *parent) : QObject(parent)

{

    source = QGeoPositionInfoSource::createDefaultSource(this);
    if (source) {
        connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)),
                this, SLOT(positionUpdated(QGeoPositionInfo)));
        source->startUpdates();
        source->setUpdateInterval(1000);

    }
}

void MainWindow::on_pushButtonSearch_clicked()
{
    this->addToLogs("Szukam");

    ui->comboBoxDevices->clear();
    ui->pushButtonSearch->setEnabled(false);
    this->discoveryAgent->start(); // rozpoczęcie wyszukiwania
    connect(this->discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(captureDeviceProperties(QBluetoothDeviceInfo)));
}


void MainWindow::on_pushButtonConnect_clicked()
{
    this->addToLogs("Inicjuję połączenie");

    QString comboBoxQString = ui->comboBoxDevices->currentText();
    QStringList portList = comboBoxQString.split(" ");
    QString deviceAddres = portList.last();

    static const QString serviceUuid(QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"));
    this->socket->connectToService(QBluetoothAddress(deviceAddres),QBluetoothUuid(serviceUuid),QIODevice::ReadWrite);
    this->addToLogs("Łączę z: " + portList.first() + " " + deviceAddres);
}


void MainWindow::on_pushButtonDisconnect_clicked()
{
    this->addToLogs("Rozłączam");
    this->socket->disconnectFromService();
}

void MainWindow::on_pushButtonClear_clicked()
{
    ui->textEditLogs->clear();
}

void MainWindow::captureDeviceProperties(const QBluetoothDeviceInfo &device) {
    ui->comboBoxDevices->addItem(device.name() + " " + device.address().toString());
    this->addToLogs("Znalazłem: " + device.name() + " " + device.address().toString());
}

void MainWindow::searchingFinished() {
    ui->pushButtonSearch->setEnabled(true);
    this->addToLogs("Wyszukiwanie zakończone");
}

void MainWindow::addToLogs(QString message)
{
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    ui->textEditLogs->append(currentDateTime + "\t" + message);
}

    void MainWindow::connectionEstablished() {
    this->addToLogs("Połączyłem się");
}

void MainWindow::connectionInterrupted() {
    this->addToLogs("Rozłączyłem się");
}


void MainWindow::socketReadyToRead() {
        this->addToLogs("Odczytuję dane");

    while(this->socket->canReadLine()) {
        QString line = this->socket->readLine();
        //qDebug() << line;

        QString terminator = "\r";
        int pos = line.lastIndexOf(terminator);
        //qDebug() << line.left(pos);

        this->addToLogs(line.left(pos));
    }
}

void MainWindow::on_pushButtonGo_clicked() {
    this->addToLogs("JADĘ DO PRZODU");
    this->sendMessageToDevice("2");
}

void MainWindow::on_pushButtonStop_clicked() {
    this->addToLogs("ZATRZYMUJĘ SIĘ");
    this->sendMessageToDevice("5");
}

void MainWindow::on_pushButtonBack_clicked()
{
    this->addToLogs("COFAM");
    this->sendMessageToDevice("8");
}


void MainWindow::on_pushButtonRight_clicked()
{
    this->addToLogs("JADĘ W PRAWO");
    this->sendMessageToDevice("6");
}


void MainWindow::on_pushButtonLeft_clicked()
{
    this->addToLogs("JADĘ W LEWO");
    this->sendMessageToDevice("4");
}


void MainWindow::on_pushButtonFinder_clicked()
{
    this->addToLogs("SZUKAM DROGI");
    this->sendMessageToDevice("0");
}

void MainWindow::sendMessageToDevice(QString message) {
    if(this->socket->isOpen() && this->socket->isWritable()) {
        this->addToLogs("Wysyłam: " + message);
        this->socket->write(message.toStdString().c_str());
    } else {
        this->addToLogs("Brak połączenia!");
    }
}

void MainWindow::onCoordinatesUpdated(double lat, double lng) {
    this->addToLogs("WYSYŁAM WSPÓŁRZEDNE: " + QString::number(lat) + ", " + QString::number(lng));
    this->sendMessageToDevice(QString::number(lat) + "," + QString::number(lng));
}



void MainWindow::on_pushButtonFollow_clicked()
{
    this->addToLogs("WYSYŁAM WSPÓŁRZEDNE");
    this->sendMessageToDevice("");
    
}


/*void LocationHandler::positionUpdated(const QGeoPositionInfo &info)
{
    if(info.isValid())
    {
        currentLocation = info.coordinate();
        
        double Poslat = currentLocation.latitude();
        QString Slat = QString::number(Poslat);

        double Poslng = currentLocation.longitude();
        QString Slng = QString::number(Poslng);

        emit coordinatesUpdated(Poslat, Poslng);

    }  
}*/
