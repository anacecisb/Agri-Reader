#include "roimanager.h"

#include <QFile>
#include <QPainter>
#include <QDebug>

ROIManager::ROIManager(QObject *parent) :
    QObject(parent)
{
    currentRoiIndex = -1;
    currentSignalIndex = -1;
    currentBackgroundIndex = -1;

    loadRoiBank();

    if ( roiBank.count() == 0 )
    {
            ROI* testROI = new ROI();
            testROI->center = QPoint( 100, 100 );
            testROI->radius = 10;

            ROI* back = new ROI();
            back->center = QPoint( 150, 150 );
            back->radius = 20;

            ROI* back2 = new ROI();
            back2->center = QPoint( 175, 175 );
            back2->radius = 20;


            ROISystem* testSystem = new ROISystem();
            testSystem->name = "Test ROI #1";
            roiBank.push_back( testSystem );

            testSystem->signalROIs.push_back( testROI );
            testSystem->backgroundROIs.push_back( back );
            testSystem->backgroundROIs.push_back( back2 );


            testROI = new ROI();
            testROI->center = QPoint( 150, 100 );
            testROI->radius = 15;

            back = new ROI();
            back->center = QPoint( 250, 150 );
            back->radius = 25;

            back2 = new ROI();
            back2->center = QPoint( 350, 175 );
            back2->radius = 10;


            testSystem = new ROISystem();
            testSystem->name = "Test ROI #2";
            roiBank.push_back( testSystem );

            testSystem->signalROIs.push_back( testROI );
            testSystem->backgroundROIs.push_back( back );
    }

}

ROIManager::~ROIManager()
{
    clearRoiBank();
}

int ROIManager::calculateROISignalAverage(unsigned char *buffer, int width, int height, ROI* roi)
{
    int result = -1;
    if ( width < 0 || height < 0 ) return result;

    long workingValue = 0;
    long numPixelsForValue = 0;

    int debugValue = 0;

    for ( int x = 0; x < width; x++ )
    {
        for ( int y = 0; y < height; y++ )
        {
            QPoint point = QPoint(x, y);
            QPoint delta = roi->center - point;
            int length = sqrt(pow(delta.x(), 2) + pow(delta.y(),2));
            if ( length <= roi->radius )
            {
                int indexByte1 = ( x * 2 ) + ( y * width * 2 );
                int indexByte2 = indexByte1 + 1;
                uchar byte1 = (uchar)buffer[indexByte1];
                uchar byte2 = (uchar)buffer[indexByte2];
                ushort byte1Short = (ushort) byte1;
                ushort byte2Short = (ushort) byte2;
                byte2Short = byte2Short << 8;
                int value = byte1Short + byte2Short;

                workingValue += value;
                numPixelsForValue++;
    //            qDebug() << valueChar;
//                rowString += QString("|%1").arg(value);

                debugValue = value;

//                qDebug() << "index" << indexByte1 << ", " << indexByte2;
//                qDebug() << "xy:" << x << y;
//                qDebug() << "b1:" << byte1Short << " b2:" << byte2Short;


            }
        }
    }

    qDebug() << "ROICalcReport| workingValue:" << workingValue << " numPixels:" << numPixelsForValue << " debugValue:" << debugValue;

    long resultLong = workingValue / numPixelsForValue;
    result = (int)resultLong;

    return result;
}


