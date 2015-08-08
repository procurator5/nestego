#include "vibuffer.h"
#include <QDir>
#include <QTime>
#include <QProcess>

#include "csignal.h"
extern CSignal* signal;

vIBuffer::vIBuffer(QObject *parent) :
    QObject(parent)
{
}

vIBuffer::~vIBuffer()
{
}

bool vIBuffer::setFile(){

    forig.close();
    fstego.close();
//Открываем пустой контейнер
    QDir dir(orig);
    QFileInfoList fl = dir.entryInfoList(QDir::Files);
        if(fl.size() == 0 ){
            signal->logMessage(CRITICAL, QString::fromLocal8Bit("Директория с открытыми контейнерами пуста. Имя директории: %1").arg(orig));
            return false;
        }

        QTime midnight(0,0,0);
        qsrand(midnight.secsTo(QTime::currentTime()));
        int pos = qrand() % fl.size();

        forig.setFileName(fl.at(pos).absoluteFilePath());
        if(!forig.open(QIODevice::ReadOnly)){
            signal->logMessage(CRITICAL, QString::fromLocal8Bit("Недоступен открытый контейнер. Имя контейнера: %1").arg(fl.at(pos).absoluteFilePath()));
            return false;
        }
        //Открываем/создаем стегоконтейнер
        if(type == tExample){
            fstego.setFileName(stego + "/" +fl.at(pos).fileName());
            if(fstego.open(QIODevice::ReadOnly))
                return false;
        }else if(type == tCommand){
            if(QFile::exists(cashe_folder + "/" + fl.at(pos).fileName())&& cashing==true){

            }else if(stego.contains("%2")){
                QString command = QString(stego).arg(fl.at(pos).absoluteFilePath()).arg(cashe_folder + "/" + fl.at(pos).fileName());
                if(QProcess::execute(command)!=0){
                    signal->logMessage(CRITICAL, QString::fromLocal8Bit("Ошибка при выполнении команды: %1").arg(command));
                    return false;
                }

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

void vIBuffer::loadConfigurationFromWidget(QTreeWidget *infoWidget){

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

void vIBuffer::loadBuffer(){
    seek = 0;
    if(!fstego.isOpen()){
        setFile();
    }
    QByteArray orig_buff = forig.read(buffer_size);
    stego_buff = fstego.read(buffer_size);

    if(stego_buff.size()<buffer_size)
        fstego.close();


    for(int i=0; i<buffer_size; i++)
        diff[i]=orig_buff[i]-stego_buff[i];

}

QByteArray vIBuffer::getDiff(int size){
    if(seek+size > buffer_size)
        loadBuffer();
    seek += size;
    return diff.mid(seek-size, size);
}

QByteArray vIBuffer::getBuffer(int size){
    return stego_buff.mid(seek, size);
}
