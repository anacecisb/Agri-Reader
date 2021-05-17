#ifndef KL46MANAGER_H
#define KL46MANAGER_H

#include <QObject>

#include <QtSerialPort/QtSerialPort>


class KL46ReaderThread;
class KL46WriterThread;
class KL46QueryThread;
class KL46System;

class KL46Manager : public QObject
{
    Q_OBJECT

    friend class KL46ReaderThread;
    friend class KL46WriterThread;
    friend class KL46QueryThread;

public:
    explicit KL46Manager(QObject *parent = 0);
    ~KL46Manager();

    void findAndConnect();

    void setSystemDirection( int system, int direction );
    void setSystemP1( int systemID, int value );
    int getSystemP1( int systemID );

    void putCommandInQueue( QString command );
    void debugA();
    void debugB();

    bool isConnected();

    bool verboseDebug;

    int getSystemState( int systemID );
    int getSystemFeedback( int systemID );



protected:
    KL46ReaderThread* readerThread;
    KL46WriterThread* writerThread;
    KL46QueryThread* queryThread;
    QSerialPort* serialPort;

    QList<QString> messageBufferOut;
    QList<QString> messageBufferIn;
    QByteArray inBuffer;

    int maxSystemID;
    QList<KL46System*> systemBank;

private:

    //QStringBuilder inBuffer;      // briefly thought about using string builder for optimization.

    bool _isConnected;

signals:
    void systemUpdated();

public slots:

};

#endif // KL46MANAGER_H
