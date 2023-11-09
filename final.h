#ifndef FINAL_H
#define FINAL_H

#include <QMainWindow>
#include <QListWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>

//#include <QtBluetooth/qbluetoothsocket.h>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothAddress>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/QBluetoothSocket>
#include <QtBluetooth/QBluetoothUuid>
#include <QtBluetooth/QBluetoothDeviceInfo>
#include "ui_final.h"
#include <QPermissions>


QT_BEGIN_NAMESPACE
namespace Ui { class final; }
QT_END_NAMESPACE

class final : public QMainWindow
{
    Q_OBJECT

public:
    final(QWidget *parent = nullptr);
    ~final();

private slots:
    void on_pushButton_1_clicked();
    //void on_pushButton_1_clicked();
    //void enterEvent(QEvent* event) override;
    //    void on_pushButton_2_clicked();
    //    void on_comboBox_3_currentIndexChanged(int item);
    //    //void on_comboBox_3_clicked(QCheckBox* item);
    //    //void on_ComboBox_3_IndexChanged(int index);
    void on_pushButton_12_clicked();
    void on_pushButton_13_clicked();

    //    //void on_pushButton_10_clicked();
    //    void on_checkBox_3_stateChanged();
    //    //void onaddwidget();
    //    //void onremovewidget();
    void on_pushButton_15_clicked();
    void on_pushButton_14_clicked();
    void on_comboBox_3_currentIndexChanged(int item);

    void on_pushButton_2_clicked();
    void discoverDevices();
    void deviceDiscovered(const QBluetoothDeviceInfo& device);
    void pairWithDevice(QListWidgetItem* item);
    void unpairDevice(QListWidgetItem* item);
    void connectToDevice();
    void sendMessage();
    void readMessage();
    void showContextMenu(const QPoint& pos);
    void itemclicked(QListWidgetItem* item);
    void socketConnected();
    void socketReadyRead();
    void blustop();
    void permissiongranted();

private:
    Ui::final *ui;

private:
    void displayContents(const QString& item);
    QBluetoothDeviceDiscoveryAgent* discoveryAgent;
    QBluetoothSocket* socket;
    QListWidget* deviceList;
    QTextEdit* messageDisplay;
    QLineEdit* messageInput;
    QPushButton* sendButton;
    QDialog* dialog;
    QDialog* dialog2;
    QPushButton* stop;
    QBluetoothDeviceInfo targetDevice;
};
#endif // FINAL_H
