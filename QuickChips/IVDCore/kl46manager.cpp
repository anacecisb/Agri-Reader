#include "kl46manager.h"

#include <QtSerialPort/QtSerialPort>

#include "kl46system.h"

#include "QCTimingMaster.h"

#define VERBOSE_DEBUG 0
#define VERBOSE_WRITER 0
#define VERBOSE_READER 0

QMutex serialConnectionMutex;
QMutex systemUpdateMutex;

//************ Worker Thread Definitions **************
class KL46ReaderThread : public QThread
{
public:
    KL46ReaderThread( KL46Manager* parent = NULL ) : QThread(parent)
    {
        kl46 = (KL46Manager*)parent;
    }

    void run() Q_DECL_OVERRIDE
    {
        int id = rand();

        bool verboseRead = VERBOSE_READER;


        if (verboseRead) qDebug() << "KL46ReaderThread Enter" << id;

        serialConnectionMutex.lock();

        if ( kl46->isConnected() && kl46->serialPort->waitForReadyRead(QCTM_KL46_WAITFORREADYREAD) )
        {
            kl46->inBuffer += kl46->serialPort->readAll();

//            bool doWhileThing = false;
//            if ( doWhileThing )
//            {
//                while( kl46->serialPort->waitForReadyRead( 10 ) )
//                {
//                   postBuffer += kl46->serialPort->readAll();
//                }
//            }



            while ( kl46->inBuffer.contains("\r\n\u0003") )   // end of message character
            {

                int indexOfEOM = kl46->inBuffer.indexOf( "\r\n\u0003" );
                QString message = kl46->inBuffer.left(indexOfEOM);
                kl46->messageBufferIn.push_back( message );

                int remainingLength = (kl46->inBuffer.length() - indexOfEOM) -1;
                kl46->inBuffer = kl46->inBuffer.right( remainingLength );

//                qDebug() << "message in:" << message << " indexOfEOM:" << indexOfEOM;
            }


            bool systemWasUpdated = false;
            int indexOfFB1 = -1;
            int indexOfP1 = -1;

            while ( kl46->messageBufferIn.length() > 0 )
            {
                QString nextMessage = kl46->messageBufferIn.front();

//                qDebug() << "nextMessage" << nextMessage;

                kl46->messageBufferIn.removeFirst();
                QStringList lines = nextMessage.split('\n');
                foreach ( QString line, lines )
                {
                    if (verboseRead) qDebug() << "Line:" << line;

                    QStringList items = line.split(',');
                    if ( items.length() > 2 )
                    {
                        int systemID = items[0].toInt();

                        if ( (systemID > 0) && (systemID < kl46->maxSystemID) )
                        {

                            if ( line.contains(';' ) )
                            {   // header line
                                for ( int i = 0; i < items.count(); i++ )
                                {
                                    QString item = items[i];
                                    if ( item == "FB1" )
                                    {
//                                        qDebug() << "setting index fb1 for " << systemID;
                                        indexOfFB1 = i;
                                    }
                                    else if ( item == "P1" )
                                    {
                                        indexOfP1 = i;
                                    }
                                }
                            }
                            else
                            {
                                int state = items[1].toInt();

                                systemUpdateMutex.lock();
                                if ( indexOfFB1 > 0 && indexOfFB1 < items.count() )
                                {
                                    int value = items[indexOfFB1].toInt();
                                    kl46->systemBank[systemID]->fb1 = value;
//                                    qDebug() << "setting fb1! " << systemID << " " << value;

                                    indexOfFB1 = -1;
                                }
                                else if ( indexOfP1 > 0 && indexOfP1 < items.count() )
                                {
                                    int value = items[indexOfP1].toInt();
                                    kl46->systemBank[systemID]->p1 = value;

                                    indexOfP1 = -1;
                                }


                                kl46->systemBank[systemID]->state = state;
                                systemUpdateMutex.unlock();

                                if (verboseRead) qDebug() << "systemID:" << systemID << " state:" << state << " raw:" << items.at(1);

                                systemWasUpdated = true;
                            }
                        }
                    }
                }
            }

            if ( systemWasUpdated )
                emit( kl46->systemUpdated() );
        }


//        if (verboseRead) qDebug() << "KL46ReaderThread Exit & mutex release" << id << " " << kl46->serialPort->errorString();

        serialConnectionMutex.unlock();
    }

private:
    KL46Manager* kl46;

};


