#include "qtteardropwindow.h"
#include "ui_qtteardropwindow.h"

QTTeardropWindow::QTTeardropWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QTTeardropWindow)
{
    ui->setupUi(this);

    listWidget = ui->listWidget;

    QPalette listPalette = ui->listWidget->palette();
    listPalette.setColor( QPalette::Highlight, QColor( 85, 195, 43 ) );
    ui->listWidget->setPalette( listPalette );
}

QTTeardropWindow::~QTTeardropWindow()
{
    delete ui;
}


void QTTeardropWindow::addItemToList( QString itemName )
{
    ui->listWidget->addItem( new QListWidgetItem( itemName ) );
}

void QTTeardropWindow::clearList()
{
    ui->listWidget->clear();
}

void QTTeardropWindow::selectFirstListItem()
{
    this->activateWindow();

    ui->listWidget->setFocus();
    ui->listWidget->setCurrentRow( 0 );
}

void QTTeardropWindow::selectLastListItem()
{
    this->activateWindow();

    ui->listWidget->setFocus();
    ui->listWidget->setCurrentRow( ui->listWidget->count()-1 );
}

void QTTeardropWindow::keyPressEvent( QKeyEvent* event )
{
    int key = event->key();

    if ( key == Qt::Key_Up )
    {
        qDebug("tdw up");
    }
    else if ( key == Qt::Key_Right )
    {
        QString itemName = ui->listWidget->currentItem()->text();
        emit menuItemWasSelected( itemName );
    }
    else if ( key == Qt::Key_Down )
    {
        qDebug("tdw down");
    }
    else if ( key == Qt::Key_Left )
    {
        emit menuItemWasSelected( "Back" );
    }

}
