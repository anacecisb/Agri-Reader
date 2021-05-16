/********************************************************************************
** Form generated from reading UI file 'quickchipsmainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUICKCHIPSMAINWINDOW_H
#define UI_QUICKCHIPSMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QuickChipsMainWindow
{
public:
    QWidget *centralWidget;
    QGraphicsView *graphicsView;
    QGroupBox *groupBox;
    QLineEdit *textBoxExposure;
    QPushButton *buttonCapture;
    QLabel *label;
    QPushButton *buttonExposureSet;
    QPushButton *buttonExposureGet;
    QPushButton *buttonCaptureDark;
    QPushButton *buttonClearDark;
    QLabel *label_6;
    QLineEdit *textBoxCameraTickRate;
    QPushButton *buttonCameraTickRateGet;
    QPushButton *buttonCameraTickRateSet;
    QGroupBox *groupBox_2;
    QPushButton *buttonBTSend;
    QLineEdit *textBoxBTDBG;
    QLabel *label_2;
    QGroupBox *groupBox_3;
    QPushButton *buttonLED;
    QPushButton *buttonHeater;
    QGroupBox *groupBox_5;
    QTreeWidget *treeWidgetROIs;
    QGroupBox *groupBox_6;
    QLineEdit *textBoxROIRadius;
    QLineEdit *textBoxROIX;
    QLineEdit *textBoxROIY;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_3;
    QLineEdit *textBoxROIName;
    QLabel *label_9;
    QGroupBox *groupBox_7;
    QPushButton *buttonDeleteROI;
    QPushButton *buttonAddBackground;
    QPushButton *buttonAddSignal;
    QPushButton *buttonAddROI;
    QGroupBox *groupBox_8;
    QTreeWidget *treeWidgetROIResults;
    QGroupBox *groupBox_9;
    QPushButton *buttonStartProcess;
    QListWidget *listWidgetProcessInstructions;
    QPushButton *buttonReloadProcess;
    QLabel *labelAssayName;
    QGroupBox *groupBox_4;
    QPushButton *buttonDebugB;
    QPushButton *buttonDebugA;
    QMenuBar *menuBar;
    QMenu *menuQuickChis;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QuickChipsMainWindow)
    {
        if (QuickChipsMainWindow->objectName().isEmpty())
            QuickChipsMainWindow->setObjectName(QStringLiteral("QuickChipsMainWindow"));
        QuickChipsMainWindow->resize(1280, 800);
        QuickChipsMainWindow->setMaximumSize(QSize(1920, 1080));
        QIcon icon;
        icon.addFile(QStringLiteral("IVDLogoIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QStringLiteral("IVDLogoIcon.png"), QSize(), QIcon::Normal, QIcon::On);
        QuickChipsMainWindow->setWindowIcon(icon);
        QuickChipsMainWindow->setIconSize(QSize(64, 64));
        centralWidget = new QWidget(QuickChipsMainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(600, 30, 650, 490));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 341, 171));
        textBoxExposure = new QLineEdit(groupBox);
        textBoxExposure->setObjectName(QStringLiteral("textBoxExposure"));
        textBoxExposure->setGeometry(QRect(130, 110, 91, 22));
        buttonCapture = new QPushButton(groupBox);
        buttonCapture->setObjectName(QStringLiteral("buttonCapture"));
        buttonCapture->setGeometry(QRect(10, 30, 211, 31));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(buttonCapture->sizePolicy().hasHeightForWidth());
        buttonCapture->setSizePolicy(sizePolicy);
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 110, 111, 21));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        buttonExposureSet = new QPushButton(groupBox);
        buttonExposureSet->setObjectName(QStringLiteral("buttonExposureSet"));
        buttonExposureSet->setGeometry(QRect(280, 110, 41, 21));
        sizePolicy.setHeightForWidth(buttonExposureSet->sizePolicy().hasHeightForWidth());
        buttonExposureSet->setSizePolicy(sizePolicy);
        buttonExposureGet = new QPushButton(groupBox);
        buttonExposureGet->setObjectName(QStringLiteral("buttonExposureGet"));
        buttonExposureGet->setGeometry(QRect(230, 110, 41, 21));
        sizePolicy.setHeightForWidth(buttonExposureGet->sizePolicy().hasHeightForWidth());
        buttonExposureGet->setSizePolicy(sizePolicy);
        buttonCaptureDark = new QPushButton(groupBox);
        buttonCaptureDark->setObjectName(QStringLiteral("buttonCaptureDark"));
        buttonCaptureDark->setGeometry(QRect(230, 30, 91, 31));
        sizePolicy.setHeightForWidth(buttonCaptureDark->sizePolicy().hasHeightForWidth());
        buttonCaptureDark->setSizePolicy(sizePolicy);
        buttonClearDark = new QPushButton(groupBox);
        buttonClearDark->setObjectName(QStringLiteral("buttonClearDark"));
        buttonClearDark->setGeometry(QRect(230, 70, 91, 31));
        sizePolicy.setHeightForWidth(buttonClearDark->sizePolicy().hasHeightForWidth());
        buttonClearDark->setSizePolicy(sizePolicy);
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 140, 111, 21));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        textBoxCameraTickRate = new QLineEdit(groupBox);
        textBoxCameraTickRate->setObjectName(QStringLiteral("textBoxCameraTickRate"));
        textBoxCameraTickRate->setGeometry(QRect(130, 140, 91, 22));
        buttonCameraTickRateGet = new QPushButton(groupBox);
        buttonCameraTickRateGet->setObjectName(QStringLiteral("buttonCameraTickRateGet"));
        buttonCameraTickRateGet->setGeometry(QRect(230, 140, 41, 21));
        sizePolicy.setHeightForWidth(buttonCameraTickRateGet->sizePolicy().hasHeightForWidth());
        buttonCameraTickRateGet->setSizePolicy(sizePolicy);
        buttonCameraTickRateSet = new QPushButton(groupBox);
        buttonCameraTickRateSet->setObjectName(QStringLiteral("buttonCameraTickRateSet"));
        buttonCameraTickRateSet->setGeometry(QRect(280, 140, 41, 21));
        sizePolicy.setHeightForWidth(buttonCameraTickRateSet->sizePolicy().hasHeightForWidth());
        buttonCameraTickRateSet->setSizePolicy(sizePolicy);
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(600, 530, 341, 101));
        buttonBTSend = new QPushButton(groupBox_2);
        buttonBTSend->setObjectName(QStringLiteral("buttonBTSend"));
        buttonBTSend->setGeometry(QRect(150, 60, 111, 31));
        textBoxBTDBG = new QLineEdit(groupBox_2);
        textBoxBTDBG->setObjectName(QStringLiteral("textBoxBTDBG"));
        textBoxBTDBG->setGeometry(QRect(80, 30, 251, 22));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 30, 61, 21));
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 190, 341, 111));
        buttonLED = new QPushButton(groupBox_3);
        buttonLED->setObjectName(QStringLiteral("buttonLED"));
        buttonLED->setGeometry(QRect(10, 30, 321, 31));
        buttonHeater = new QPushButton(groupBox_3);
        buttonHeater->setObjectName(QStringLiteral("buttonHeater"));
        buttonHeater->setGeometry(QRect(10, 70, 321, 31));
        groupBox_5 = new QGroupBox(centralWidget);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(360, 10, 231, 731));
        treeWidgetROIs = new QTreeWidget(groupBox_5);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidgetROIs->setHeaderItem(__qtreewidgetitem);
        treeWidgetROIs->setObjectName(QStringLiteral("treeWidgetROIs"));
        treeWidgetROIs->setGeometry(QRect(10, 130, 211, 201));
        treeWidgetROIs->header()->setVisible(false);
        groupBox_6 = new QGroupBox(groupBox_5);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(10, 340, 211, 151));
        textBoxROIRadius = new QLineEdit(groupBox_6);
        textBoxROIRadius->setObjectName(QStringLiteral("textBoxROIRadius"));
        textBoxROIRadius->setGeometry(QRect(70, 120, 81, 22));
        textBoxROIX = new QLineEdit(groupBox_6);
        textBoxROIX->setObjectName(QStringLiteral("textBoxROIX"));
        textBoxROIX->setGeometry(QRect(70, 60, 81, 22));
        textBoxROIY = new QLineEdit(groupBox_6);
        textBoxROIY->setObjectName(QStringLiteral("textBoxROIY"));
        textBoxROIY->setGeometry(QRect(70, 90, 81, 22));
        label_4 = new QLabel(groupBox_6);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 90, 41, 21));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_5 = new QLabel(groupBox_6);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 120, 41, 21));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_3 = new QLabel(groupBox_6);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 60, 41, 21));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        textBoxROIName = new QLineEdit(groupBox_6);
        textBoxROIName->setObjectName(QStringLiteral("textBoxROIName"));
        textBoxROIName->setGeometry(QRect(70, 30, 131, 22));
        label_9 = new QLabel(groupBox_6);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(20, 30, 41, 21));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        groupBox_7 = new QGroupBox(groupBox_5);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setGeometry(QRect(10, 30, 211, 91));
        buttonDeleteROI = new QPushButton(groupBox_7);
        buttonDeleteROI->setObjectName(QStringLiteral("buttonDeleteROI"));
        buttonDeleteROI->setGeometry(QRect(10, 60, 91, 21));
        buttonAddBackground = new QPushButton(groupBox_7);
        buttonAddBackground->setObjectName(QStringLiteral("buttonAddBackground"));
        buttonAddBackground->setGeometry(QRect(110, 60, 91, 21));
        buttonAddSignal = new QPushButton(groupBox_7);
        buttonAddSignal->setObjectName(QStringLiteral("buttonAddSignal"));
        buttonAddSignal->setGeometry(QRect(110, 30, 91, 21));
        buttonAddROI = new QPushButton(groupBox_7);
        buttonAddROI->setObjectName(QStringLiteral("buttonAddROI"));
        buttonAddROI->setGeometry(QRect(10, 30, 91, 21));
        groupBox_8 = new QGroupBox(groupBox_5);
        groupBox_8->setObjectName(QStringLiteral("groupBox_8"));
        groupBox_8->setGeometry(QRect(10, 500, 211, 221));
        treeWidgetROIResults = new QTreeWidget(groupBox_8);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QStringLiteral("1"));
        treeWidgetROIResults->setHeaderItem(__qtreewidgetitem1);
        treeWidgetROIResults->setObjectName(QStringLiteral("treeWidgetROIResults"));
        treeWidgetROIResults->setGeometry(QRect(10, 30, 191, 181));
        treeWidgetROIResults->header()->setVisible(false);
        groupBox_9 = new QGroupBox(centralWidget);
        groupBox_9->setObjectName(QStringLiteral("groupBox_9"));
        groupBox_9->setGeometry(QRect(10, 310, 341, 431));
        buttonStartProcess = new QPushButton(groupBox_9);
        buttonStartProcess->setObjectName(QStringLiteral("buttonStartProcess"));
        buttonStartProcess->setGeometry(QRect(10, 30, 111, 31));
        listWidgetProcessInstructions = new QListWidget(groupBox_9);
        listWidgetProcessInstructions->setObjectName(QStringLiteral("listWidgetProcessInstructions"));
        listWidgetProcessInstructions->setGeometry(QRect(10, 90, 321, 331));
        buttonReloadProcess = new QPushButton(groupBox_9);
        buttonReloadProcess->setObjectName(QStringLiteral("buttonReloadProcess"));
        buttonReloadProcess->setGeometry(QRect(190, 30, 141, 31));
        labelAssayName = new QLabel(groupBox_9);
        labelAssayName->setObjectName(QStringLiteral("labelAssayName"));
        labelAssayName->setGeometry(QRect(10, 70, 321, 21));
        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(600, 640, 341, 101));
        buttonDebugB = new QPushButton(groupBox_4);
        buttonDebugB->setObjectName(QStringLiteral("buttonDebugB"));
        buttonDebugB->setGeometry(QRect(130, 30, 181, 31));
        buttonDebugA = new QPushButton(groupBox_4);
        buttonDebugA->setObjectName(QStringLiteral("buttonDebugA"));
        buttonDebugA->setGeometry(QRect(10, 30, 111, 31));
        QuickChipsMainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QuickChipsMainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1280, 19));
        menuQuickChis = new QMenu(menuBar);
        menuQuickChis->setObjectName(QStringLiteral("menuQuickChis"));
        QuickChipsMainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QuickChipsMainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QuickChipsMainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QuickChipsMainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QuickChipsMainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(buttonCapture, textBoxExposure);
        QWidget::setTabOrder(textBoxExposure, textBoxBTDBG);
        QWidget::setTabOrder(textBoxBTDBG, buttonBTSend);
        QWidget::setTabOrder(buttonBTSend, buttonDebugA);
        QWidget::setTabOrder(buttonDebugA, buttonDebugB);
        QWidget::setTabOrder(buttonDebugB, buttonAddROI);
        QWidget::setTabOrder(buttonAddROI, buttonAddSignal);
        QWidget::setTabOrder(buttonAddSignal, buttonDeleteROI);
        QWidget::setTabOrder(buttonDeleteROI, buttonAddBackground);
        QWidget::setTabOrder(buttonAddBackground, treeWidgetROIs);
        QWidget::setTabOrder(treeWidgetROIs, textBoxROIName);
        QWidget::setTabOrder(textBoxROIName, textBoxROIX);
        QWidget::setTabOrder(textBoxROIX, textBoxROIY);
        QWidget::setTabOrder(textBoxROIY, textBoxROIRadius);
        QWidget::setTabOrder(textBoxROIRadius, graphicsView);

        menuBar->addAction(menuQuickChis->menuAction());

        retranslateUi(QuickChipsMainWindow);

        QMetaObject::connectSlotsByName(QuickChipsMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *QuickChipsMainWindow)
    {
        QuickChipsMainWindow->setWindowTitle(QApplication::translate("QuickChipsMainWindow", "QuickChips", 0));
        groupBox->setTitle(QApplication::translate("QuickChipsMainWindow", "Camera", 0));
        buttonCapture->setText(QApplication::translate("QuickChipsMainWindow", "Capture", 0));
        label->setText(QApplication::translate("QuickChipsMainWindow", "Exposure", 0));
        buttonExposureSet->setText(QApplication::translate("QuickChipsMainWindow", "Set", 0));
        buttonExposureGet->setText(QApplication::translate("QuickChipsMainWindow", "Get", 0));
        buttonCaptureDark->setText(QApplication::translate("QuickChipsMainWindow", "Capture Dark", 0));
        buttonClearDark->setText(QApplication::translate("QuickChipsMainWindow", "Clear Dark", 0));
        label_6->setText(QApplication::translate("QuickChipsMainWindow", "Update Rate (ms)", 0));
        buttonCameraTickRateGet->setText(QApplication::translate("QuickChipsMainWindow", "Get", 0));
        buttonCameraTickRateSet->setText(QApplication::translate("QuickChipsMainWindow", "Set", 0));
        groupBox_2->setTitle(QApplication::translate("QuickChipsMainWindow", "Bluetooth", 0));
        buttonBTSend->setText(QApplication::translate("QuickChipsMainWindow", "BT Send", 0));
        label_2->setText(QApplication::translate("QuickChipsMainWindow", "BT DBG", 0));
        groupBox_3->setTitle(QApplication::translate("QuickChipsMainWindow", "Instrument", 0));
        buttonLED->setText(QApplication::translate("QuickChipsMainWindow", "Illumination (off) [00.00C] (no connection)", 0));
        buttonHeater->setText(QApplication::translate("QuickChipsMainWindow", "Heater (off) [00.00C] (no connection)", 0));
        groupBox_5->setTitle(QApplication::translate("QuickChipsMainWindow", "ROI Manager", 0));
        groupBox_6->setTitle(QApplication::translate("QuickChipsMainWindow", "Modify", 0));
        label_4->setText(QApplication::translate("QuickChipsMainWindow", "Y", 0));
        label_5->setText(QApplication::translate("QuickChipsMainWindow", "Radius", 0));
        label_3->setText(QApplication::translate("QuickChipsMainWindow", "X", 0));
        textBoxROIName->setText(QString());
        label_9->setText(QApplication::translate("QuickChipsMainWindow", "Name", 0));
        groupBox_7->setTitle(QApplication::translate("QuickChipsMainWindow", "Add and Delete", 0));
        buttonDeleteROI->setText(QApplication::translate("QuickChipsMainWindow", "Delete", 0));
        buttonAddBackground->setText(QApplication::translate("QuickChipsMainWindow", "+Background", 0));
        buttonAddSignal->setText(QApplication::translate("QuickChipsMainWindow", "+Signal", 0));
        buttonAddROI->setText(QApplication::translate("QuickChipsMainWindow", "+ROI", 0));
        groupBox_8->setTitle(QApplication::translate("QuickChipsMainWindow", "Results", 0));
        groupBox_9->setTitle(QApplication::translate("QuickChipsMainWindow", "Assay", 0));
        buttonStartProcess->setText(QApplication::translate("QuickChipsMainWindow", "Start Assay", 0));
        buttonReloadProcess->setText(QApplication::translate("QuickChipsMainWindow", "Reload Instructions", 0));
        labelAssayName->setText(QApplication::translate("QuickChipsMainWindow", "Assay Name", 0));
        groupBox_4->setTitle(QApplication::translate("QuickChipsMainWindow", "Debug", 0));
        buttonDebugB->setText(QApplication::translate("QuickChipsMainWindow", "Instrument Display Toggle", 0));
        buttonDebugA->setText(QApplication::translate("QuickChipsMainWindow", "Debug A", 0));
        menuQuickChis->setTitle(QApplication::translate("QuickChipsMainWindow", "QuickChips", 0));
    } // retranslateUi

};

namespace Ui {
    class QuickChipsMainWindow: public Ui_QuickChipsMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUICKCHIPSMAINWINDOW_H
