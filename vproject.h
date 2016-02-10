#ifndef VPROJECT_H
#define VPROJECT_H

#include <QObject>
#include <QSqlDatabase>

/**
 * @brief Класс содержащий описание проекта nestego
 */

class vProject : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор по умолчанию (создаем пустой проект)
     * @param parent
     */
    explicit vProject(QObject *parent = 0);

    /**
      * @brief Деструктор
      * @detailed Удаляет соединение с БД
      */
    ~vProject();

    /**
     * @brief Получает параметры, определяющие параметры получения обучающей выборки
     * @param ключ, по которому надо получить значение. Варианты
     * >type
       >no_stego
       >command
       >cashe

     * @return Значение, соответствующее ключу
     */
    QString getSorceParam(QString key);
    /**
     * @brief Получает параметры, определяющие параметры получения обучающей выборки
     * @param key Ключ, значение которого модифицируем
     * @param valueт новое значение
     * @return получилось/нет
     */
    bool setSourceParam(QString key, QString value);

signals:
    /**
     * @brief Сигнал при возникновении ошибок работы с БД
     */
    void databaseError(QString);

public slots:
    void debugDB(QString);

protected:

private:
    QSqlDatabase db;
    const QString trainTableSql = "CREATE TABLE train_results(  \
                id integer PRIMARY KEY AUTOINCREMENT, \
                output_before_train real, \
                output_after_train real, \
                need_result real, \
                error1 boolean, \
                error2 boolean);";

    const QString stegoSourceTableSql = "CREATE TABLE stego_source(\
            key varchar(20) PRIMARY KEY NOT NULL, \
            value varchar(2000));";

    const QString stegoNodesTableSql = "CREATE TABLE nodes (\
            node_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\
            node_name TEXT NOT NULL,\
            node_type INTEGER NOT NULL DEFAULT (1),\
            x INTEGER NOT NULL DEFAULT (0),\
            y INTEGER NOT NULL DEFAULT (0),\
            node_data BLOB\
        );";

    const QString stegoNodeRelationTable = ("CREATE TABLE node_relations (\
            node_relation_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\
            node_relation_node1_id INTEGER,\
            node_relation_node2_id INTEGER \
        );" );

};

#endif // VPROJECT_H
