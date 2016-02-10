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

    if(!sql.next())
        return "";
    return sql.value(0).toString();
}

bool vProject::setSourceParam(QString key, QString value){
    //Проверяем, есть ли параметр в базе
    QSqlQuery query;
    query.prepare("select count(*) from stego_source where key=:key;");
    query.bindValue(":key", key);
    if(!query.exec()){
        emit databaseError(query.lastError().text());
        return false;
    }
    if(!query.next())
        return false;

    int value_count=query.value(0).toInt();
    if(value_count == 0){
        query.prepare("INSERT INTO stego_source VALUES(:key, :value);");
        query.bindValue(":key", key);
        query.bindValue(":value", value);
        if(!query.exec()){
            emit databaseError(query.lastError().text());
            return false;
        }
    }else if(value_count == 1){
        query.prepare("UPDATE stego_source set value=:value where key=:key;");
        query.bindValue(":key", key);
        query.bindValue(":value", value);
        if(!query.exec()){
            emit databaseError(query.lastError().text());
            return false;
        }
    }else{
        query.prepare("DELETE FROM stego_source WHERE key=:key;");
        query.bindValue(":key", key);
        if(!query.exec()){
            emit databaseError(query.lastError().text());
            return false;
        }

        query.prepare("INSERT INTO stego_source VALUES(:key, :value);");
        query.bindValue(":key", key);
        query.bindValue(":value", value);
        if(!query.exec()){
            emit databaseError(query.lastError().text());
            return false;
        }

    }
    return true;
}
