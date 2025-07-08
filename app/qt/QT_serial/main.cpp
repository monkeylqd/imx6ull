#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);
    MainWindow w;
    w.show();
    return a.exec();
}
