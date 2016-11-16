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

    /**
     * @brief Сохраняет информацию о добавленном буфере (исходные данные для анализа)
     * @param node_name имя ноды (уникальное?)
     * @param buffer_size размерность буфера
     * @param x - расположение на экране по х
     * @param y - расположение на экране по y
     * @return id добавленной записи
     */
    int saveBuffer(QString node_name, int buffer_size, int x = -50, int y = -50);

    /**
     * @brief deleteNode Удаление ноды
     * @param node_name Имя ноды
     * @return Результат удаления ноды
     */
    bool deleteNode(QString node_name);

    /**
     * @brief Изменении координат ноды в БД при ее перемещении
     * @param node_name имя ноды (уникальное?)
     * @param x - расположение на экране по х
     * @param y - расположение на экране по y
     * @return
     */
    bool moveNode(QString node_name, int x, int y);

    /**
     * @brief Изменении координат ноды в БД при ее перемещении
     * @param x - расположение на экране по х
     * @param y - расположение на экране по y
     */
    bool getBrainCoords(int &x, int &y);

    /**
     * @brief Получение списков нод
     * @return Список наименований буфферов обмена
     */
    QList <QString> getBuffers();

    /**
     * @brief Получение размера буфера
     * @param buffer_name - имя буфера
     * @return размер буфера
     */
    int getBufferSize(QString buffer_name);

signals:
    /**
     * @brief Сигнал при возникновении ошибок работы с БД
     */
    void databaseError(QString);

public slots:
    void debugDB(QString);

protected:
    /**
     * @brief Сохраняет информацию об очередном блоке сети
     * @param node_name имя ноды (уникальное?)
     * @param type - тип ноды: буфер, нейросеть, выходной параметр
     * @param x - расположение на экране по х
     * @param y - расположение на экране по y
     * @return id добавленной записи
     */
    int saveNode(QString node_name, QString type, int x, int y);

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
            node_type_id INTEGER NOT NULL DEFAULT (1),\
            x INTEGER NOT NULL DEFAULT (0),\
            y INTEGER NOT NULL DEFAULT (0),\
            node_data BLOB\
        );";

    const QString stegoNodeRelationTable = ("CREATE TABLE node_relations (\
            node_relation_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\
            node_relation_node1_id INTEGER NOT NULL,\
            node_relation_node2_id INTEGER NOT NULL,\
            weigth real NOT NULL DEFAULT(1.0)\
        );" );

    const QString stegoNodeTypeTable = ("CREATE TABLE node_types ( \
            node_type_id INTEGER PRIMARY KEY, \
            node_type_name TEXT \
        );" );

};

#endif // VPROJECT_H
