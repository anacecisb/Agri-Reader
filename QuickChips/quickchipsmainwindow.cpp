#include "quickchipsmainwindow.h"
#include "ui_quickchipsmainwindow.h"
#include "QCTimingMaster.h"
#include "qtteardropwindow.h"

#include <QtWidgets>

#include <linux/apm_bios.h>

extern "C" {
#include <apm.h>
}

//#include "IVDCore/IVDCamera.h"



//class UpdateThread : public QThread
//{
//    void run()
//    {
//        while( updateThreadShouldRun )
//        videoStream->capFrame();
//        if ( videoStream->qImage.isNull() )
//        {
//            qDebug("videoStream->qImage.isNull()");
//        }
//        else
//        {
//    //        qDebug("I CAN HAS?");
//            QGraphicsScene* scene = new QGraphicsScene();
//    //        QPixmap pixmap = videoStream->qImage;
//    //        pixmap.setSize( ui->graphicsView->size().width(), ui->graphicsView->size().width() );

//            scene->addPixmap( videoStream->qImage );
//            ui->graphicsView->setScene( scene );
//        }
//    }
//};




QuickChipsMainWindow::QuickChipsMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QuickChipsMainWindow)
{
    doSaveNextImage = false;
    doSaveNextDarkImage = false;

    frameUpdateCounter = 0;


    darkFrameBuffer = NULL;

    loadGlobalImageNumber();

    ui->setupUi(this);
    this->setWindowIconText( "IVD" );

    ui->groupBox_2->setVisible(false);
    ui->groupBox_4->setVisible(false);

    debugFlagA = 0;

    captureConsecutiveErrorCount = 11;
    emptyFrameCount = 0;

//    camera = new IVDCamera( (ApplicationWindow*)this );

//    ui->buttonCapture->setVisible( false );
//    ui->buttonQuit->setVisible( false );

//    ui->buttonReconnect->setVisible( false );

    ui->graphicsView->setAttribute(Qt::WA_TransparentForMouseEvents);

    ui->graphicsView->scale( -1, -1 );

    connect( ui->buttonCapture, SIGNAL(clicked()), this, SLOT(buttonCaptureClicked()) );
    connect( ui->buttonCaptureDark, SIGNAL(clicked()), this, SLOT(buttonCaptureDarkClicked()) );
    connect( ui->buttonClearDark, SIGNAL(clicked()), this, SLOT(buttonClearDarkClicked()) );
    connect( ui->buttonDebugA, SIGNAL(clicked()), this, SLOT(buttonDebugAClicked()) );
    connect( ui->buttonDebugB, SIGNAL(clicked()), this, SLOT(buttonDebugBClicked()) );
    connect( ui->buttonBTSend, SIGNAL(clicked()), this, SLOT(buttonBTSendClicked()) );
//    connect( ui->buttonQuit, SIGNAL(clicked()), this, SLOT(buttonQuitClicked()) );

//    connect( ui->buttonReconnect, SIGNAL(clicked()), this, SLOT(buttonReconnectClicked()) );

    connect( ui->buttonExposureSet, SIGNAL(clicked()), SLOT(setExposureFromTextBox()) );
    connect( ui->buttonExposureGet, SIGNAL(clicked()), SLOT(getAndUpdateCameraExposureField()) );

    connect( ui->buttonCameraTickRateGet, SIGNAL(clicked()), SLOT(updateCameraTickRateField()) );
    connect( ui->buttonCameraTickRateSet, SIGNAL(clicked()), SLOT(setCameraTickRateFromTextBox()) );

    connect( ui->buttonLED, SIGNAL(clicked()), SLOT(buttonLEDClicked()) );
    connect( ui->buttonHeater, SIGNAL(clicked()), SLOT(buttonHeaterClicked()) );

    connect( ui->buttonGetHeaterTarget, SIGNAL(clicked()), SLOT(buttonGetHeaterTargetClicked()) );
    connect( ui->buttonSetHeaterTarget, SIGNAL(clicked()), SLOT(buttonSetHeaterTargetClicked()) );

    currentScene = NULL;

    clearPixmap = QPixmap::fromImage( QImage( "assets/MDX clear.png" ) );

//    clearPixmap.setSize( ui->graphicsView->size().width(), ui->graphicsView->size().width() );

    isConnectedToCamera = false;
    Videostreaming* vs = new Videostreaming;
    setVideoStream( vs );
//    videoStream = NULL;

    updateTimer = new QTimer( this );
    connect( updateTimer, SIGNAL(timeout()), this, SLOT( updateFrame() ) );

    updateTimer->start( QCTM_CAMERA_TICKRATE );
    setUpdateRate( QCTM_CAMERA_TICKRATE );

    // *** KL46 ***
//    kl46 = new KL46Manager();
    heaterSystemNumber = 8;
    LEDSystemNumber = 10;
    connect( &kl46, SIGNAL(systemUpdated()), this, SLOT(kl46StatusUpdated()) );
    // ************


    // *** Bluetooth ***
//    btManager = new BTManager();

    connect( &btManager, SIGNAL(messageReceived(QString,QString)), this, SLOT(btMessageReceived(QString,QString)) );
    connect( &btManager, SIGNAL(clientConnected(QString)), this, SLOT(btClientConnected(QString)) );
    connect( &btManager, SIGNAL(clientDisconnected(QString)), this, SLOT(btClientDisconnected(QString)) );

    btManager.startServer();
    // *****************



    // *** Developer Window
    this->setGeometry( 50, 50, 1300, 800 );


    // *** Teardrop Window ***
    tdw = new QTTeardropWindow();

    connect( tdw, SIGNAL(menuItemWasSelected(QString)), this, SLOT(menuItemWasSelected(QString) ) );
    tdw->show();
    tdw->setGeometry( 1920, 0, 160, 128 );

    loadMenu( "Main Menu" );
    // ***********************


    // *** Process Manager ***
    processManager = new QCProcessManager();
    connect( processManager, SIGNAL(issueCommand(QString,int)), this, SLOT(receiveCommand(QString,int) ) );

    buttonReloadProcessClicked();
    // ***********************

    // *** Load old results ***
    resultManager.loadAllResults();
    loadResultsToBrowser();

    syncROIManager();
    connect( ui->treeWidgetROIs, SIGNAL( itemClicked(QTreeWidgetItem*,int) ), this, SLOT(roiWasClicked(QTreeWidgetItem*,int)) );

    connect( ui->buttonAddROI, SIGNAL(clicked()), this, SLOT(buttonAddRoiWasClicked()) );
    connect( ui->buttonAddSignal, SIGNAL(clicked()), this, SLOT(buttonAddSignalWasClicked()) );
    connect( ui->buttonAddBackground, SIGNAL(clicked()), this, SLOT(buttonAddBackgroundWasClicked()) );
    connect( ui->buttonDeleteROI, SIGNAL(clicked()), this, SLOT(buttonDeleteRoiWasClicked()) );

    connect( ui->textBoxROIName, SIGNAL( editingFinished() ), this, SLOT(textBoxROINameChanged()) );
    connect( ui->textBoxROIX, SIGNAL(textChanged(QString)), this, SLOT(textBoxROIXChanged()) );
    connect( ui->textBoxROIY, SIGNAL(textChanged(QString)), this, SLOT(textBoxROIYChanged()) );
    connect( ui->textBoxROIRadius, SIGNAL(textChanged(QString)), this, SLOT(textBoxROIRadiusChanged()) );

    connect( ui->buttonStartProcess, SIGNAL(clicked()), this, SLOT(buttonStartProcessClicked()) );
    connect( ui->buttonReloadProcess, SIGNAL(clicked()), this, SLOT(buttonReloadProcessClicked()) );
}

