#include "vchoosedirectory.h"
#include "ui_vchoosedirectory.h"
#include <QFileDialog>

vChooseDirectory::vChooseDirectory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vChooseDirectory)
{
    ui->setupUi(this);
    ui->lineEdit->setFocus();
}

vChooseDirectory::~vChooseDirectory()
{
    delete ui;
}

void vChooseDirectory::on_toolButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QCoreApplication::applicationDirPath (),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->lineEdit->setText(dir);
}

QString vChooseDirectory::getText(){
    return ui->lineEdit->text();
}

void vChooseDirectory::setText(QString text){
    ui->lineEdit->setText(text);
}