class KL46WriterThread : public QThread
{
public:
    KL46WriterThread( KL46Manager* parent = NULL ) : QThread(parent)
    {
        kl46 = (KL46Manager*)parent;
    }

    void run() Q_DECL_OVERRIDE
    {
        bool verboseWriter = VERBOSE_WRITER;

//        if (verboseWriter) qDebug() << "KL46WriterThread Enter";

        serialConnectionMutex.lock();

//        if (verboseWriter) qDebug() << "KL46WriterThread has lock";

        int bytesToWrite = kl46->serialPort->bytesToWrite();
//        qDebug() << "bytes to write pre:" << bytesToWrite;

        msleep( QCTM_KL46_WRITING_PRESLEEP );

        if ( kl46->messageBufferOut.count() > 0 && kl46->isConnected() )
        {
            QString commandString = kl46->messageBufferOut.front();
            int writeResult = kl46->serialPort->write( commandString.toStdString().c_str() );
            kl46->serialPort->flush();
            kl46->messageBufferOut.pop_front();
            if (verboseWriter) qDebug() << "KL46WriterThread writeResult[" << writeResult << "] cmd:" << commandString;
        }

        msleep( QCTM_KL46_WRITING_POSTSLEEP );

        bytesToWrite = kl46->serialPort->bytesToWrite();
//        qDebug() << "bytes to write post:" << bytesToWrite;

        serialConnectionMutex.unlock();

//        if (verboseWriter) qDebug() << "KL46WriterThread releasing lock";



        kl46->readerThread->run();
    }

private:
    KL46Manager* kl46;

};


class KL46QueryThread : public QThread
{
public:
    KL46QueryThread( KL46Manager* parent = NULL ) : QThread(parent)
    {
        kl46 = (KL46Manager*)parent;
        isAlive = true;

        kl46->serialPort = new QSerialPort();
        kl46->serialPort->setBaudRate( 230400 );
    }

    ~KL46QueryThread()
    {
        delete kl46->serialPort;
    }

    void findAndConnect()
    {
        qDebug() << "KL46Manager KL46QueryThread - findAndConnect";

        foreach ( const QSerialPortInfo &portInfo, QSerialPortInfo::availablePorts() )
        {
            /* kl 46 serial port info from debuging
             * Name "ttyACM0"
                desc "FSL CDC DEVICE"
                vid 1240
                pid 10 */
            qDebug() << "Name" << portInfo.portName();
            qDebug() << "desc" << portInfo.description();
            qDebug() << "v" << portInfo.vendorIdentifier();
            qDebug() << "p" << portInfo.productIdentifier();

            bool isUSBKL = portInfo.vendorIdentifier() == 1240 && portInfo.productIdentifier() == 10;
            bool isEmbeddedKL = portInfo.portName().contains("ttymxc1");
            if ( isEmbeddedKL || isUSBKL )
            {
                qDebug() << "KL46Manager - Found KL46, attempting to connect...";
                kl46->serialPort->setPort( portInfo );
                if ( kl46->serialPort->open( QIODevice::ReadWrite ) )
                {
                    qDebug() << "KL46Manager - Serial port successfully opened";
                    kl46->_isConnected = true;

                    kl46->setSystemDirection( 6, 0 );

                    kl46->writerThread->run();
                }
            }
        }
    }


    void run() Q_DECL_OVERRIDE
    {

        while ( isAlive )
        {
            if ( !kl46->_isConnected )
            {
                findAndConnect();
                if ( !kl46->_isConnected )
                    msleep(3000);
            }
            else
            {
                int heaterSystem = 8;
                QString commandString = QString( "%1,sysQ\r").arg( QString::number(heaterSystem) );
                kl46->putCommandInQueue( commandString );
                msleep( QCTM_KL46_QUERYTICKRATE );

                int LEDSystem = 10;
                commandString = QString( "%1,sysQ\r").arg( QString::number(LEDSystem) );
                kl46->putCommandInQueue( commandString );
                msleep( QCTM_KL46_QUERYTICKRATE );
            }
        }
    }

