#include "mainwindow.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow *mainWin = new MainWindow;
    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsProxyWidget *proxy = scene->addWidget(mainWin);
    proxy->setRotation(90);  // 旋转90度
//    proxy->resize(3200, 1440);

    QGraphicsView *view = new QGraphicsView(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setWindowFlags(Qt::FramelessWindowHint);
    view->showFullScreen();  // 先全屏（但尺寸尚未最终生效）



    return a.exec();
}