QuickChipsMainWindow::~QuickChipsMainWindow()
{
    delete ui;

    clearDarkFrame();

    tdw->close();

    delete videoStream;
}

void QuickChipsMainWindow::setVideoStream( Videostreaming* stream )
{
    videoStream = stream;

    bool debugConnectSuccess    = connect( stream, SIGNAL(logDebugHandle(QString)),    this, SLOT(debugLog(QString) ) );
    bool criticalConnectSuccess = connect( stream, SIGNAL(logCriticalHandle(QString)), this, SLOT(debugLog(QString) ) );
    if ( !debugConnectSuccess )
        qDebug("logDebugHandle connection failure!" );
    if ( !criticalConnectSuccess )
        qDebug("logDebugHandle connection failure!" );

}


void QuickChipsMainWindow::setUpdateRate( int msec )
{
//    qDebug() << "setUpdateRate[" << msec << "] remaining[" << updateTimer->remainingTime() << "]";
    cameraTickRate = msec;
    updateTimer->setInterval( msec );

    updateCameraTickRateField();
}


void QuickChipsMainWindow::executeSystemCommand(QString command, int value)
{
    qDebug() << "executeSystemCommand[" << command << "] value[" << value << "]";

//    tdw->addItemToList( "[" + command + "," + QString::number(value) + "]" );
//    tdw->selectLastListItem();


    if ( command == "setExposure" )
    {
        setCameraExposure( QString::number( value ) );
    }
    else if ( command == "setHeaterTarget" )
    {
        kl46.setSystemP1( heaterSystemNumber, value );
    }
    else if ( command == "setHeatDuration" )
    {
        // not implemented, we do heating duration via a process wait command
    }
    else if ( command == "setHeaterDirection" )
    {
        kl46.setSystemDirection( heaterSystemNumber, value );
    }
    else if ( command == "setLEDDirection" )
    {
        kl46.setSystemDirection( LEDSystemNumber, value );
    }
    else if ( command == "captureDark" )
    {
        buttonCaptureDarkClicked();
    }
    else if ( command == "capture" )
    {
        buttonCaptureClicked();
    }
    else if ( command == "assay_complete" )
    {
        tdw->addItemToList( QString("Assay Complete") );
        tdw->addItemToList( QString("Back Arrow to Return") );
        tdw->selectLastListItem();
    }
}


void QuickChipsMainWindow::loadMenu( QString menuName )
{
    tdw->clearList();

    qDebug() << "loadMenu" << menuName;

    if ( menuName == "Main Menu" )
    {
        tdw->addItemToList( QString("Start Test") );
        tdw->addItemToList( QString("Results") );
        tdw->addItemToList( QString("Settings") );
        tdw->addItemToList( QString("Shut Down") );

        int batteryPercent = getBatteryPercent();
        tdw->addItemToList( QString("Battery %1\%").arg(batteryPercent) );
    }
    else if ( menuName ==("Start Test" ) )
    {
        tdw->addItemToList( QString("Load Chip and continue") );
    }    
    else if ( menuName == ("Pre Test" ) )
    {        
        tdw->addItemToList( QString("in pretest") );
    }
    else if ( menuName == ("Run Test" ) )
    {
        tdw->addItemToList( QString("(press back to abort)") );
        tdw->addItemToList( QString("Running Assay:") );
        tdw->addItemToList( QString("[" + processManager->processName + "]") );

        processManager->startProcess();
    }
    else if ( menuName == ("Results" ) )
    {
        for ( int i = 0; i < ui->listWidgetResultsBrowser->count(); i++ )
        {
            QListWidgetItem* item = ui->listWidgetResultsBrowser->item(i);
            tdw->addItemToList( item->text() );
        }
    }
    else if ( menuName.contains("result summary:" ) )
    {
        QStringList split = menuName.split(":");
        QString filename = split[1];

        QString resultFile = resultManager.loadResultFile(filename);
        QStringList lines = resultFile.split("\n");

        foreach ( QString line, lines )
        {
            line.replace( "\t", "" );

            tdw->addItemToList( line );
        }
    }
    else if ( menuName == ("Shut Down" ) )
    {         
        tdw->addItemToList( QString("Back") );
        tdw->addItemToList( QString("Confirm Shut Down") );
    }
    else
    {
        tdw->addItemToList( QString("Error: menu name not found.") );
    }


    tdw->selectFirstListItem();


}


