#ifndef VTRAINTHREAD_H
#define VTRAINTHREAD_H

#include <QThread>
#include <QList>
#include "fann.h"

#include "vibuffer.h"

struct train_result{
    float output_before_train;
    float output_after_train;
    float need_result;
    bool error1;
    bool error2;
};

/**
 * @brief Класс для тренировки нейронной сети методом
 * обратного распространения ошибки
 */

class vTrainThread : public QThread
{
    Q_OBJECT
public:
    explicit vTrainThread(QObject *parent = 0);
    /**
     * @brief Функция, которая задает нейронную сеть, которую будут тренировать
     * @param struct fann* an структура, связанная с нейронной сетью из библиотеки libfann
     */
    void setFann(struct fann* an){
        neural = an;
    };
    void setBuffer(vIBuffer *buf){
        buffer = buf;
    }
    QList <train_result> & getTrainResults(){
        return results;
    };

signals:

public slots:

protected:
    virtual void run();

private:
    struct fann* neural;
    vIBuffer *buffer;
    int steps;
    QList <train_result> results;


};

#endif // VTRAINTHREAD_H
