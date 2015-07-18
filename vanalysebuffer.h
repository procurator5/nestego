#ifndef VANALYSEBUFFER_H
#define VANALYSEBUFFER_H

#include <QWidget>
#include <QTreeWidget>
#include <QFile>

namespace Ui {
class vAnalyseBuffer;
}
/**
 * @brief Класс хранитель и обработчик буфера обмена
 */

class vAnalyseBuffer : public QWidget
{
    Q_OBJECT

    enum typeProject{
        tCommand,
        tExample
    };

public:
    explicit vAnalyseBuffer();
    bool setFile();
    ~vAnalyseBuffer();
public slots:
    void loadBuffer();
    void loadConfigurationFromWidget(QTreeWidget *infoWidget);

protected:
    void paintEvent (QPaintEvent *e);

private:
    Ui::vAnalyseBuffer *ui;
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

    QFile forig;
    QFile fstego;

    QByteArray stego_buff;
    QByteArray diff;
};

#endif // VANALYSEBUFFER_H
