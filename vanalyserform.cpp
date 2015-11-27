#include "vanalyserform.h"
#include "ui_vanalyserform.h"
#include "vlibfannclass.h"
#include "buffer.h"

vAnalyserForm::vAnalyserForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::vAnalyserForm), input_size(0)
{
    ui->setupUi(this);
    if(ui->tabWidget->tabText(1) == "FANN")
            ui->tabWidget->removeTab(1);
    ui->groupBox_2->hide();
    ui->doubleSpinBox->hide();
    ui->label_8->hide();

    AcceptButtonClicked = false;
    ui->treeWidget_2->setColumnWidth(0, 350);
}

vAnalyserForm::~vAnalyserForm()
{
    delete ui;
}

void vAnalyserForm::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "libFANN"){
        ui->tabWidget->insertTab(1, ui->tab_fann, QString ("FANN"));
        ui->groupBox_2->show();
    }else if(ui->tabWidget->tabText(1) == "FANN"){
        ui->tabWidget->removeTab(1);
        ui->groupBox_2->hide();
    }
}

Analyse* vAnalyserForm::getAnalyse(QGraphicsView* view){

    if(!AcceptButtonClicked)
        return NULL;
    if(ui->lineEdit->text().isEmpty())
        return NULL;

    Analyse* an = new Analyse(view);
    an->setName(ui->lineEdit->text());

    vLibFannClass * fann = an->getLibFann();
    if(fann == NULL){
        return an;

    }

    int num_input = an->getNumInput();
    fann->updateFann(num_input, ui->spinBox_3->value(),
                     ui->spinBox_2->value(),
                     (fann_activationfunc_enum) ui->comboBox_3->currentIndex());
    return an;
}

void vAnalyserForm::on_verticalSlider_sliderMoved(int position)
{
    ui->spinBox_2->setValue(position);
}

void vAnalyserForm::on_horizontalSlider_sliderMoved(int position)
{
    ui->spinBox_3->setValue(position);
}

void vAnalyserForm::on_spinBox_2_valueChanged(int arg1)
{
    ui->verticalSlider->setValue(arg1);
}

void vAnalyserForm::on_spinBox_3_valueChanged(int arg1)
{
    ui->horizontalSlider->setValue(arg1);
}

void vAnalyserForm::on_buttonBox_accepted()
{
    AcceptButtonClicked = true;
}

void vAnalyserForm::setAnalyseName(QString name){
    ui->lineEdit->setText(name);
}

QString vAnalyserForm::analyseName(){
    return ui->lineEdit->text();
}

void vAnalyserForm::setLibFannParametrs(int layers, int hidden_neurons,
                         fann_activationfunc_enum func){
    ui->comboBox->setCurrentIndex(2);
    ui->spinBox_2->setValue(hidden_neurons);
    ui->verticalSlider->setValue(hidden_neurons);
    ui->spinBox_3->setValue(layers);
    ui->horizontalSlider->setValue(layers);
    ui->comboBox_3->setCurrentIndex((int)func);
}

#include <QDebug>

void vAnalyserForm::setDataSource(QList<Edge *> edgeList){
    input_size = 0;
    for(QList<Edge*>::iterator it = edgeList.begin(); it != edgeList.end(); ++it){
        Buffer* buffer = dynamic_cast <Buffer*> ((*it)->sourceNode());
        if(buffer != NULL){
            QStringList top;
            top.append(QString("buffer(%1)").arg(buffer->getBufferSize()));
            QTreeWidgetItem* b = new QTreeWidgetItem(top);
            ui->treeWidget->addTopLevelItem(b);
            QByteArray buf = buffer->buffer();
            for(QByteArray::iterator it = buf.begin(); it!= buf.end(); ++it){
                QStringList data;
                data.append(QString("%1(%2)").arg((char)(*it)).arg((int)*it));
                QTreeWidgetItem* element = new QTreeWidgetItem(b,
                                                               data);
                b->addChild(element);
            }

            input_size+=buffer->getBufferSize();
        }

    }
    ui->treeWidget->expandAll();
}

void vAnalyserForm::on_comboBox_5_activated(int index)
{

}

void vAnalyserForm::on_comboBox_5_currentIndexChanged(int index)
{
    if(index ==0 ){
        ui->doubleSpinBox->hide();
        ui->label_8->hide();
    }else{
        ui->doubleSpinBox->show();
        ui->label_8->show();
    }
}

int vAnalyserForm::numLayers(){
    return ui->spinBox_3->value();
}

fann_activationfunc_enum vAnalyserForm::outputActivationFunction(){
    return (fann_activationfunc_enum) ui->comboBox_3->currentIndex();
}

void vAnalyserForm::setOutput(int before, int after, int need){
    QTreeWidgetItem *top = ui->treeWidget_2->topLevelItem(0);
    QStringList beforeData;
    beforeData<<QString::fromLocal8Bit("Значение НС до обучения")<<QString("%1").arg(before);
    QTreeWidgetItem* beforeItem = new QTreeWidgetItem(top, beforeData);
    top->addChild(beforeItem);
    QStringList afterData;
    afterData<<QString::fromLocal8Bit("Значение НС после обучения")<<QString("%1").arg(after);
    QTreeWidgetItem* afterItem = new QTreeWidgetItem(top, afterData);
    top->addChild(afterItem);
    QStringList needData;
    needData<<QString::fromLocal8Bit("Ожидаемое значение НС")<<QString("%1").arg(need);
    QTreeWidgetItem* needItem = new QTreeWidgetItem(top, needData);
    top->addChild(needItem);

    QString error1 = (need == 0 && before != 0) ?  QString::fromLocal8Bit("ДА") : QString::fromLocal8Bit("НЕТ");
    QString error2 = (need != 0 && before == 0) ? QString::fromLocal8Bit("ДА") : QString::fromLocal8Bit("НЕТ");

    QStringList error1Data;
    error1Data<<QString::fromLocal8Bit("Ложное срабатывание")<<error1;
    QTreeWidgetItem* error1Item = new QTreeWidgetItem(top, error1Data);
    top->addChild(error1Item);

    QStringList error2Data;
    error2Data<<QString::fromLocal8Bit("Пропуск блока")<<error2;
    QTreeWidgetItem* error2Item = new QTreeWidgetItem(top, error2Data);
    top->addChild(error2Item);

    ui->treeWidget_2->expandAll();
    ui->treeWidget_2->adjustSize();

}
