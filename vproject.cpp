#include "vproject.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

vProject::vProject(QObject *parent) :
    QObject(parent)
{
    *db = QSqlDatabase::addDatabase("QSQLITE");
    db->setDatabaseName("undef.sqlite");
    //Разметка БД
    if (!db->open()) {
        QSqlQuery sql;
        if(!sql.exec(trainTableSql))
            emit databaseError(db->lastError().text());
    }else
        emit databaseError(db->lastError().text());
    /*

    */
}

vProject::~vProject(){
    db->close();
    delete db;
}
