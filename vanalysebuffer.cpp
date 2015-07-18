#include "vanalysebuffer.h"
#include "ui_vanalysebuffer.h"

#include <QDir>
#include <QFileInfoList>
#include <QProcess>

#include <QTime>
#include <QDebug>
#include <QPainter>

vAnalyseBuffer::vAnalyseBuffer( ) :
    QWidget(),
    ui(new Ui::vAnalyseBuffer)
{
    ui->setupUi(this);
    buffer_size=256;
}

vAnalyseBuffer::~vAnalyseBuffer()
{
    delete ui;
}

bool vAnalyseBuffer::setFile(){

    forig.close();
    fstego.close();
//Открываем пустой контейнер
    QDir dir(orig);
    QFileInfoList fl = dir.entryInfoList(QDir::Files);
        if(fl.size() == 0 )
            return false;

        QTime midnight(0,0,0);
        qsrand(midnight.secsTo(QTime::currentTime()));
        int pos = qrand() % fl.size();

        forig.setFileName(fl.at(pos).absoluteFilePath());
        if(!forig.open(QIODevice::ReadOnly))
            return false;
        //Открываем/создаем стегоконтейнер
        if(type == tExample){
            fstego.setFileName(stego + "/" +fl.at(pos).fileName());
            if(fstego.open(QIODevice::ReadOnly))
                return false;
        }else if(type == tCommand){
            if(QFile::exists(cashe_folder + "/" + fl.at(pos).fileName())&& cashing==true){

            }else if(stego.contains("%2")){
                QString command = QString(stego).arg(fl.at(pos).absoluteFilePath()).arg(cashe_folder + "/" + fl.at(pos).fileName());
                if(QProcess::execute(command)!=0)
                    return false;

            }else if(stego.contains("%1")){
                QFile::copy(fl.at(pos).absoluteFilePath(), cashe_folder + "/" + fl.at(pos).fileName());
                QString command = QString(stego).arg(cashe_folder + "/" + fl.at(pos).fileName());
                if(QProcess::execute(command)!=0)
                    return false;
            }else
                return false;

            fstego.setFileName(cashe_folder + "/" + fl.at(pos).fileName());
            if(fstego.open(QIODevice::ReadOnly))
                return false;
        }else
            return false;

        return true;
}

void vAnalyseBuffer::loadConfigurationFromWidget(QTreeWidget *infoWidget){

    //Отпределяем тип контрольного примера
    QTreeWidgetItem * controlItem;
    if(infoWidget->topLevelItem(0)->text(2)=="X"){
        type = tCommand;
        controlItem = infoWidget->topLevelItem(0);
        cashe_folder =  controlItem->child(3)->text(1);
        cashing = QVariant(controlItem->child(2)->text(1)).toBool();

    }else{
        type = tExample;
        controlItem= infoWidget->topLevelItem(1);

    }
    orig = controlItem->child(0)->text(1);
    stego = controlItem->child(1)->text(1);

    QTreeWidgetItem * item = infoWidget->topLevelItem(2);
    buffer_size = item->child(0)->text(1).toInt();

}

void vAnalyseBuffer::loadBuffer(){
    if(!fstego.isOpen()){
        setFile();
    }
    QByteArray orig_buff = forig.read(buffer_size);
    stego_buff = fstego.read(buffer_size);

    if(stego_buff.size()<buffer_size)
        fstego.close();


    for(int i=0; i<buffer_size; i++)
        diff[i]=orig_buff[i]-stego_buff[i];

    repaint();
}

void vAnalyseBuffer::paintEvent (QPaintEvent *e){
    qreal step = height()/buffer_size;
    QPainter *painter = new QPainter (this);
    painter->eraseRect(rect());
    painter->end ();
    painter->begin(this);
    painter->setPen(Qt::red);

    for(int i=0; i<buffer_size; i++)
        if(diff[i]!= '\0'){
    //Отрисовываем различия между буферами
            painter->drawRect(QRectF(QPoint(0, i*step), QSize(width(), step)));
        }
    painter->end ();
    delete painter;
}
