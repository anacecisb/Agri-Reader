#include "btmanager.h"

//Q_LOGGING_CATEGORY( QT_BT_DBG, "btdbg" );

static const QLatin1String serviceUuid("80b79d0c-2dbd-11e6-b67b-9e71128cae77");

BTManager::BTManager(QObject *parent) :
    QObject(parent)
{
    rfcommServer = NULL;
//    QLoggingCategory::CategoryFilter
}

BTManager::~BTManager()
{
    stopServer();
}

//void BTManager::initAndBroadcast( const QBluetoothAddress &localAdapter )
//{
//    btServer = new QBluetoothServer( QBluetoothServiceInfo::RfcommProtocol, this );
//    connect( btServer, SIGNAL(newConnection()), this, SLOT(clientConnected()));
//    bool success = btServer->listen(localAdapter);
//    if ( !success )
//    {
//        qWarning() << "Cannot connect to BT local adapter" << localAdapter.toString();
//        return;
//    }

//    QBluetoothServiceInfo::Sequence classId;
//}


void BTManager::startServer()
{
    if (rfcommServer)
        return;

    QBluetoothLocalDevice* localDevice = new QBluetoothLocalDevice();
    if ( localDevice->isValid() )
    {
        qDebug() << "BTManager: SUCCESS - Got a local Bluetooth device";
    }
    else
    {
        qDebug() << "BTManager: FAILED to get a local BT device";
    }

    localDevice->powerOn();
    QString deviceName = localDevice->name();

    localDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable);

    qDebug() << "BTManager Local device name:" << deviceName;


//    QList<QBluetoothHostInfo> hosts = QBluetoothLocalDevice::allDevices();
//    if ( hosts.length() <= 0 )
//    {
//        qCritical() << "QBluetoothLocalDevice::allDevices return empty list!";
//        return;
//    }

//    QBluetoothHostInfo hostInfo = hosts.first();
//    QBluetoothAddress localAdapter = hostInfo.address();

    //*** Listen for pairing requests
//    connect( localDevice, SIGNAL(pairingConfirmation(bool)), this, SLOT(pairingConfirmation(bool)));
    connect( localDevice, SIGNAL(pairingDisplayPinCode(const QBluetoothAddress&, QString)), this, SLOT(pairingDisplayPinCode(const QBluetoothAddress&, QString)));
    connect( localDevice, SIGNAL(pairingDisplayConfirmation(const QBluetoothAddress&, QString)), this, SLOT(pairingDisplayConfirmation(const QBluetoothAddress&, QString)));
    connect( localDevice, SIGNAL(pairingFinished(const QBluetoothAddress&, QBluetoothLocalDevice::Pairing)), this, SLOT(pairingFinished(const QBluetoothAddress&, QBluetoothLocalDevice::Pairing)));
    connect( localDevice, SIGNAL(error(QBluetoothLocalDevice::Error)), this, SLOT(error(QBluetoothLocalDevice::Error)));

    //*** Listen for connections ***
    QBluetoothAddress localAdapter = localDevice->address();

    rfcommServer = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
//    rfcommServer->setSecurityFlags(QBluetooth::NoSecurity);
    connect(rfcommServer, SIGNAL(newConnection()), this, SLOT(clientConnected()));
    connect(rfcommServer, SIGNAL(error(QBluetoothServer::Error)), this, SLOT(serverError(QBluetoothServer::Error)));
    bool result = rfcommServer->listen(localAdapter);
    if (!result)
    {
        qWarning() << "Cannot connect BTManager server to" << localAdapter.toString();
        return;
    }


    //serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceRecordHandle, (uint)0x00010010);

    // *** Bluetooth service definition ***

    QBluetoothServiceInfo::Sequence classId;

    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList, classId);

    classId.prepend(QVariant::fromValue(QBluetoothUuid(serviceUuid)));

    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);
    serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList,classId);

    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, tr("QC BT Dev 1"));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription,
                             tr("QhickChips Dev 1 bluetooth connection"));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider, tr("ivdvision.com"));

    // Register UUID
    serviceInfo.setServiceUuid(QBluetoothUuid(serviceUuid));

    // Register for PublicBrowseGroup
    QBluetoothServiceInfo::Sequence publicBrowse;
    publicBrowse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList,publicBrowse);

    // Define Protocols
    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    // Android does not support L2cap
//    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::L2cap));
//    protocolDescriptorList.append(QVariant::fromValue(protocol));
//    protocol.clear();
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
             << QVariant::fromValue(quint8(rfcommServer->serverPort()));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList,
                             protocolDescriptorList);

    serviceInfo.registerService(localAdapter);

}

void BTManager::stopServer()
{
    // Unregister service
    serviceInfo.unregisterService();

    // Close sockets
    qDeleteAll(clientSockets);

    // Close server
    delete rfcommServer;
    rfcommServer = 0;
}

void BTManager::sendMessage(const QString &message)
{
    qDebug() << "BTManager::sendMessage:" << message;
    QByteArray text = message.toUtf8();

    foreach (QBluetoothSocket *socket, clientSockets)
        socket->write(text);
}


void BTManager::serverError(QBluetoothServer::Error error)
{
    qDebug() << "BTManager::serverError";
}

void BTManager::clientConnected()
{
    qDebug() << "BTManager::clientConnected";
    QBluetoothSocket *socket = rfcommServer->nextPendingConnection();
    if (!socket)
    {
        qDebug() << "BTManager::clientConnected ERROR No Socket!";
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
    clientSockets.append(socket);
    emit clientConnected(socket->peerName());
}

void BTManager::clientDisconnected()
{
    qDebug() << "BTManager::clientDisconnected";
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;

    emit clientDisconnected(socket->peerName());

    clientSockets.removeOne(socket);

    socket->deleteLater();
}

void BTManager::readSocket()
{
//    qDebug() << "BTManager::readSocket enter";

    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
    {
        qDebug() << "BTManager::readSocket ERROR no socket!";
        return;
    }

    QByteArray data = socket->readAll();
//    qDebug() << "read socket data:" << data;
    QString message = QString::fromUtf8( data.constData(), data.length() );
//    qDebug() << "BTManager::readSocket message received:" << message;

//    message = "BTServer Received ECHO:" + message;
//    sendMessage( message );

//    while ( socket->canReadLine() )
//    {
//        QByteArray line = socket->readLine().trimmed();
//        QString message = QString::fromUtf8(line.constData(), line.length());
      emit messageReceived(socket->peerName(), message );

//        qDebug() << "BTManager::readSocket message received:" << message;

//        message = "BTServer Received ECHO:" + message;
//        sendMessage( message );
//    }
}


void BTManager::pairingConfirmation(bool confirmation)
{
    qDebug() << "BTManager::pairingRequested";
}

void BTManager::pairingDisplayPinCode(const QBluetoothAddress &address, QString pin)
{
    qDebug() << "BTManager::pairingDisplayPinCode";
}

void BTManager::pairingDisplayConfirmation(const QBluetoothAddress &address, QString pin)
{
    qDebug() << "BTManager::pairingDisplayConfirmation";
}

void BTManager::pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing)
{
    qDebug() << "BTManager::pairingFinished";
}

void BTManager::error(QBluetoothLocalDevice::Error error)
{
    qDebug() << "BTManager::error";
}


void BTManager::debugA()
{

}

