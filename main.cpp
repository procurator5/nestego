#include "mainwindow.h"
#include <QApplication>

#include "csignal.h"
#include "vproject.h"

CSignal *signal = new CSignal;
vProject *currentProject =new vProject;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("org");
    QCoreApplication::setApplicationName("nestego");
    MainWindow w;
    w.show();

    return a.exec();
    return 0;
}
