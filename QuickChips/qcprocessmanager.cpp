#include "qcprocessmanager.h"

#include <qthread.h>
#include <qdebug.h>



class QCProcessManagerThread : public QThread
{
public:
    QCProcessManagerThread( QCProcessManager* parent = NULL ) : QThread(parent)
    {
        pm = (QCProcessManager*)parent;
    }

    void run() Q_DECL_OVERRIDE
    {
        qDebug() << "QCProcessManagerThread starting - " << pm->processName;

        pm->issueCommand( "setHeaterDirection", 1 );
        msleep( 5000 );
        pm->issueCommand( "setHeaterDirection", 0 );
        msleep( 1000 );
        pm->issueCommand( "setLEDDirection", 1 );
        msleep( 2000 );
        pm->issueCommand( "setLEDDirection", 0 );
        msleep( 2000 );
        pm->issueCommand( "capture", 0 );

    }

private:
    QCProcessManager* pm;

};


QCProcessManager::QCProcessManager(QObject *parent) :
    QObject(parent)
{
}


void QCProcessManager::loadProcessFile( QString filename )
{

}

void QCProcessManager::setExposure( int exposureTarget )
{

}

void QCProcessManager::setTargetTemperatureCentiCel( int targetCentiCel )
{

}

void QCProcessManager::setHeatingDuration( int heatingDurationSec )
{

}

void QCProcessManager::setROIFile( QString filename )
{

}

void QCProcessManager::startProcess()
{
    processThread = new QCProcessManagerThread( this );
    processThread->start();
}

void QCProcessManager::pauseProcess()
{

}

void QCProcessManager::stopProcess()
{

}

