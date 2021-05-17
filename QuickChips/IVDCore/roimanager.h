#ifndef ROIMANAGER_H
#define ROIMANAGER_H

#include "roisystem.h"

#include <QPixmap>
#include <QObject>
#include <QTreeWidget>

class ROIManager : public QObject
{
    Q_OBJECT
public:
    explicit ROIManager(QObject *parent = 0);
    ~ROIManager();

    static int calculateROISignalAverage( unsigned char* buffer, int width, int height, ROI* roi );
    void analyizeCurrentRoiSet( unsigned char* buffer, int width, int height, QTreeWidget* outputTree );

    ROISystem* getRoi( int roiIndex );
    ROI* getCurrentTarget();
    void addRoi( QString name );
    void addSignal( int roiIndex );
    void addBackground( int roiIndex );

    void setSelectedRoi( int roiIndex );
    void setSelectedRoiSignal( int roiIndex, int signalIndex );
    void setSelectedRoiBackground( int roiIndex, int backgroundIndex );

    void drawROIsOnPixmap( QPixmap* pixmap );

    QList<ROISystem*> roiBank;

    void clearRoiBank();

    void loadRoiBank();
    void saveRoiBank();

//private:
    int currentRoiIndex;
    int currentSignalIndex;
    int currentBackgroundIndex;

signals:

public slots:

};

#endif // ROIMANAGER_H
