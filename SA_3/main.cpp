#include "mainwindow.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    //qDebug()<<erfc(0.5);;
    //return  0;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
