#ifndef QCPROCESSMANAGER_H
#define QCPROCESSMANAGER_H

#include <QObject>

class QCProcessManagerThread;

class QCProcessManager : public QObject
{
    Q_OBJECT
public:
    explicit QCProcessManager(QObject *parent = 0);

    void loadProcessFile( QString filename );

    void setExposure( int exposureTarget );
    void setTargetTemperatureCentiCel( int targetCentiCel );
    void setHeatingDuration( int heatingDurationSec );
    void setROIFile( QString filename );

    void startProcess();
    void pauseProcess();
    void stopProcess();

    QCProcessManagerThread* processThread;

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
