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
/*        if(!sql.exec("drop table if exists stego_source;"))
            emit databaseError(sql.lastError().text());
        if(!sql.exec("drop table if exists train_results;"))
            emit databaseError(sql.lastError().text());
        if(!sql.exec("drop table if exists node_relations;"))
            emit databaseError(sql.lastError().text());
        if(!sql.exec("drop table if exists nodes;"))
            emit databaseError(sql.lastError().text());
        if(!sql.exec("drop table if exists node_types;"))
            emit databaseError(sql.lastError().text());
        if(!sql.exec(trainTableSql))
            emit databaseError(sql.lastError().text());
        if(!sql.exec(stegoSourceTableSql))
            emit databaseError(sql.lastError().text());
        if(!sql.exec(stegoNodeTypeTable))
            emit databaseError(sql.lastError().text());
        if(!sql.exec(stegoNodesTableSql))
            emit databaseError(sql.lastError().text());
        if(!sql.exec(stegoNodeRelationTable))
            emit databaseError(sql.lastError().text());

        //Добавляем "Значения по умолчанию"
        sql.exec("INSERT INTO stego_source VALUES('type', 'program');");
        sql.exec("INSERT INTO stego_source VALUES('no_stego', 'undef');");
        sql.exec("INSERT INTO stego_source VALUES('command', 'undef');");
        sql.exec("INSERT INTO stego_source VALUES('cashe', 'false');");

        sql.exec("insert into node_types values(1, 'buffer');");
        sql.exec("insert into node_types values(2, 'fann');");
        sql.exec("insert into node_types values(3, 'brain');");

        sql.exec("insert into nodes (node_name, node_type_id, x, y)values('Brain', 3, 0, 0);");
*/
    }else{
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

int vProject::saveNode(QString node_name, QString type, int x, int y){
    QSqlQuery query;
    query.prepare("insert into nodes ( node_name, node_type_id, x, y )\
               select :node_name, node_type_id, :x, :y from \
                      node_types where node_type_name = :type;");
    query.bindValue(":node_name", node_name);
    query.bindValue(":x", x);
    query.bindValue(":y", y);
    query.bindValue(":type", type);
    if(!query.exec()){
        emit databaseError(query.lastError().text());
        return -1;
    }
    return query.lastInsertId().toInt();
}

int vProject::saveBuffer(QString node_name, int buffer_size, int x, int y){
    int rowid = saveNode(node_name, "buffer", x,y);
    if(rowid>0){
        QSqlQuery query;
        query.prepare("update nodes set node_data=:data where node_id=:rowid;");
        query.bindValue(":data",  buffer_size);
        query.bindValue(":rowid", rowid);
        if(!query.exec()){
            emit databaseError(query.lastError().text());
            deleteNode(node_name);
            return -1;
        }
    }
    return rowid;
}

bool vProject::deleteNode(QString node_name){
    QSqlQuery query;
    query.prepare("delete from nodes where node_name=:node_name;");
    query.bindValue(":node_name", node_name);
    if(!query.exec()){
        emit databaseError(query.lastError().text());
        return false;
    }
    return false;
}

bool vProject::moveNode(QString node_name, int x, int y){
    QSqlQuery query;
    query.prepare("update nodes set x=:x, y=:y where node_name=:node_name;");
    query.bindValue(":x", x);
    query.bindValue(":y", y);
    query.bindValue(":node_name", node_name);
    if(!query.exec()){
        emit databaseError(query.lastError().text());
        return false;
    }
    return false;
}

bool vProject::getBrainCoords(int &x, int &y){
    //Получаем координаты brain
    x=0;
    y=0;
    QSqlQuery query;
    query.prepare("select x, y from nodes where node_type_id=3;");
    if(!query.exec()){
        emit databaseError(query.lastError().text());
        return false;
    }
    if(!query.next()){
        query.prepare("insert into nodes ( node_name, node_type_id, x, y )\
                   values('Brain', 3,0,0)");
        if(!query.exec()){
            emit databaseError(query.lastError().text());
            return false;
        }
        return false;
    }

    x=query.value(0).toInt();
    x=query.value(1).toInt();
    return true;
}


QList <QString> vProject::getBuffers(){
    QList <QString> listNodes;
    QSqlQuery query;
    query.prepare("select node_name from nodes where node_type_id=1;");
    if(!query.exec()){
        emit databaseError(query.lastError().text());
        return listNodes;
    }

    while(query.next()){
        listNodes.append(query.value(0).toString());
    }
    return listNodes;
}

int vProject::getBufferSize(QString buffer_name){
    QSqlQuery query;
    query.prepare("select node_data from nodes where node_name=:name;");
    query.bindValue(":name", buffer_name);
    if(!query.exec()){
        emit databaseError(query.lastError().text());
        return 0;
    }
    if(!query.next())
        return 0;

    return query.value(0).toInt();
}
