#ifndef QCPROCESSMANAGER_H
#define QCPROCESSMANAGER_H

#include <QObject>

#include "processcommand.h"

class QCProcessManagerThread;

class QCProcessManager : public QObject
{
    Q_OBJECT
public:
    explicit QCProcessManager(QObject *parent = 0);

    void loadProcessFile( QString filename );
    void clearCurrentProcessCommands();

    void setExposure( int exposureTarget );
    void setTargetTemperatureCentiCel( int targetCentiCel );
    void setHeatingDuration( int heatingDurationSec );    

    void startProcess();
    void pauseProcess();
    void stopProcess();

    void executeCommand( ProcessCommand* pCommand );

    QCProcessManagerThread* processThread;

    int currentProcessCommandIndex;
    QList<ProcessCommand*> processCommands;

    QString processName;

    int targetExposure;
    int targetTempCentiCel;
    int heatingDurationSec;
    QString roiFilename;

signals:
    void issueCommand( QString command, int value );

public slots:

};

#endif // QCPROCESSMANAGER_H