void QuickChipsMainWindow::menuItemWasSelected( QString itemName )
{
    qDebug() << "menuItemSelected " << itemName;

    if ( itemName == "Start Test" )
    {
        loadMenu( "Start Test" );
    }
    else if ( itemName == "Load Chip and continue" )
    {
        loadMenu( "Run Test" );
    }
    else if ( itemName == "Results" )
    {
        loadMenu( "Results" );
    }
    else if ( itemName == "Back" )
    {
        processManager->stopProcess();
        loadMenu( "Main Menu" );
    }
    else if ( itemName == "Confirm Shut Down" )
    {
        QProcess* shutdownProcess = new QProcess(this);
        QString shutdownScriptName = "shutdownPlz";
        QStringList shutdownScriptArgs;
//        shutdownScriptArgs << "-m" << "IVDAnalyzer.txt" << "Exp1.raw";

        shutdownProcess->startDetached( shutdownScriptName, shutdownScriptArgs );

        this->close();
    }
    else if ( itemName == "Shut Down" )
    {
        loadMenu( "Shut Down" );
    }
    else if ( itemName.contains( "result_" ) )
    {
        QStringList split = itemName.split(".");
        QString filename = split[0];
        filename = filename + ".txt";
        loadMenu ("result summary:" + filename);
    }

}


void QuickChipsMainWindow::receiveCommand(QString command, int value)
{
    executeSystemCommand( command, value );
}

void QuickChipsMainWindow::updateDeviceNumberList()
{
    int deviceBeginNumber,deviceEndNumber;
//    cameraMap.clear();
//    deviceNodeMap.clear();
//    int deviceIndex = 1;
//    availableCam.clear();

    deviceNumbers.clear();

    //***debug hack
//    deviceNumbers.push_back(2);
//    return;
    //***end debug hack

    QDir qDir;
    if( qDir.cd("/sys/class/video4linux/") )
    {
        QStringList filters,list;
        filters << "video*";
        qDir.setNameFilters(filters);
        list << qDir.entryList(filters,QDir::Dirs ,QDir::Name);
        qSort(list.begin(), list.end());
        deviceBeginNumber = list.value(0).mid(5).toInt();   //Fetching all values after "video"
        deviceEndNumber = list.value(list.count()-1).mid(5).toInt();
        for ( int qDevCount = deviceBeginNumber; qDevCount <= deviceEndNumber; qDevCount++ )
        {
//            qDebug("%d", qDevCount );
            deviceNumbers.push_back(qDevCount);
        }
    }
    else
    {

    }
}


bool QuickChipsMainWindow::connectToCamera()
{

    if ( videoStream != NULL )
    {
        videoStream->stopCapture();
        videoStream->closeDevice();
        delete videoStream;
        videoStream = NULL;
    }

    Videostreaming* vs = new Videostreaming;
    setVideoStream( vs );

    currentDeviceIndex = -1;
    updateDeviceNumberList();

    for ( std::list<int>::iterator it = deviceNumbers.begin(); it != deviceNumbers.end(); ++it )
    {
        int deviceNumber = *it;
        qDebug("QuickChipsMainWindow::connectToCamera() is trying device #%d", deviceNumber );

        videoStream->stopCapture();
        videoStream->closeDevice();

        videoStream->getCameraName("See3CAM_12CUNIR");
        videoStream->getFirstDevice( deviceNumber );
        videoStream->setDevice( "/dev/video" );

        if ( videoStream->openSuccess )
        {
            videoStream->displayOutputFormat();
            videoStream->displayStillResolution();
            videoStream->displayVideoResolution();

//            videoStream->setResoultion("1280x960");
            videoStream->setResoultion("640x480");

            videoStream->startAgain();

            videoStream->masterModeEnabled();

            videoStream->changeSettings( V4L2_CID_EXPOSURE_ABSOLUTE, "35000" );

            getAndUpdateCameraExposureField();

            it = deviceNumbers.end();

            return true;
        }
    }
    return false;
}


void QuickChipsMainWindow::getAndUpdateCameraExposureField()
{
    if ( videoStream != NULL && videoStream->openSuccess )
    {
        QString exposureAsString = videoStream->getSettings( V4L2_CID_EXPOSURE_ABSOLUTE );
        qDebug() << "exposure from camera is [" << exposureAsString << "]";
        ui->textBoxExposure->setText( exposureAsString );
    }
}


void QuickChipsMainWindow::setCameraExposure( QString newExposure )
{
    if ( videoStream != NULL && videoStream->openSuccess )
    {
        qDebug() << "setting camera exposure[" << newExposure;
//        QString exposureAsString = QString("%1").args( newExposure );
        videoStream->changeSettings( V4L2_CID_EXPOSURE_ABSOLUTE, newExposure );

        getAndUpdateCameraExposureField();
    }
}


void QuickChipsMainWindow::loadGlobalImageNumber()
{
    QFile file("globalImageNumber.txt");
    bool fileSuccess = file.open( QIODevice::ReadOnly | QIODevice::Text );
    if ( fileSuccess )
    {
        QTextStream in(&file);
        while( !in.atEnd() )
        {
            QString line = in.readLine();
            QStringList items = line.split(",");
            if ( items.count() == 2 )
            {
                if ( items[0] == "globalImageNumber" )
                {
                    int value = items[1].toInt();
                    globalImageNumber = value;
                    qDebug() << "globalImageNumber loaded [" << value << "]";
                }
            }
        }
    }
    else
    {
        qDebug() << "globalImageNumber file not found! Defaulting to 0";
        globalImageNumber = 0;
    }
    file.close();
}

