#include "mainwindow.h"
#include <QApplication>

#include "csignal.h"

CSignal *signal = new CSignal;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("org");
    QCoreApplication::setApplicationName("nestego");
    MainWindow w;
    w.show();

    return a.exec();
}
