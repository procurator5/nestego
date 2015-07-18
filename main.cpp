#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("org");
    QCoreApplication::setApplicationName("nestego");
    MainWindow w;
    w.show();

    return a.exec();
}