void QuickChipsMainWindow::saveGlobalImageNumber()
{
    qDebug() << "saveGlobalImageNumber enter";
    QFile::remove( "globalImageNumber.txt" );
    qDebug() << "saveGlobalImageNumber old file removed...";
    QFile file( "globalImageNumber.txt" );
    if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
        return;

    qDebug() << "saveGlobalImageNumber globalImageNumber.txt open successfully";
    file.flush();
    QTextStream out(&file);

    out << "globalImageNumber," << globalImageNumber;

    file.close();
}


void QuickChipsMainWindow::kl46StatusUpdated()
{
    // *** LED Button Update ***
    int ledState = kl46.getSystemState( LEDSystemNumber );
    int ledFeedback1 = kl46.getSystemFeedback( LEDSystemNumber );
    float ledFeedbackFloat = (float) ledFeedback1;
    ledFeedbackFloat /= 100;   // go to C from centi C

    QString ledFeedbackText = QString();
    ledFeedbackText = ledFeedbackText.sprintf("[%+.2f]", ledFeedbackFloat);

    QString ledStateText = ledState ? "ON" : "OFF";

    QString ledButtonText = QString("Illumination %1 %2").arg(ledStateText).arg(ledFeedbackText);
    ui->buttonLED->setText( ledButtonText );

    Qt::GlobalColor newLEDButtonColor = ledState ? Qt::green : Qt::darkGray;
    setButtonColor( ui->buttonLED, newLEDButtonColor );


    // *** Heater Button Update ***
    int heaterState = kl46.getSystemState( heaterSystemNumber );
    int heaterFeedback1 = kl46.getSystemFeedback( heaterSystemNumber );
    float heaterFeedbackFloat = (float) heaterFeedback1;
    heaterFeedbackFloat /= 100;   // go to C from centi C

    QString heaterFeedbackText = QString();
    heaterFeedbackText = heaterFeedbackText.sprintf("[%+.2f]", heaterFeedbackFloat);

    QString heaterStateText = heaterState ? "ON" : "OFF";

    QString heaterButtonText = QString("Heater %1 %2").arg(heaterStateText).arg(heaterFeedbackText);
    ui->buttonHeater->setText ( heaterButtonText );

    Qt::GlobalColor newHeaterButtonColor = heaterState ? Qt::green : Qt::darkGray;
    setButtonColor( ui->buttonHeater, newHeaterButtonColor );
}


void QuickChipsMainWindow::setButtonColor(QPushButton *button, Qt::GlobalColor newQtColor )
{
    QPalette p = button->palette();
    p.setBrush(QPalette::Button, newQtColor );
    button->setPalette( p );
}


// door switch                      /sys/class/gpio/gpio119/value
// BT blue          led 0 or 1 to   /sys/class/gpio/gpio106/value
// Battery green    led 0 or 1 to   /sys/class/gpio/gpio107/value
// backlight 0 or 1 to              /sys/class/gpio/gpio204/value
void QuickChipsMainWindow::setBatteryLight( int value )
{
    QFile file( "/sys/class/gpio/gpio107/value" );
    if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
        return;

    file.flush();
    QTextStream out(&file);

    out << value;

    file.close();
}

void QuickChipsMainWindow::setBluetoothLight( int value )
{
    QFile file( "/sys/class/gpio/gpio106/value" );
    if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
        return;

    file.flush();
    QTextStream out(&file);

    out << value;

    file.close();
}

void QuickChipsMainWindow::setBacklight( int value )
{
    QFile file( "/sys/class/gpio/gpio204/value" );
    if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
        return;

    file.flush();
    QTextStream out(&file);

    out << value;

    file.close();
}

int QuickChipsMainWindow::getDoorSwitchPosition()
{
    int value = -1;

    QFile file("roibank.txt");
    bool fileSuccess = file.open( QIODevice::ReadOnly | QIODevice::Text );
    if ( fileSuccess )
    {
        QTextStream in(&file);

        QString stringValue = in.readAll();
        value = stringValue.toInt();
    }
    file.close();

    return value;
}


