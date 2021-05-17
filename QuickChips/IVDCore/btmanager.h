#ifndef BTMANAGER_H
#define BTMANAGER_H

#include <QObject>
#include <QBluetoothServer>
#include <QBluetoothServiceInfo>
#include <QBluetoothSocket>
#include <QBluetoothHostInfo>
#include <QBluetoothLocalDevice>

class BTManager : public QObject
{
    Q_OBJECT
public:
    explicit BTManager(QObject *parent = 0);
    ~BTManager();

    void startServer();
    void stopServer();

    void debugA();

    void sendMessage( const QString &message );

private:
    QBluetoothServer* rfcommServer;
    QBluetoothServiceInfo serviceInfo;
    QList<QBluetoothSocket*> clientSockets;


signals:
    void messageReceived( const QString &sender, const QString &message );
    void clientConnected(const QString &name);
    void clientDisconnected(const QString &name);


public slots:

private slots:
    void serverError(QBluetoothServer::Error error);
    void clientConnected();
    void clientDisconnected();
    void readSocket();

    void pairingConfirmation(bool confirmation);
    void pairingDisplayPinCode(const QBluetoothAddress &address, QString pin);
    void pairingDisplayConfirmation(const QBluetoothAddress &address, QString pin);
    void pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing);
    void error(QBluetoothLocalDevice::Error error);
};

#endif // BTMANAGER_H
