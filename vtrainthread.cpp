#include "vtrainthread.h"
#include "csignal.h"
#include <QDebug>

extern CSignal *signal;

vTrainThread::vTrainThread(QObject *parent) :
    QThread(parent)
{
    steps = 100;
}

void vTrainThread::run(){
    results.clear();
    const unsigned int num_input = fann_get_num_input(neural);
    //Готовим выборку для шага обучения

    float *data =new float[num_input];
    for(int i = 0;i<steps; i++ ){
        struct train_result step;
        memset(&step, 0, sizeof(train_result));

        signal->logMessage(DEBUG, QString("Step%1").arg(i));
        memset(data, 0, num_input*sizeof(float));
        float desired_output = 0.0;

        QByteArray input = buffer->getBuffer(num_input);
        QByteArray diff = buffer->getDiff(num_input);

        for(int i = 0; i<input.size(); i++){
            data[i] = static_cast<float>(input.at(i));
            desired_output += abs(static_cast<float>(diff.at(i)));
        }

        signal->logMessage(DEBUG, QString("   Diff Sum:     %1").arg(desired_output));

        desired_output /=255;

        step.need_result = desired_output;

        signal->logMessage(DEBUG, QString("   need output:  %1").arg(desired_output));
        float* var = fann_run(neural, data);

        step.output_before_train = *var;
        signal->logMessage(DEBUG, QString("   value before: %1").arg(*var));


        fann_train(neural, data, &desired_output);

        var = fann_run(neural, data);

        step.output_after_train = desired_output;

        signal->logMessage(DEBUG, QString("   value after:  %1").arg(*var));

        step.error1 = (step.need_result == 0 && step.output_before_train != 0) ?  true : false;

        step.error2 = (step.need_result != 0 && step.output_before_train == 0) ?  1 : 0;

        results.append(step);

    }

}