void QuickChipsMainWindow::updateFrame()
{
//    videoStream->capFrame();
//    QPixmap pixmap;

    // easy way to keep battery percent updated here....
    updateMenuBatteryPercent();

    int width = videoStream->width;
    int height = videoStream->height;
    int bufferSize = width * height * 2;
    unsigned char* rawBuffer = (unsigned char *)malloc( bufferSize );
    bool captureSuccess = false;

    if ( videoStream->openSuccess )
        captureSuccess = videoStream->capRawY16Frame( rawBuffer );

    // brute force image validation
    bool imageIsValid = false;

    int bufferValidationStepSize = 3333;
    for ( int i = 0; i < bufferSize; i += bufferValidationStepSize )
    {
        unsigned char value = rawBuffer[i];
        if ( value != 0 )
        {
            imageIsValid = true;
            i = bufferSize;
        }
    }

    if ( !imageIsValid )
    {
        emptyFrameCount++;
        qDebug() << "Warning: Empty frame found, will skip, emptyFrameCount[" << emptyFrameCount << "]";
    }

    if ( !captureSuccess )
    {
        qDebug() << "ERROR capture failure";
        captureConsecutiveErrorCount++;
    }



    if ( !isConnectedToCamera || captureConsecutiveErrorCount > QCTM_CAMERA_BAD_FRAME_COUNT_FOR_RECONNECT )
    {        
        captureConsecutiveErrorCount = 0;
        qDebug() << "Error: captureConsecutiveErrorCount > " << QCTM_CAMERA_BAD_FRAME_COUNT_FOR_RECONNECT << ", attempting to reconnect to camera";

        isConnectedToCamera = connectToCamera();

//        if ( cameraTickRate != QCTM_CAMERA_ERROR_SLEEP )
//            setUpdateRate( QCTM_CAMERA_ERROR_SLEEP );

    }
    else if ( captureSuccess )
    {
        isConnectedToCamera = true;
        captureConsecutiveErrorCount = 0;
//        if ( cameraTickRate != QCTM_CAMERA_TICKRATE )
//            setUpdateRate( QCTM_CAMERA_TICKRATE );
    }

    if ( !captureSuccess )
    {
        free(rawBuffer);
        return;
    }

//    if ( !imageIsValid )
//    {
//        return;
//    }

    if ( doSaveNextDarkImage )
    {
        doSaveNextDarkImage = false;

        if ( darkFrameBuffer != NULL )
        {
            clearDarkFrame();
        }

        darkFrameBuffer = (unsigned char *)malloc( bufferSize );
        memcpy( darkFrameBuffer, rawBuffer, bufferSize );
    }


    // *** Dark Correction ***
    if ( ui->checkBoxApplyDarkCorrection->isChecked() )
    {
        if ( darkFrameBuffer != NULL )
        {
            int numPixels = width * height;
            int pixel = 0;
            int pixelDark = 0;
            for ( int i = 0; i < numPixels; i++ )
            {
                pixel       = (rawBuffer[i*2+1] * 256) + rawBuffer[i*2];
                pixelDark   = (darkFrameBuffer[i*2+1] * 256) + darkFrameBuffer[i*2];

                int newPixel = pixel - pixelDark;

                if ( newPixel < 0 )
                    newPixel = 0;

                rawBuffer[i*2+1] = newPixel/256;
                rawBuffer[i*2] = newPixel;
            }
        }
    }


    if ( doSaveNextImage )
    {
        doSaveNextImage = false;

        //*** Save Image ***        
        QString resultImageFilename = QString("assay_images/image_%1.raw").arg( globalImageNumber );
        QFile file( resultImageFilename );
        if(file.open(QIODevice::WriteOnly))
        {
            file.write( (const char*)rawBuffer, bufferSize );
            qDebug("Saved image [" + resultImageFilename.toLatin1() + "]");
            file.close();
        }
        else
        {
            qDebug() << "ERROR: error opening file " << resultImageFilename << " for writing";
        }
        // ******************


        //*** Save Dark Image ***
        if ( darkFrameBuffer != NULL )
        {
            QString resultImageFilename = QString("assay_images/dark_%1.raw").arg( globalImageNumber );
            QFile file( resultImageFilename );
            if(file.open(QIODevice::WriteOnly))
            {
                file.write( (const char*)darkFrameBuffer, bufferSize );
                qDebug("Saved dark image [" + resultImageFilename.toLatin1() + "]");
                file.close();
            }
            else
            {
                qDebug() << "ERROR: error opening file " << resultImageFilename << " for writing";
            }
        }
        // ******************

        roiManager.analyizeCurrentRoiSet( rawBuffer, width, height, ui->treeWidgetROIResults );

        QString resultManagerString = resultManager.generateAnalysisResultsFromTree( ui->treeWidgetROIResults );

        QString resultFilename = QString( "result_%1.txt").arg(globalImageNumber);
        QString resultFilePath = QString ("assay_results/%1").arg(resultFilename);

        QStringList resultStringList;
        resultStringList << resultFilename << "\n"
                         << "Assay Name:" << processManager->processName << "\n\n"
                         << resultManagerString;
        QString resultString = resultStringList.join("");

        resultManager.saveResultStringToFile( resultString, resultFilePath );


        // *** Global device image number ***
        globalImageNumber++;
        saveGlobalImageNumber();

        resultManager.loadAllResults();
        loadResultsToBrowser();

//         *** Invoke ImageJ ***
//        bool doImageJ = false;
//        if ( doImageJ )
//        {
//            QProcess* imageJ = new QProcess(this);
//            QString imageJExec = "imagej";
//            QStringList imageJArgs;
//            imageJArgs << "-m" << "IVDAnalyzer.txt" << "Exp1.raw";
//    //        imageJArgs << "-port0";

//            imageJ->start( imageJExec, imageJArgs );
//        }
        // *********************

    }


    if ( imageIsValid && ui->checkBoxNormalizeDisplay->isChecked() )
        {
            //*** image normalization ***
            int numPixels = width*height;
            float numPixelsf = (float)numPixels;
            float normalizationCeilingFactor = 1.0f;
            int maxValue = 4096;
            int histogram[maxValue];
            memset( histogram, 0, maxValue );
            for ( int i = 0; i < maxValue; i++ )
            {
                histogram[i] = 0;
            }

            for ( int i = 0; i < numPixels; i++ )
            {
                int pixel = (rawBuffer[i * 2 + 1] * 256 + rawBuffer[i * 2]);
                histogram[pixel]++;
            }
            float normalizationCeilingPercent = 0.97;
            float normalizationFloorPercent = 0.97;
            int numPixelsAboveCeiling = (int)((float)numPixels - (numPixelsf * normalizationCeilingPercent));
            int numPixelsBelowFloor = (int)((float)numPixels - (numPixelsf * normalizationFloorPercent));

//            qDebug() << "numpixels>ceil" << numPixelsAboveCeiling;

            int iHistoCeiling = maxValue-1;

            int numPixelsInBucket = histogram[iHistoCeiling];
            numPixelsAboveCeiling -= numPixelsInBucket;

//            qDebug() << "numpixels>ceil" << numPixelsAboveCeiling << " npinbuket" << numPixelsInBucket;

            while ( numPixelsAboveCeiling > 0 )
            {
                --iHistoCeiling;
                numPixelsInBucket = histogram[iHistoCeiling];
                numPixelsAboveCeiling -= numPixelsInBucket;
            }

            ++iHistoCeiling;
            normalizationCeilingFactor = (1.0f / ((float)iHistoCeiling / (float)maxValue));
//            qDebug() << "iHisto" << iHistoCeiling << " mv" << maxValue;

            float pixelFloat = 0;
            int pixval = 0;
            for ( int i = 0; i < numPixels; i++ )
            {
                pixelFloat = ( (((float)rawBuffer[i*2+1]*256) + ((float)rawBuffer[i*2])) );
                pixelFloat *= normalizationCeilingFactor;

//                if ( i == 999 )
//                    qDebug() << "p" << normalizationCeilingFactor;


                if ( pixelFloat >= 4095 )
                    pixelFloat = 4095;
                if ( pixelFloat < 0 )
                    pixelFloat = 0;

                pixval = pixelFloat;

                rawBuffer[i*2+1] = pixval/256;
                rawBuffer[i*2] = pixval;
            }
    //        delete histogram;
        }

    // this updates videoStream->qImage
    QPixmap* what = videoStream->convertRawY16Frame( rawBuffer );

    // call ROI Manager roi draw call here
    roiManager.drawROIsOnPixmap( &(videoStream->qImage) );
    // *******


    // *** frame update indicator ***
    QPainter fuPainter( &(videoStream->qImage) );
    fuPainter.setPen( Qt::yellow );
    int ls = 10;
    int ss = 5;
    if ( frameUpdateCounter == 0 )
        fuPainter.drawLine( ss, ss, 0, 0 );
    if ( frameUpdateCounter == 1 )
        fuPainter.drawLine( ss, ss, ss, 0 );
    if ( frameUpdateCounter == 2 )
        fuPainter.drawLine( ss, ss, ls, 0 );
    if ( frameUpdateCounter == 3 )
        fuPainter.drawLine( ss, ss, ls, ss );
    if ( frameUpdateCounter == 4 )
        fuPainter.drawLine( ss, ss, ls, ls );
    if ( frameUpdateCounter == 5 )
        fuPainter.drawLine( ss, ss, ss, ls );
    if ( frameUpdateCounter == 6 )
        fuPainter.drawLine( ss, ss, 0, ls );
    if ( frameUpdateCounter == 7 )
        fuPainter.drawLine( ss, ss, 0, ss );

    frameUpdateCounter++;
    if ( frameUpdateCounter >= 8 )
        frameUpdateCounter = 0;

    fuPainter.end();
    // ******************************


    // *** New Scene for new frame! ***
    if ( currentScene != NULL )
    {
        delete currentScene;
        currentScene = NULL;
    }
    currentScene = new QGraphicsScene();

    currentScene->addPixmap( videoStream->qImage );

    ui->graphicsView->setScene( currentScene );
    // *********************************

    free(rawBuffer);
}


