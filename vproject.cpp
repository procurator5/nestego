#include "vproject.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
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
        if(!sql.exec("drop table if exists stego_source;"))
            emit databaseError(sql.lastError().text());
        if(!sql.exec("drop table if exists train_results;"))
            emit databaseError(sql.lastError().text());
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

QString vProject::getSorceParam(QString key){
    QSqlQuery sql;
    sql.prepare("SELECT value FROM stego_source where key=:key;");
    sql.bindValue(":key", key);
    if (!sql.exec()) {
        emit databaseError(sql.lastError().text());
        return "undef";
    }

    sql.next();
    return sql.value(0).toString();
}
