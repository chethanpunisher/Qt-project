#include "final.h"
#include "ui_final.h"
#include <qfiledialog.h>

QBluetoothDeviceInfo deviceInfo; // Create a QBluetoothDeviceInfo object
QBluetoothDeviceInfo& temp = deviceInfo;

QString tm = "";
QString validate = "Excel";

final::final(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::final)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_4->setCurrentIndex(0);
    // ui.comboBox_3->addItem("");
    ui->comboBox_16->addItem("Excel");
    ui->comboBox_16->addItem("CSV");
    ui->comboBox_16->addItem("Q-DAS");
    ui->comboBox_16->addItem("STAT-EXPRESS");
    ui->comboBox_16->addItem("Custom");

    QObject::connect(ui->comboBox_16, qOverload<int>(&QComboBox::activated), this, &final::on_comboBox_3_currentIndexChanged);

}

final::~final()
{
    delete ui;
}


void final::on_pushButton_1_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void final::on_comboBox_3_currentIndexChanged(int item)
{
    ui->stackedWidget_4->setCurrentIndex(1);


    if (item == 0) {

        ui->stackedWidget_4->setCurrentIndex(0);
        //ui.label_8->setText("excel");
        validate = "Excel";
    }
    else if (item == 1) {

        ui->stackedWidget_4->setCurrentIndex(1);
        // ui.label_8->setText("CSV");
        validate = "CSV";
    }
    else if (item == 2) {

        ui->stackedWidget_4->setCurrentIndex(2);
        // ui.label_8->setText("Q-DAS");
        validate = "Q-DAS";

    }
    else if (item == 3) {

        ui->stackedWidget_4->setCurrentIndex(3);
        //ui.label_8->setText("STAT-EXPRESS");
        validate = "STAT-EXPRESS";
    }
    else if (item == 4) {

        ui->stackedWidget_4->setCurrentIndex(4);
        // ui.label_8->setText("Custom");
        validate = "Custom";
    }
}

//void final::on_checkBox_3_stateChanged()
//{
//    if (ui.checkBox_3->isChecked())
//    {
//        // QMessageBox::information(this, "", "enable auto connect mode all COM port it can affect other software requiring COM port");
//        // QMessageBox::information(this, "enable auto connect mode all COM port it can affect other software requiring COM port");
//        QPixmap iconPixmap("C:\\Users\\cheth\\OneDrive\\Desktop\\icons\\warning_changed.png");
//        ui.label_9->setPixmap(iconPixmap);

//        ui.label_10->setText("Enable auto connect mode all COM port it can affect other software requiring COM port");
//    }
//    else
//    {
//        ui.label_9->clear();
//        ui.label_10->clear();
//    }

//}
//ui.stackedWidget->setCurrentIndex(1);




void final::on_pushButton_2_clicked()
{
    //qApp->requestPermission(QBluetoothPermission{}, context, &socketConnected);
    qApp->requestPermission(QBluetoothPermission{}, this, &final::permissiongranted);


}

void final::permissiongranted(){
    dialog = new QDialog();
    dialog->setMinimumHeight(450);
    dialog->setMinimumWidth(300);
    dialog->setWindowTitle("Press send button on instrument");


    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);

    deviceList = new QListWidget(this);
    connect(deviceList, &QListWidget::itemClicked, this, &final::itemclicked);
    deviceList->setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(deviceList, &QListWidget::customContextMenuRequested,
    //    this, &MainWindow::showContextMenu);

    // messageDisplay = new QTextEdit(this);
    //messageDisplay->setReadOnly(true);

    messageInput = new QLineEdit(this);

    sendButton = new QPushButton("Send", this);
    connect(sendButton, &QPushButton::clicked, this, &final::sendMessage);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(deviceList);
    //layout->addWidget(messageDisplay);
    layout->addWidget(messageInput);
    layout->addWidget(sendButton);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &final::deviceDiscovered);
    //connect(socket, &QBluetoothSocket::readyRead, this, &MainWindow::readMessage);
    connect(socket, &QBluetoothSocket::connected,
            this, &final::socketConnected);
    connect(socket, &QBluetoothSocket::readyRead,
            this, &final::socketReadyRead);



    dialog->setLayout(layout);

    dialog->show();
    discoveryAgent->start();
}

void final::on_pushButton_12_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

}

void final::on_pushButton_13_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

}

void final::on_pushButton_14_clicked()
{
    ui->label_8->setText(validate);
    ui->stackedWidget->setCurrentIndex(0);

}

void final::discoverDevices() {
    deviceList->clear();
    discoveryAgent->start();
}