void QuickChipsMainWindow::btMessageReceived( const QString& sender, const QString& message )
{
    qDebug() << "qcmw bt message" << sender << " " << message;
    if ( message == "{request_results}" )
    {
        foreach( QString result, resultManager.allResults )
        {
            QString resultMessage = "{" + result + "}";
            btManager.sendMessage( resultMessage );
        }
    }
    else if ( message == "{toggleLED}" )
    {
        buttonLEDClicked();
    }
    else if ( message == "{toggleHeater}" )
    {
        buttonHeaterClicked();
    }
}

void QuickChipsMainWindow::btClientConnected(const QString &name)
{
    setBluetoothLight( 1 );
    qDebug() << "qcmw bt btClientConnected" << name;
}

void QuickChipsMainWindow::btClientDisconnected(const QString &name)
{
    setBluetoothLight( 0 );
    qDebug() << "qcmw bt btClientDisconnected" << name;
}



void QuickChipsMainWindow::clearDarkFrame()
{
    free(darkFrameBuffer);
    darkFrameBuffer = NULL;
}

void QuickChipsMainWindow::syncROIManager()
{
    QTreeWidget* tree = ui->treeWidgetROIs;

    tree->clear();

    for ( int i = 0; i < roiManager.roiBank.count(); i++ )
    {
        ROISystem* roiSystem = roiManager.roiBank[i];
        QTreeWidgetItem* roiSystemItem = new QTreeWidgetItem( tree );

//        QVariant roiWrapper;
//        roiWrapper.setValue( roiSystem );
//        roiSystemItem->setData( 0, Qt::UserRole, roiWrapper );

        roiSystemItem->setText( 0, QString("[ROI %2] %1").arg(roiSystem->name).arg(i+1) );
        ui->treeWidgetROIs->addTopLevelItem( roiSystemItem );

        for ( int iSignal = 0; iSignal < roiSystem->signalROIs.count(); iSignal++ )
        {
            QTreeWidgetItem* signalItem = new QTreeWidgetItem( roiSystemItem );
            signalItem->setText( 0, QString("[%1] signal").arg(iSignal+1) );

//            ROI* signal = roiSystem->signalROIs[iSignal];
//            QVariant signalWrapper;
//            signalWrapper.setValue( signal );
//            signalItem->setData( 0, Qt::UserRole, signalWrapper );
        }

        for ( int iBackground = 0; iBackground < roiSystem->backgroundROIs.count(); iBackground++ )
        {
            QTreeWidgetItem* backgroundItem = new QTreeWidgetItem( roiSystemItem );
            backgroundItem->setText( 0, QString("[%1] background").arg(iBackground+1) );
        }
    }
}


