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
    //Определяем список нейронов по слоям
    unsigned int *layers_detailed = new unsigned int[num_layers +2];
    layers_detailed[0] = num_input;
    for(int i = 0; i <num_layers; i++)
        layers_detailed[i+1] = num_neurons_hidden;

    layers_detailed[num_layers+1] = 1;

    neural = fann_create_standard_array(num_layers, layers_detailed);

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

    //Определяем список нейронов по слоям
    unsigned int *layers_detailed = new unsigned int[num_layers +2];
    layers_detailed[0] = num_input;
    for(int i = 0; i <num_layers; i++)
        layers_detailed[i+1] = num_neurons_hidden;

    layers_detailed[num_layers+1] = 1;

    neural = fann_create_standard_array(num_layers, layers_detailed);

//Функция активации задается параметрами
    fann_set_activation_function_hidden(neural, activate_function);
    fann_set_activation_function_output(neural, activate_function);

    //Заполняем веса случайными цифрами
    fann_randomize_weights(neural, -0.1, 0.1);
}

void vLibFannClass::setNumInput(unsigned int num_input){
    unsigned int num_layers = fann_get_num_layers(neural);
    unsigned int num_neurons_hidden = 3;
    fann_activationfunc_enum activate_function = getActivationFunction();
    updateFann(num_input, num_layers, num_neurons_hidden, activate_function);

}

void vLibFannClass::setActivationFunction(fann_activationfunc_enum activate_function){
    const unsigned int num_input = fann_get_num_input(neural);
    unsigned int num_layers = fann_get_num_layers(neural);
    unsigned int num_neurons_hidden = 3;
    updateFann(num_input, num_layers, num_neurons_hidden, activate_function);
}

fann_activationfunc_enum vLibFannClass::getActivationFunction(){

    if(neural == NULL)
        return FANN_SIGMOID_SYMMETRIC;
    return fann_get_activation_function(neural, 0, 0 );
}

int vLibFannClass::trainFannOneStep(QByteArray input, QByteArray diff){
    int desired_output = 0;

    for(int i = 0; i<input.size(); i++){
        desired_output += abs(diff.at(i));
    }

    return trainFannOneStep(input, desired_output);
}

int vLibFannClass::trainFannOneStep(QByteArray input, int result){
    const unsigned int num_input = fann_get_num_input(neural);
    //Готовим выборку для шага обучения

    float *data =new float[num_input];
    memset(data, 0, num_input*sizeof(float));

    for(int i = 0; i<input.size(); i++){
        data[i] = static_cast<float>(input.at(i));
    }
    float desired_output = (float)result/255;

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


void vLibFannClass::setNumLayers(int num_layers){
    const unsigned int num_input = input;
    unsigned int num_neurons_hidden = 3;
    fann_activationfunc_enum activate_function = getActivationFunction();

    updateFann(num_input, num_layers, num_neurons_hidden, activate_function);
}

int vLibFannClass::getNumLayers(){
    if(neural == NULL)
        return 1;
    return fann_get_num_layers(neural);
}

int vLibFannClass::calculate(QByteArray input){
    const unsigned int num_input = fann_get_num_input(neural);
    if(num_input<1)
        return INT_MIN;
    if(input.size() <1)
        return INT_MIN;

    //Готовим входные данные

    float *data =new float[num_input];
    memset(data, 0, num_input*sizeof(float));
//    float *desired_output = new float [fann_get_num_output(neural)];

    for(int i = 0; i<input.size(); i++){
        data[i] = static_cast<float>(input.at(i));
    }

    float *desired_output =  fann_run(neural, data);

    int result = (int)(*desired_output*255);

    delete [] data;
//    delete [] desired_output;

    return result;
}

void vLibFannClass::setTrainResult(train_result step){
    trains.append(step);
}

train_result vLibFannClass::lastStep(){
    if(trains.isEmpty()){
        train_result empty;
        memset(&empty, 0 , sizeof(struct train_result));
        return empty;
    }
    return trains.last();
}

bool vLibFannClass::saveFann(QString filename){
    return fann_save(neural, filename.toLocal8Bit().data()) == 0 ? true :false;
}

bool vLibFannClass::loadFannFromFile(QString filename){
    if(neural != NULL)
        fann_destroy(neural);
    neural = fann_create_from_file(filename.toLocal8Bit().data());
    return neural == NULL ? false :true;
}
