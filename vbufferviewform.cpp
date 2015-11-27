#include "vbufferviewform.h"
#include "ui_vbufferviewform.h"

vBufferViewForm::vBufferViewForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::vBufferViewForm)
{
    ui->setupUi(this);
}

vBufferViewForm::~vBufferViewForm()
{
    delete ui;
}

void vBufferViewForm::setWindowTitle(const QString &title){
    ui->label->setText(title);
    QDialog::setWindowTitle(title);
}

QTableWidget* vBufferViewForm::getTableWidget(){
    return ui->tableWidget;
}
