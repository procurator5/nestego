#include "vabufferform.h"
#include "ui_vabufferform.h"

#include <QDebug>

vABufferForm::vABufferForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::vABufferForm)
{
    ui->setupUi(this);
}

vABufferForm::~vABufferForm()
{
    delete ui;
}

Buffer* vABufferForm::returnElement(QGraphicsView *view){

    Buffer* buf = new Buffer(view);
    buf->setBufferSize(ui->spinBox->value());
    QGraphicsScene *scene = view->scene();
    scene->addItem(buf);
    buf->setPos(-50, -50);
    return buf;
}

FeedBackBuffer* vABufferForm::returnFeedBackBuffer(QGraphicsView *view){

    FeedBackBuffer* buf = new FeedBackBuffer(view);
    buf->setBufferSize(ui->spinBox->value());
    QGraphicsScene *scene = view->scene();
    scene->addItem(buf);
    buf->setPos(-50, -50);
    return buf;
}