void final::deviceDiscovered(const QBluetoothDeviceInfo& device) {
    QListWidgetItem* item = new QListWidgetItem(device.name());
    item->setData(Qt::UserRole, device.address().toString());
    deviceList->addItem(item);
    temp = device;
    if (device.name() == "") {
        targetDevice = temp;
        discoveryAgent->stop();

        // Use the UUID for the Serial Port service
        QBluetoothUuid serialPortServiceUuid = QBluetoothUuid("{00001101-0000-1000-8000-00805F9B34FB}");

        socket->connectToService(QBluetoothAddress(targetDevice.address()), serialPortServiceUuid);
    }
}

void final::pairWithDevice(QListWidgetItem* item) {
    QString macAddress = item->data(Qt::UserRole).toString();
    QBluetoothAddress address(macAddress);

    QBluetoothLocalDevice localDevice;
    QBluetoothLocalDevice::Pairing pairingStatus = localDevice.pairingStatus(address);

    if (pairingStatus != QBluetoothLocalDevice::Paired) {
        localDevice.requestPairing(address, QBluetoothLocalDevice::Paired);
    }
}

void final::unpairDevice(QListWidgetItem* item) {
    QString macAddress = item->data(Qt::UserRole).toString();
    QBluetoothAddress address(macAddress);

    QBluetoothLocalDevice localDevice;
    if (localDevice.pairingStatus(address) == QBluetoothLocalDevice::Paired) {
        localDevice.requestPairing(address, QBluetoothLocalDevice::Unpaired);
    }
}

void final::connectToDevice() {
    QListWidgetItem* selectedItem = deviceList->currentItem();
    if (selectedItem) {
        QString macAddress = selectedItem->data(Qt::UserRole).toString();
        QBluetoothAddress address(macAddress);

        QBluetoothLocalDevice localDevice;
        if (localDevice.pairingStatus(address) == QBluetoothLocalDevice::Paired) {
            QBluetoothUuid uuid = QBluetoothUuid(QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"));
            socket->connectToService(address, uuid);
        }
    }
}

void final::sendMessage() {
    QString message = messageInput->text();
    socket->write(message.toUtf8());
    //socket->write("");
    messageInput->clear();
}

void final::readMessage() {
    QByteArray data = socket->readAll();
    messageDisplay->append(QString(data));
}

void final::showContextMenu(const QPoint& pos) {
    QListWidgetItem* selectedItem = deviceList->itemAt(pos);
    if (selectedItem) {
        QMenu contextMenu(this);
        QAction* unpairAction = contextMenu.addAction("Unpair");
        connect(unpairAction, &QAction::triggered, this, [this, selectedItem]() {
            unpairDevice(selectedItem);
        });
        contextMenu.exec(deviceList->mapToGlobal(pos));
    }
}

void final::itemclicked(QListWidgetItem* item) {
    qDebug() << "Discovered device:" << item->text();
    QString macAddress = item->data(Qt::UserRole).toString();
    QBluetoothAddress address(macAddress);
    QBluetoothLocalDevice localDevice;
    QBluetoothUuid serialPortServiceUuid = QBluetoothUuid("{00001101-0000-1000-8000-00805F9B34FB}");

    socket->connectToService(QBluetoothAddress(macAddress), serialPortServiceUuid);
    if (localDevice.pairingStatus(address) == QBluetoothLocalDevice::Paired) {

    }

    dialog2 = new QDialog();
    QVBoxLayout* layout = new QVBoxLayout;
    stop = new QPushButton("stop");
    connect(stop, &QPushButton::clicked, this, &final::blustop);
    messageDisplay = new QTextEdit(this);
    messageDisplay->setReadOnly(true);
    layout->addWidget(messageDisplay);
    layout->addWidget(stop);
    dialog2->setMinimumHeight(450);
    dialog2->setMinimumWidth(300);
    dialog2->setWindowTitle("Output");
    dialog2->setLayout(layout);
    dialog2->show();
    dialog->close();
}

void final::socketConnected() {
    qDebug() << "Connected to device:" << targetDevice.name();
    socket->write("Hello from desktop!");
}

void final::socketReadyRead() {
    QByteArray data = socket->readAll();
    //QByteArray data;
    //QDataStream datastream(data);
    //int i = 0;
    //datastream >> i;
    qint16 i;
    if (data.size() >= 2) {
        i = static_cast<qint16>(
            (static_cast<quint8>(data.at(0)) << 8) |
            static_cast<quint8>(data.at(1))
            );
        qDebug() << "Received 16-bit value:" << i;
    }

    qDebug() << "Received data:" << i;
    messageDisplay->setText(QString::number(i));
}

void final::on_pushButton_15_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, "open a file", QDir::homePath());

}

void final::blustop()
{
    discoveryAgent->stop();
    dialog2->close();
    socket->disconnectFromService();
}