void ROIManager::analyizeCurrentRoiSet( unsigned char* buffer, int width, int height, QTreeWidget* outputTree )
{
    outputTree->clear();
    qDebug() << "*** Analyize Begin ***";
    foreach( ROISystem* system, roiBank )
    {
        int iSystem = 0;
        qDebug() << "System: " << system->name;
        QTreeWidgetItem* roiSystemItem = new QTreeWidgetItem( outputTree );
        roiSystemItem->setText( 0, QString("[ROI %2] %1").arg(system->name).arg(iSystem+1) );
//        ui->treeWidgetROIs->addTopLevelItem( roiSystemItem );

        long signalWorking = 0;
        long numSignals = system->signalROIs.count();
        long backgroundWorking = 0;
        long numBackgrounds = system->backgroundROIs.count();

        int iSignal = 0;
        foreach( ROI* signal, system->signalROIs )
        {
            int value = ROIManager::calculateROISignalAverage( buffer, width, height, signal );
            signalWorking += value;
            qDebug() << "[" << iSignal << "] signal: " << value;

            iSignal++;
        }

        long signalResult = signalWorking / numSignals;
        QTreeWidgetItem* signalItem = new QTreeWidgetItem( roiSystemItem );
        signalItem->setText( 0, QString("[%1] signal [%2]").arg(iSignal+1).arg(signalResult) );

        int iBackground = 0;
        foreach( ROI* background, system->backgroundROIs )
        {
            int value = ROIManager::calculateROISignalAverage( buffer, width, height, background );
            backgroundWorking += value;
            qDebug() << "[" << iBackground << "] background: " << value;
            iBackground++;
        }
        long backgroundResult = backgroundWorking / numBackgrounds;
        QTreeWidgetItem* backgroundItem = new QTreeWidgetItem( roiSystemItem );
        backgroundItem->setText( 0, QString("[%1] background [%2]").arg(iBackground+1).arg(backgroundResult) );

        long signalDeltaValue = signalResult - backgroundResult;
        QTreeWidgetItem* signalDelta = new QTreeWidgetItem( roiSystemItem );
        signalDelta->setText( 0, QString("[%1] signalDelta [%2]").arg(iSignal+1).arg(signalDeltaValue) );


        iSystem++;
    }

    qDebug() << "*** Analyize End ***";
}


ROISystem* ROIManager::getRoi(int roiIndex)
{
    if ( roiIndex >= 0 && roiIndex < roiBank.count() )
    {
        return roiBank[roiIndex];
    }

    return NULL;
}

ROI* ROIManager::getCurrentTarget()
{
    ROI* ret = NULL;
    ROISystem* system = getRoi( currentRoiIndex );
    if ( system != NULL )
    {
        if ( currentSignalIndex >= 0 )
            ret = system->signalROIs[currentSignalIndex];
        else if ( currentBackgroundIndex >= 0 )
            ret = system->backgroundROIs[currentBackgroundIndex];
    }

    return ret;
}


void ROIManager::addRoi( QString name )
{
    ROISystem* newSystem = new ROISystem();
    newSystem->name = name;
    roiBank.push_back( newSystem );
}


void ROIManager::addSignal( int roiIndex )
{
    ROISystem* system = getRoi( roiIndex );
    if ( system != NULL )
    {
        ROI* newRoi = new ROI();
        newRoi->center = QPoint( 100, 100 );
        newRoi->radius = 10;

        system->signalROIs.push_back( newRoi );
    }
}

void ROIManager::addBackground( int roiIndex )
{
    ROISystem* system = getRoi( roiIndex );
    if ( system != NULL )
    {
        ROI* newRoi = new ROI();
        newRoi->center = QPoint( 150, 150 );
        newRoi->radius = 15;

        system->backgroundROIs.push_back( newRoi );
    }
}


void ROIManager::setSelectedRoi( int roiIndex )
{
    currentRoiIndex = roiIndex;
    currentSignalIndex = -1;
    currentBackgroundIndex = -1;
}

void ROIManager::setSelectedRoiSignal( int roiIndex, int signalIndex )
{
    currentRoiIndex = roiIndex;
    currentSignalIndex = signalIndex;
    currentBackgroundIndex = -1;
}

void ROIManager::setSelectedRoiBackground( int roiIndex, int backgroundIndex )
{
    currentRoiIndex = roiIndex;
    currentBackgroundIndex = backgroundIndex;
    currentSignalIndex = -1;
}

