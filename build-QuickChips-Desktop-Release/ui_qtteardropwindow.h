/********************************************************************************
** Form generated from reading UI file 'qtteardropwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTTEARDROPWINDOW_H
#define UI_QTTEARDROPWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>

QT_BEGIN_NAMESPACE

class Ui_QTTeardropWindow
{
public:
    QListWidget *listWidget;

    void setupUi(QDialog *QTTeardropWindow)
    {
        if (QTTeardropWindow->objectName().isEmpty())
            QTTeardropWindow->setObjectName(QStringLiteral("QTTeardropWindow"));
        QTTeardropWindow->resize(160, 128);
        listWidget = new QListWidget(QTTeardropWindow);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(0, 0, 160, 128));

        retranslateUi(QTTeardropWindow);

        QMetaObject::connectSlotsByName(QTTeardropWindow);
    } // setupUi

    void retranslateUi(QDialog *QTTeardropWindow)
    {
        QTTeardropWindow->setWindowTitle(QApplication::translate("QTTeardropWindow", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class QTTeardropWindow: public Ui_QTTeardropWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTTEARDROPWINDOW_H
