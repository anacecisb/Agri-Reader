#ifndef QTTEARDROPWINDOW_H
#define QTTEARDROPWINDOW_H

#include <QDialog>
#include <QtWidgets>

#include <QListWidgetItem>

namespace Ui {
class QTTeardropWindow;
}

class QTTeardropWindow : public QDialog
{
    Q_OBJECT

public:
    explicit QTTeardropWindow(QWidget *parent = 0);
    ~QTTeardropWindow();

    void addItemToList( QString itemName );
    void clearList();
    QString currentMenuName;
    void selectFirstListItem();
    void selectLastListItem();

    QListWidget* listWidget;

signals:
    void menuItemWasSelected( QString itemName );
    void leftArrowPressed();

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::QTTeardropWindow *ui;

};

#endif // QTTEARDROPWINDOW_H