void ROIManager::drawROIsOnPixmap(QPixmap *pixmap)
{
    QPainter roiPainter( pixmap );

    for ( int iSystem = 0; iSystem < roiBank.count(); iSystem++ )
    {
        bool systemIsSelected = currentRoiIndex == iSystem;
        ROISystem* system = roiBank[iSystem];


        for ( int iSignal = 0; iSignal < system->signalROIs.count(); iSignal++ )
        {
            bool signalIsSelected = currentSignalIndex == iSignal;
            if ( systemIsSelected && signalIsSelected )
                roiPainter.setPen( Qt::blue );
            else
                roiPainter.setPen( Qt::darkGreen );

            ROI* signalROI = system->signalROIs[iSignal];
            roiPainter.drawEllipse( signalROI->center, signalROI->radius, signalROI->radius );
        }


        for ( int iBackground = 0; iBackground < system->backgroundROIs.count(); iBackground++ )
        {
            bool backgroundIsSelected = currentBackgroundIndex == iBackground;
            if ( systemIsSelected && backgroundIsSelected )
                roiPainter.setPen( Qt::blue );
            else
                roiPainter.setPen( Qt::darkYellow );

            ROI* backgroundROI = system->backgroundROIs[iBackground];
            roiPainter.drawEllipse( backgroundROI->center, backgroundROI->radius, backgroundROI->radius );
        }
    }

    roiPainter.end();
}


void ROIManager::clearRoiBank()
{
    foreach( ROISystem* system, roiBank )
    {
        foreach( ROI* roi, system->signalROIs )
            delete roi;
        foreach( ROI* roi, system->backgroundROIs )
            delete roi;

        delete system;
    }
    roiBank.clear();
}

void ROIManager::loadRoiBank()
{

    QFile file("roibank.txt");
    bool fileSuccess = file.open( QIODevice::ReadOnly | QIODevice::Text );
    if ( fileSuccess )
    {
        QTextStream in(&file);

        clearRoiBank();

        ROISystem* system = NULL;
        while( !in.atEnd() )
        {
            QString line = in.readLine();

            if ( line.contains("ROI" ) )
            {
                system = new ROISystem();
                int indexOfBracket = line.indexOf("]");
                int lengthOfName = (line.length()) - (indexOfBracket+1);
                QString name = line.mid( indexOfBracket+1, lengthOfName ); // trim the newline
                system->name = name;
                roiBank.push_back( system );
            }
            else if ( (system != NULL) && (line.contains("signal") || line.contains("background")) )
            {
                QStringList lines = line.split(",");
                ROI* roi = new ROI();
                if ( line.contains("signal" ) )
                    system->signalROIs.push_back( roi );
                else
                    system->backgroundROIs.push_back( roi );

                QPoint center = QPoint( 0,0 );
                for ( int i = 0; i < lines.count(); i++ )
                {
                    if ( i == 1 )
                        center.setX( lines[i].toInt() );
                    if ( i == 2 )
                        center.setY( lines[i].toInt() );
                    if ( i == 3 )
                        roi->radius = lines[i].toInt();
                }
                roi->center = center;
            }
        }
    }
    file.close();
}




void ROIManager::saveRoiBank()
{
    QFile::remove( "roibank.txt" );
    QFile file( "roibank.txt" );
    if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
        return;

    file.flush();
    QTextStream out(&file);

    for( int iSystem = 0; iSystem < roiBank.count(); iSystem++ )
    {
        ROISystem* system = roiBank[iSystem];
        QString systemString = QString("[ROI %2]%1").arg(system->name).arg(iSystem+1);
        out << systemString << "\n";

        for ( int iSignal = 0; iSignal < system->signalROIs.count(); iSignal++ )
        {
            ROI* signal = system->signalROIs[iSignal];
            out << "\t[" << iSignal + 1 << "] signal," << signal->center.x() << "," << signal->center.y() << "," << signal->radius << "\n";
        }

        for ( int iBackground = 0; iBackground < system->backgroundROIs.count(); iBackground++ )
        {
            ROI* background = system->backgroundROIs[iBackground];
            out << "\t[" << iBackground + 1 << "] background," << background->center.x() << "," << background->center.y() << "," << background->radius << "\n";
        }
    }

    file.close();
}
