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
    void setActivationFunction(fann_activationfunc_enum activate_function);

    int trainFannOneStep(QByteArray input, QByteArray diff);
    void startTrainThread(vIBuffer* buffer);
    void startTrainThread();
    bool initThread(vIBuffer* buffer);
    QList <train_result>& getTrainResults(){
        if(this->thread == NULL){
            QList <train_result> empty;
            return empty;
        }
        return thread->getTrainResults();
    };


signals:

public slots:

private:
    int input;
    struct fann* neural;
    vTrainThread *thread;
};

#endif // VLIBFANNCLASS_H