    bool isAlive;

private:
    KL46Manager* kl46;

};
//******************************************************


KL46Manager::KL46Manager(QObject *parent) :
    QObject(parent)
{
    _isConnected = false;

    maxSystemID = 15;
    for ( int i = 0; i < maxSystemID; i++ )
    {
        KL46System* system = new KL46System();
        system->id = i;
        systemBank.push_back( system );
    }

    // **** DEBUG disable KL comms for now
//    return;
    // **** END DEBUG *****

    writerThread = new KL46WriterThread( this );
    readerThread = new KL46ReaderThread( this );

    queryThread = new KL46QueryThread( this );    
    queryThread->start();

    verboseDebug = VERBOSE_DEBUG;
}

KL46Manager::~KL46Manager()
{
    setSystemDirection( 10, 0 );
    setSystemDirection( 8, 0 );
    // TODO: delete worker thread and stuff.....
}


void KL46Manager::findAndConnect()
{
    foreach ( const QSerialPortInfo &portInfo, QSerialPortInfo::availablePorts() )
    {
        /* kl 46 serial port info from debuging
         * Name "ttyACM0"
            desc "FSL CDC DEVICE"
            vid 1240
            pid 10 */
//        qDebug() << "Name" << info.portName();
//        qDebug() << "desc" << info.description();
//        qDebug() << "v" << info.vendorIdentifier();
//        qDebug() << "p" << info.productIdentifier();

        if ( portInfo.vendorIdentifier() == 1240 && portInfo.productIdentifier() == 10 )
        {
            qDebug() << "KL46Manager - Found KL46, attempting to connect...";
            serialPort->setPort( portInfo );
            if ( serialPort->open( QIODevice::ReadWrite ) )
            {
                qDebug() << "KL46Manager - Serial port successfully opened";
                _isConnected = true;

                setSystemDirection( 6, 0 );

                queryThread->start();
            }
        }
    }
}


void KL46Manager::setSystemDirection( int system, int direction )
{
    QString commandString = QString( "%1,setDir,%2\r").arg( QString::number(system), QString::number(direction) );
    putCommandInQueue( commandString );
}


void KL46Manager::setSystemP1( int systemID, int value )
{
    QString commandString = QString( "%1,setP1,%2\r").arg( QString::number(systemID), QString::number(value) );
    putCommandInQueue( commandString );
}

int KL46Manager::getSystemP1( int systemID )
{
    return systemBank[systemID]->p1;
}

void KL46Manager::putCommandInQueue( QString command )
{
    if (verboseDebug) qDebug() << "KL46Manager putting command in messageBufferOut :" << command ;
    messageBufferOut.push_back( command );

    writerThread->run();

//    readerThread->run();
    // wake up writer thread maybe? for now we're just gonna have it tick at 50ms
}


bool KL46Manager::isConnected()
{
    return _isConnected;
}


int KL46Manager::getSystemState(int systemID)
{
    int state = -1;
    if ( systemID > 0 && systemID < maxSystemID )
        state = systemBank[systemID]->state;
    return state;
}

int KL46Manager::getSystemFeedback(int systemID)
{
    int feedback = std::numeric_limits<int>::max();
    if ( systemID > 0 && systemID < maxSystemID )
        feedback = systemBank[systemID]->fb1;
    return feedback;
}

void KL46Manager::debugA()
{
    qDebug() << "debugA";
    int direction = 0;
    systemUpdateMutex.lock();
    direction = systemBank[10]->state ? 0 : 1;
    systemUpdateMutex.unlock();
    setSystemDirection( 10, direction );

    qDebug() << direction;
}

void KL46Manager::debugB()
{
    setSystemDirection( 10, 0 );
}