void QuickChipsMainWindow::loadResultsToBrowser()
{
    ui->listWidgetResultsBrowser->clear();

    foreach (QString result, resultManager.allResults )
    {
        QStringList resultLines = result.split("\n");
        if ( resultLines.length() > 5 )
        {
//            QString resultFilename = resultLines[0];
//            QString resultDate = resultLines[3];
            QString resultListEntry = QString("%1 %2").arg(resultLines[0]).arg(resultLines[3]);
            QListWidgetItem* listItem = new QListWidgetItem( resultListEntry, ui->listWidgetResultsBrowser );
        }
    }
}


void QuickChipsMainWindow::roiWasClicked( QTreeWidgetItem* treeItem, int column )
{
    QString name = treeItem->text(0);

    bool isSignal = name.contains( "signal" );
    if ( isSignal || name.contains( "background" ) )
    {
        QTreeWidgetItem* parent = treeItem->parent();
        QString roiSystemName = parent->text(0);
        int indexOfROISystemNumber = 5;
        int indexOfROITargetNumber = 1;

        int indexOfRoiBracket = roiSystemName.indexOf( ']', 0, Qt::CaseInsensitive ) - indexOfROISystemNumber;
        int indexOfTargetBracket = name.indexOf( ']', 0, Qt::CaseInsensitive ) - indexOfROITargetNumber;

        QString roiSystemNumber = roiSystemName.mid( indexOfROISystemNumber, indexOfRoiBracket );
        QString targetNumber = name.mid( indexOfROITargetNumber, indexOfTargetBracket );

        int roiSystemIndex = roiSystemNumber.toInt() - 1;
        int roiSystemTarget = targetNumber.toInt() - 1;

        if ( isSignal )
            roiManager.setSelectedRoiSignal( roiSystemIndex, roiSystemTarget );
        else
            roiManager.setSelectedRoiBackground( roiSystemIndex, roiSystemTarget );

        ROISystem* system = roiManager.getRoi( roiSystemIndex );
        if ( system != NULL )
        {
            ROI* target = NULL;
            if ( isSignal )
                target = system->signalROIs[roiSystemTarget];
            else
                target = system->backgroundROIs[roiSystemTarget];

            ui->textBoxROIX->setText( QString("%1").arg(target->center.x()) );
            ui->textBoxROIY->setText( QString("%1").arg(target->center.y()) );
            ui->textBoxROIRadius->setText( QString("%1").arg(target->radius) );
            ui->textBoxROIName->setText( name );
        }

    }
    else if ( name.contains( "[ROI" ) )
    {
        int indexOfROISystemNumber = 5;
        int indexOfRoiBracket = name.indexOf( ']', 0, Qt::CaseInsensitive ) - indexOfROISystemNumber;

        QString roiSystemNumber = name.mid( indexOfROISystemNumber, indexOfRoiBracket );

        int roiSystemIndex = roiSystemNumber.toInt() - 1;

        roiManager.setSelectedRoi( roiSystemIndex );
        ROISystem* system = roiManager.getRoi( roiSystemIndex );
        ui->textBoxROIName->setText( system->name );
        ui->textBoxROIX->setText( QString("") );
        ui->textBoxROIY->setText( QString("") );
        ui->textBoxROIRadius->setText( QString("") );
    }
}


void QuickChipsMainWindow::buttonAddRoiWasClicked()
{
    roiManager.addRoi( QString("New ROI" ) );

    syncROIManager();
    roiManager.saveRoiBank();
}

void QuickChipsMainWindow::buttonAddSignalWasClicked()
{
    roiManager.addSignal( roiManager.currentRoiIndex );

    syncROIManager();
    roiManager.saveRoiBank();
}

void QuickChipsMainWindow::buttonAddBackgroundWasClicked()
{
    roiManager.addBackground( roiManager.currentRoiIndex );

    syncROIManager();
    roiManager.saveRoiBank();
}

void QuickChipsMainWindow::buttonDeleteRoiWasClicked()
{
    ROISystem* system = roiManager.getRoi( roiManager.currentRoiIndex );
    ROI* target = roiManager.getCurrentTarget();

    if ( target != NULL )
    {
        bool isSignal = roiManager.currentSignalIndex >= 0;
        if ( isSignal )
            system->signalROIs.removeAt( roiManager.currentSignalIndex );
        else
            system->backgroundROIs.removeAt( roiManager.currentBackgroundIndex );

    }
    else if ( system != NULL )
    {
        roiManager.roiBank.removeAt( roiManager.currentRoiIndex );
    }

    ui->textBoxROIName->clear();
    ui->textBoxROIX->clear();
    ui->textBoxROIY->clear();
    ui->textBoxROIRadius->clear();

    syncROIManager();
    roiManager.saveRoiBank();
}


void QuickChipsMainWindow::textBoxROINameChanged()
{
    QString newName = ui->textBoxROIName->text();
    ROISystem* system = roiManager.getRoi( roiManager.currentRoiIndex );
    ROI* currentTarget = roiManager.getCurrentTarget();
    if ( currentTarget != NULL )
    {

    }
    else if ( system != NULL )
    {
        system->name = newName;
        syncROIManager();
        roiManager.saveRoiBank();
    }
}


void QuickChipsMainWindow::textBoxROIXChanged()
{
    int newValue = ui->textBoxROIX->text().toInt();
    ROI* currentTarget = roiManager.getCurrentTarget();
    if ( currentTarget != NULL )
    {
        QPoint newCenter = QPoint( newValue, currentTarget->center.y() );
        currentTarget->center = newCenter;
    }
    roiManager.saveRoiBank();
}

void QuickChipsMainWindow::textBoxROIYChanged()
{
    int newValue = ui->textBoxROIY->text().toInt();
    ROI* currentTarget = roiManager.getCurrentTarget();
    if ( currentTarget != NULL )
    {
        QPoint newCenter = QPoint( currentTarget->center.x(), newValue );
        currentTarget->center = newCenter;
    }
    roiManager.saveRoiBank();
}

