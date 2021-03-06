#ifndef VIBUFFER_H
#define VIBUFFER_H

#include <QObject>
#include <QFile>
#include <QByteArray>
#include <QTreeWidget>

class vIBuffer : public QObject
{
    Q_OBJECT
public:
    explicit vIBuffer(QObject *parent = 0);
    enum typeProject{
        tCommand,
        tExample
    };

     void loadConfigurationFromWidget(QTreeWidget *infoWidget);
    ~vIBuffer();

     QByteArray getDiff(int size);
     QByteArray getBuffer(int size);
     QByteArray getStegoBuffer(int size);
     QByteArray getOriginBuffer(int size);
public slots:
     /**
      * @brief Загрузить следующую порцию данных
      */
     void loadBuffer();
     void moveBuffer(int size);
signals:

public slots:

protected:

    /**
     * @brief Выбор и загрузка файла для анализа
     * @return успешно ДА/НЕТ
     */
    bool setFile();

    /**
      @brief Принцип формирования стегоконтейнера: контрольный пример или команда
     */
    typeProject type;

    /**
      @brief Папка с оригинальнымми (чистыми) сообщениями
     */
    QString orig;
    /**
      @brief Стегоконтейнеры. Могут принимать вид:
      >Папка с контейнерами (контрольный пример)
      >Командная строка
     */
    QString stego;
    /**
     @brief Папка кэша. Актуально для type == tCommand
      */
    QString cashe_folder;

    /**
     * @brief Использовать кэширование результататов?
     */

    bool cashing;

    /**
     * @brief Размер буфера
     */

    int buffer_size;
    long int seek;

    QFile forig;
    QFile fstego;

    QByteArray stego_buff;
    QByteArray origin;
    QByteArray diff;

private:
    void isBuf(int size);
};

#endif // VIBUFFER_H
