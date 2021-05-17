#include "qcprocessmanager.h"

#include <qthread.h>
#include <qdebug.h>

#include <qfile.h>


class QCProcessManagerThread : public QThread
{
public:
    bool isAlive = true;

    QCProcessManagerThread( QCProcessManager* parent = NULL ) : QThread(parent)
    {
        pm = (QCProcessManager*)parent;
    }

    void run() Q_DECL_OVERRIDE
    {
        qDebug() << "QCProcessManagerThread starting [" << pm->processName << "] length[" << pm->processCommands.length() << "]";

        while( isAlive && pm->currentProcessCommandIndex < pm->processCommands.length() )
        {
            msleep(1000);   // overall 1 second command throttle.
            ProcessCommand* nextCommand = pm->processCommands[pm->currentProcessCommandIndex];
            pm->executeCommand( nextCommand );
            pm->currentProcessCommandIndex++;
        }

        if ( isAlive )
        {
            ProcessCommand* processCompleteCommand = new ProcessCommand();
            processCompleteCommand->system = "system";
            processCompleteCommand->target = "assay_status";
            processCompleteCommand->command = "complete";
            processCompleteCommand->value = "1";
            pm->executeCommand( processCompleteCommand );
        }

        qDebug() << "QCProcessManagerThread finishing";
    }

private:
    QCProcessManager* pm;

};


QCProcessManager::QCProcessManager(QObject *parent) :
    QObject(parent)
{
    currentProcessCommandIndex = 0;
    processThread = NULL;

}


void QCProcessManager::loadProcessFile( QString filename )
{
    QFile file(filename);
    bool fileSuccess = file.open( QIODevice::ReadOnly | QIODevice::Text );

    if ( fileSuccess )
    {
        QTextStream in(&file);

        clearCurrentProcessCommands();

        while( !in.atEnd() )
        {
            QString line = in.readLine();
            QStringList fields = line.split(",");

            qDebug() << "line " << line << " field lengh" << fields.length();

            if ( fields.length() == 2 )
            {
                QString name = fields[0];
                QString value = fields[1];
                if ( name == "Assay Name" )
                {
                    processName = value;
                }
                else if ( name == "ROI Definition" )
                {
                    roiFilename = value;
                }
            }
            else if ( fields.length() == 4 )
            {
                ProcessCommand* newCommand = new ProcessCommand();
                newCommand->system = fields[0];
                newCommand->target = fields[1];
                newCommand->command = fields[2];
                newCommand->value = fields[3];
                processCommands.push_back( newCommand );
            }
            else
            {
                qDebug() << "Warning ProcessManager found line length != 4 [" << line << "]";
            }
        }
    }
    else
    {
        qDebug() << "ERROR cannot open process file [" << filename << "]";
    }

    file.close();
}

void QCProcessManager::clearCurrentProcessCommands()
{
    foreach ( ProcessCommand* command, processCommands )
    {
        delete command;
    }

    processCommands.clear();
    currentProcessCommandIndex = 0;
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

void QCProcessManager::startProcess()
{
    currentProcessCommandIndex = 0;
    processThread = new QCProcessManagerThread( this );
    processThread->start();
}

void QCProcessManager::pauseProcess()
{

}

void QCProcessManager::stopProcess()
{
    if ( processThread != NULL )
    {
        processThread->wait(1000);
        processThread->isAlive = false;
        processThread = NULL;
    }
}


void QCProcessManager::executeCommand( ProcessCommand* pCommand )
{
    QString system = pCommand->system;
    QString target = pCommand->target;
    QString command = pCommand->command;
    int value = pCommand->value.toInt();

    qDebug() << "executeCommand " << system << "," << target << "," << command << "," << value;

    if ( system == "system" )
    {
        if ( target == "system" )
        {
            if ( command == "wait" )
            {
                issueCommand( "wait", value );  // this is just to update the ui
                QThread::msleep( value );
            }
        }
        else if ( target == "assay_status" )
        {
            if ( command == "complete" )
            {
                issueCommand( "assay_complete", 1 );
            }
        }
    }
    else if ( system == "heater" )
    {
        if ( target == "heater1" )
        {
            if ( command == "setDirection" )
            {
                issueCommand( "setHeaterDirection", value );
            }
            else if ( command == "setHeaterTarget" )
            {
                issueCommand( "setHeaterTarget", value );
            }
        }
    }
    else if ( system == "illumination" )
    {
        if ( target == "illumination1" )
        {
            if ( command == "setDirection" )
            {
                issueCommand( "setLEDDirection", value );
            }
        }
    }
    else if ( system == "camera" )
    {
        if ( target == "camera" )
        {
            if ( command == "setExposure" )
            {
                issueCommand( "setExposure", value );
            }
            else if ( command == "captureDark" )
            {
                issueCommand( "captureDark", 0 );
            }
            else if ( command == "capture" )
            {
                issueCommand( "capture", 0 );
            }

        }
    }

}
