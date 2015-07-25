#include "vedgedialog.h"
#include "ui_vedgedialog.h"

vEdgeDialog::vEdgeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::vEdgeDialog)
{
    ui->setupUi(this);
}

vEdgeDialog::~vEdgeDialog()
{
    delete ui;
}

void vEdgeDialog::loadNodesList(QMap<QString, Node *> map){
    elements = map;
    int i=0;
    for(QMap<QString, Node *>::iterator it = elements.begin(); it!=elements.end();++it){
        ui->comboBox->insertItem(i++, it.key());
        ui->comboBox_2->insertItem(i++, it.key());
    }

}

void vEdgeDialog::on_comboBox_currentIndexChanged(const QString &arg1)
{

}

Edge* vEdgeDialog::getEdge(){
    Node *src = elements.value(ui->comboBox->currentText());
    Node *dst = elements.value(ui->comboBox_2->currentText());
    return new Edge(src, dst);
}
