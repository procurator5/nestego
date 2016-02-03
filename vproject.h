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
            key varchar(20), \
            value varchar(2000));";

};

#endif // VPROJECT_H
