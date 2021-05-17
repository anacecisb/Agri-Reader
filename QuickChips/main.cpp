#include "quickchipsmainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QLoggingCategory::setFilterRules()
    QuickChipsMainWindow w;
    w.show();

//    Camera camera;
//    camera.show();

    return a.exec();
}
