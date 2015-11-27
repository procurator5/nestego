#ifndef VLIBFANNCLASS_H
#define VLIBFANNCLASS_H

#include <QObject>
#include <QByteArray>
#include "fann.h"
#include "vtrainthread.h"
#include "vibuffer.h"

class vLibFannClass : public QObject
{
    Q_OBJECT
public:
    explicit vLibFannClass(unsigned int n_input,
                           unsigned int n_layers = 3,
                           unsigned int num_neurons_hidden=3,
                           fann_activationfunc_enum activate_function = FANN_SIGMOID_SYMMETRIC);

    void updateFann(unsigned int num_input,
                    unsigned int num_layers = 3,
                    unsigned int num_neurons_hidden=3,
                    fann_activationfunc_enum activate_function = FANN_SIGMOID_SYMMETRIC);
    void setNumInput(unsigned int num_input);

    /**
     * @brief Установить функцию активации нейронной сети
     * для внешнего и скрытого слоев (одновременно)
     * @param fann_activationfunc_enum activate_function тип функции активации
     */
    void setActivationFunction(fann_activationfunc_enum activate_function);

    /**
     * @brief Возвращает функцию активации нейронной сети для 0-го слоя 0-го нейрона
     * @return fann_activationfunc_enum
     */
    fann_activationfunc_enum getActivationFunction();

    void setNumLayers(int num_layers);
    int getNumLayers();
    /**
     * @brief Тренировка НС 1 шаг
     * @param QByteArray input Входные данные
     * @param QByteArray diff Ожидаемое различие между входными данными и
     * блоком до модификации НС
     * @return Результат обучения
     */
    int trainFannOneStep(QByteArray input, QByteArray diff);

    /**
     * @brief Тренировка НС 1 шаг
     * @param QByteArray input
     * @param int need_result
     * @return
     */
    int trainFannOneStep(QByteArray input, int need_result);

    /**
     * @brief Вычисление значения НС
     * @param input Входные параметры
     * @return Значение НС (int)
     */
    int calculate(QByteArray input);

    /**
     * @brief Возвращает результаты работы НС на последнем шаге
     * @return
     */
    train_result lastStep();

    void startTrainThread(vIBuffer* buffer);
    void startTrainThread();
    bool initThread(vIBuffer* buffer);
    QList <train_result>& getTrainResults(){
        if(this->thread == NULL){
            return trains;
        }

        trains.append(thread->getTrainResults());

    };

    void setTrainResult(struct train_result step);

    /**
     * @brief saveFann сохранение в файле настроек НС
     * @param filename имя файла
     * @return результат сохранения
     */
    bool saveFann(QString filename);

    /**
     * @brief loadFannFromFile загрузка НС из файла настроек
     * @param filename имя файла
     * @return результат загрузки
     */
    bool loadFannFromFile(QString filename);


signals:

public slots:

private:
    int input;
    struct fann* neural;
    vTrainThread *thread;
    QList <train_result> trains;
};

#endif // VLIBFANNCLASS_H
