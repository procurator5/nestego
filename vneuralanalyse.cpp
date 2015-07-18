#include "vneuralanalyse.h"
#include "ui_vneuralanalyse.h"

vNeuralAnalyse::vNeuralAnalyse(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vNeuralAnalyse)
{
    ui->setupUi(this);
}

vNeuralAnalyse::~vNeuralAnalyse()
{
    delete ui;
}
