#include "vproject.h"
#include <QSqlDatabase>

vProject::vProject(QObject *parent) :
    QObject(parent)
{
    *db = QSqlDatabase::addDatabase("QSQLITE");
    db->setDatabaseName("undef.sqlite");
}

vProject::~vProject(){
    db->close();
    delete db;
}
