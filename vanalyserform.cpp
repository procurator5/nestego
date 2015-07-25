#include "vanalyserform.h"
#include "ui_vanalyserform.h"

vAnalyserForm::vAnalyserForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::vAnalyserForm)
{
    ui->setupUi(this);
}

vAnalyserForm::~vAnalyserForm()
{
    delete ui;
}
