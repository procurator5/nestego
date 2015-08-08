#include "vlibfannclass.h"

vLibFannClass::vLibFannClass(unsigned int num_input,
                              unsigned int num_layers,
                              unsigned int num_neurons_hidden,
                              fann_activationfunc_enum activate_function) :
    QObject(), neural(NULL), thread(NULL)
{

    const unsigned int num_output = 1;

//Настройки нейронной стети, которые задаются соответствующими параметрами
//Из пользовательской формы

    neural = fann_create_standard(num_layers, num_input,
        num_neurons_hidden, num_output);

//Функция активации задается параметрами
    fann_set_activation_function_hidden(neural, activate_function);
    fann_set_activation_function_output(neural, activate_function);

    //Заполняем веса случайными цифрами
    fann_randomize_weights(neural, -0.1, 0.1);
}


void vLibFannClass::updateFann(unsigned int num_input,
                               unsigned int num_layers,
                               unsigned int num_neurons_hidden,
                               fann_activationfunc_enum activate_function){
    fann_destroy(neural);
    const unsigned int num_output = 1;

//Настройки нейронной стети, которые задаются соответствующими параметрами
//Из пользовательской формы

    neural = fann_create_standard(num_layers, num_input,
        num_neurons_hidden, num_output);

//Функция активации задается параметрами
    fann_set_activation_function_hidden(neural, activate_function);
    fann_set_activation_function_output(neural, activate_function);

    //Заполняем веса случайными цифрами
    fann_randomize_weights(neural, -0.1, 0.1);
}

void vLibFannClass::setNumInput(unsigned int num_input){
    unsigned int num_layers = fann_get_num_layers(neural);
    unsigned int num_neurons_hidden = 3;
    fann_activationfunc_enum activate_function = FANN_SIGMOID_SYMMETRIC;
    updateFann(num_input, num_layers, num_neurons_hidden, activate_function);

}

void vLibFannClass::setActivationFunction(fann_activationfunc_enum activate_function){
    const unsigned int num_input = fann_get_num_input(neural);
    unsigned int num_layers = fann_get_num_layers(neural);
    unsigned int num_neurons_hidden = 3;
    updateFann(num_input, num_layers, num_neurons_hidden, activate_function);

}

int vLibFannClass::trainFannOneStep(QByteArray input, QByteArray diff){
    const unsigned int num_input = fann_get_num_input(neural);
    //Готовим выборку для шага обучения

    float *data =new float[num_input];
    memset(data, 0, num_input*sizeof(float));
    float desired_output = 0.0;

    for(int i = 0; i<input.size(); i++){
        data[i] = static_cast<float>(input.at(i));
        desired_output += abs(static_cast<float>(diff.at(i)));
    }
    desired_output /=255;

    fann_train(neural, data, &desired_output);

    return 0;
}


bool vLibFannClass::initThread(vIBuffer* buffer){
    if(neural == NULL)
        return false;
    thread = new vTrainThread();
    thread->setFann(neural);
    thread->setBuffer(buffer);
    return true;
}

void vLibFannClass::startTrainThread(vIBuffer* buffer){
    initThread(buffer);
    thread->start();
}

void vLibFannClass::startTrainThread(){
    thread->start();
}