void QuickChipsMainWindow::textBoxROIRadiusChanged()
{
    int newValue = ui->textBoxROIRadius->text().toInt();
    ROI* currentTarget = roiManager.getCurrentTarget();
    if ( currentTarget != NULL )
    {
        currentTarget->radius = newValue;
    }
    roiManager.saveRoiBank();
}


int QuickChipsMainWindow::getBatteryPercent()
{

    apm_info info = { 0 };
    memset(&info, 0, sizeof(apm_info));

    int result = -1;
//    apm_info* info = (apm_info*) info;
    result = apm_read(&info);

//    info.battery_percentage = 42;

//    qDebug() << "Battery percent apm query result:" << result << " percent:" << info.battery_percentage;

    return info.battery_percentage;
}


void QuickChipsMainWindow::updateMenuBatteryPercent()
{
//    QList<QListWidgetItem*> menuItems = tdw->listWidget->itemAt();
//    foreach( QListWidgetItem* item, tdw->listWidget->items )
    for ( int i = 0; i < tdw->listWidget->count(); i++ )
    {
        QListWidgetItem* item = tdw->listWidget->item(i);
        if ( item->text().contains( "Battery" ) )
        {
            int batteryPercent = getBatteryPercent();
            QString batteryText = QString("Battery %1\%").arg(batteryPercent);
            item->setText( batteryText );
        }
    }
}

void QuickChipsMainWindow::buttonCaptureClicked()
{
    doSaveNextImage = true;
}

void QuickChipsMainWindow::buttonCaptureDarkClicked()
{
    doSaveNextDarkImage = true;
}

void QuickChipsMainWindow::buttonClearDarkClicked()
{
    clearDarkFrame();
}

void QuickChipsMainWindow::buttonQuitClicked()
{
//    this->close();
    emit closeCalled();
}

void QuickChipsMainWindow::buttonReconnectClicked()
{
    emit reconnect();
}


void QuickChipsMainWindow::buttonStartProcessClicked()
{
    processManager->startProcess();
}


void QuickChipsMainWindow::buttonReloadProcessClicked()
{
    processManager->loadProcessFile( QString("assay_definitions/assay_1.csv") );

    qDebug() <<" pm" << processManager->processCommands.length();

    ui->labelAssayName->setText( processManager->processName );

    ui->listWidgetProcessInstructions->clear();

    foreach( ProcessCommand* command, processManager->processCommands )
    {
        QString commandString = command->getCommandAsString();
        QListWidgetItem* newItem = new QListWidgetItem( commandString );
        ui->listWidgetProcessInstructions->addItem( newItem );
    }
}

void QuickChipsMainWindow::buttonLEDClicked()
{
    int currentState = kl46.getSystemState( LEDSystemNumber );
    kl46.setSystemDirection( LEDSystemNumber, 1 - currentState );
}


void QuickChipsMainWindow::buttonHeaterClicked()
{
    int currentState = kl46.getSystemState( heaterSystemNumber );
    kl46.setSystemDirection( heaterSystemNumber, 1 - currentState );
}


void QuickChipsMainWindow::buttonSetHeaterTargetClicked()
{
    int textBoxValue = ui->textBoxHeaterTarget->text().toInt();
    kl46.setSystemP1( heaterSystemNumber, textBoxValue );
}

void QuickChipsMainWindow::buttonGetHeaterTargetClicked()
{
    int systemValue = kl46.getSystemP1( heaterSystemNumber );
    ui->textBoxHeaterTarget->setText( QString::number(systemValue) );
}

void QuickChipsMainWindow::buttonDebugAClicked()
{
//    processManager->startProcess();
    getBatteryPercent();
}

void QuickChipsMainWindow::buttonDebugBClicked()
{
    if ( tdw->geometry().x() < 1920 )
       tdw->setGeometry( 1920, 0, 160, 128 );
    else
       tdw->setGeometry( 1920 - 160, 0, 160, 128 );

    tdw->selectFirstListItem();
}

void QuickChipsMainWindow::buttonBTSendClicked()
{
    QString message = ui->textBoxBTDBG->text();
    btManager.sendMessage( message );
}

void QuickChipsMainWindow::setExposureFromTextBox()
{
    QString textBoxValue = ui->textBoxExposure->text();
    setCameraExposure( textBoxValue );
}


void QuickChipsMainWindow::updateCameraTickRateField()
{
    QString tickRateString = QString( "%1").arg( QString::number(cameraTickRate) );
    ui->textBoxCameraTickRate->setText( tickRateString );
}

void QuickChipsMainWindow::setCameraTickRateFromTextBox()
{
    int newTickRate = ui->textBoxCameraTickRate->text().toInt();
    setUpdateRate( newTickRate );
}


void QuickChipsMainWindow::debugLog( QString message )
{
    QByteArray byteArray = message.toLatin1();
    const char* c_str = byteArray.data();
    qDebug( c_str );
}

void QuickChipsMainWindow::mousePressEvent( QMouseEvent* event )
{
    emit tapped();
}


void QuickChipsMainWindow::keyPressEvent( QKeyEvent* event )
{
    int key = event->key();

    if ( key == Qt::Key_Up )
    {
        qDebug("QuickChipsMainWindow up");
    }
    else if ( key == Qt::Key_Right )
    {
        qDebug("QuickChipsMainWindow right");
    }
    else if ( key == Qt::Key_Down )
    {
        qDebug("QuickChipsMainWindow down");
    }
    else if ( key == Qt::Key_Left )
    {
        qDebug("QuickChipsMainWindow left");
    }
}


void QuickChipsMainWindow::closeEvent( QCloseEvent* event )
{

    tdw->close();
}
