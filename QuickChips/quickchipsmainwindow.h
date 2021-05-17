#ifndef QUICKCHIPSMAINWINDOW_H
#define QUICKCHIPSMAINWINDOW_H

#include <QMainWindow>

#include <QtWidgets>

//#include "IVDCore/IVDCamera.h"

#include "qtcamsrc/videostreaming.h"

#include "IVDCore/kl46manager.h"
#include "IVDCore/btmanager.h"

#include "IVDCore/roi.h"
#include "IVDCore/roisystem.h"
#include "IVDCore/roimanager.h"

#include "qtteardropwindow.h"
#include "IVDCore/ProcessManager/qcprocessmanager.h"

#include "qcresultsmanager.h"


namespace Ui {
class QuickChipsMainWindow;
class UpdateThread;
}

class QuickChipsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QuickChipsMainWindow(QWidget *parent = 0);
    ~QuickChipsMainWindow();

    void setVideoStream( Videostreaming* stream );

    void setUpdateRate( int msec );

    void executeSystemCommand( QString command, int value );

private:
    Ui::QuickChipsMainWindow *ui;

    QTTeardropWindow* tdw;
    void loadMenu( QString menuName );
//    void menuItemWasSelected( QString menuName, QString itemName );

    QCProcessManager* processManager;

    Videostreaming* videoStream;

    int currentDeviceIndex;
    std::list<int> deviceNumbers;
    void updateDeviceNumberList();
    bool connectToCamera();

    int cameraTickRate;


    unsigned char* darkFrameBuffer;

    void setCameraExposure( QString newExposure );
    QTimer* updateTimer;

    bool updateThreadShouldRun;

    bool doSaveNextImage;
    bool doSaveNextDarkImage;

    void clearDarkFrame();

    bool isConnectedToCamera;
    int captureConsecutiveErrorCount;
    int emptyFrameCount;

    int globalImageNumber;
    void loadGlobalImageNumber();
    void saveGlobalImageNumber();


    int frameUpdateCounter;

    QPixmap clearPixmap;
    QPixmap pixmap;

    KL46Manager kl46;
    int heaterSystemNumber;
    int LEDSystemNumber;

    BTManager btManager;

    ROIManager roiManager;
    void syncROIManager();

    QCResultsManager resultManager;
    void loadResultsToBrowser();

    void setButtonColor( QPushButton* button, Qt::GlobalColor newQtColor );

    void setBatteryLight( int value );
    void setBluetoothLight( int value );
    void setBacklight( int value );
    int getDoorSwitchPosition();

    int getBatteryPercent();
    void updateMenuBatteryPercent();

private slots:
    void roiWasClicked( QTreeWidgetItem *treeItem, int column );
    void buttonAddRoiWasClicked();
    void buttonAddSignalWasClicked();
    void buttonAddBackgroundWasClicked();
    void buttonDeleteRoiWasClicked();

    void textBoxROINameChanged();
    void textBoxROIXChanged();
    void textBoxROIYChanged();
    void textBoxROIRadiusChanged();

private:

    QGraphicsScene* currentScene;
//    UpdateThread* updateThread;

//    IVDCamera* camera;a
    int debugFlagA;

signals:
    void tapped();
    void reconnect();
    void closeCalled();

public slots:

    void menuItemWasSelected( QString itemName );

    void receiveCommand( QString command, int value );

protected:
    void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

//    virtual void keyReleaseEvent(QKeyEvent *event);
//    virtual void focusInEvent(QFocusEvent *event);
//    virtual void focusOutEvent(QFocusEvent *event);
//    virtual void enterEvent(QEvent *event);
//    virtual void leaveEvent(QEvent *event);
//    virtual void paintEvent(QPaintEvent *event);
//    virtual void moveEvent(QMoveEvent *event);
//    virtual void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void kl46StatusUpdated();

    void updateFrame();

    void btMessageReceived( const QString& sender, const QString& message );
    void btClientConnected(const QString &name);
    void btClientDisconnected(const QString &name);

    void buttonCaptureClicked();

    void buttonCaptureDarkClicked();
    void buttonClearDarkClicked();

    void buttonQuitClicked();
    void buttonReconnectClicked();

    void buttonStartProcessClicked();
    void buttonReloadProcessClicked();

    void buttonLEDClicked();
    void buttonHeaterClicked();

    void buttonSetHeaterTargetClicked();
    void buttonGetHeaterTargetClicked();

    void buttonDebugAClicked();
    void buttonDebugBClicked();
    void buttonBTSendClicked();

    void getAndUpdateCameraExposureField();
    void setExposureFromTextBox();

    void updateCameraTickRateField();
    void setCameraTickRateFromTextBox();

    void debugLog(QString message);
};

#endif // QUICKCHIPSMAINWINDOW_H
