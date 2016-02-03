#include "vproject.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

vProject::vProject(QObject *parent) :
    QObject(parent)
{
    connect(this,SIGNAL(databaseError(QString)),this,SLOT(debugDB(QString)));
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("undef.sqlite");
    //Разметка БД
    if (db.open()) {
        QSqlQuery sql;
        if(!sql.exec(trainTableSql))
            emit databaseError(sql.lastError().text());
        if(!sql.exec(stegoSourceTableSql))
            emit databaseError(sql.lastError().text());

        //Добавляем "Значения по умолчанию"
        sql.exec("INSERT INTO stego_source VALUES('type', 'program');");
        sql.exec("INSERT INTO stego_source VALUES('no_stego', 'undef');");
        sql.exec("INSERT INTO stego_source VALUES('command', 'undef');");
        sql.exec("INSERT INTO stego_source VALUES('cashe', 'false');");

    }else{
        QSqlError err = db.lastError();
        qDebug()<<err.ConnectionError;
        emit databaseError(db.lastError().text());
    }
}

vProject::~vProject(){
    db.close();
}

void vProject::debugDB(QString err){
    qDebug()<<err;
}
