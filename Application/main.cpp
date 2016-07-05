#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    /* Create instance of QApplication to start application*/
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    /* Run application */
    return a.exec();
}
