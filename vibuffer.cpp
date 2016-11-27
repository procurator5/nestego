#include "vibuffer.h"
#include <QDir>
#include <QTime>
#include <QProcess>

#include "csignal.h"
extern CSignal* signal;

vIBuffer::vIBuffer(QObject *parent) :
    QObject(parent)
{
    seek =0x8FFFFFFFFFFFFF;
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
            if(!fstego.open(QIODevice::ReadOnly))
                signal->logMessage(CRITICAL, QString::fromLocal8Bit("Недоступен стего-контейнер. Имя контейнера: %1. Err: %2").arg(stego + "/" +fl.at(pos).fileName()).arg(fstego.errorString()));
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

#include <QDebug>

void vIBuffer::loadBuffer(){
    if(!fstego.isOpen()){
        setFile();
        seek = 0;
    }

/*    if(seek<buffer_size)
        return;
*/
    origin = forig.read(buffer_size);
    stego_buff = fstego.read(buffer_size);

    if(stego_buff.size()<buffer_size)
        fstego.close();


    for(int i=0; i<buffer_size; i++)
        diff[i]=origin[i]-stego_buff[i];
}

QByteArray vIBuffer::getDiff(int size){
    if(seek+size > buffer_size)
        loadBuffer();
    seek += size;
    return diff.mid(seek-size, size);
}

QByteArray vIBuffer::getBuffer(int size){

    isBuf(size);
    return stego_buff.mid(seek, size);
}

QByteArray vIBuffer::getStegoBuffer(int size){
    isBuf(size);
    return stego_buff.mid(seek, size);
}

QByteArray vIBuffer::getOriginBuffer(int size){
    if(origin.size()<seek+size)
        return QByteArray();
    return origin.mid(seek, size);
}

void vIBuffer::moveBuffer(int size){
    seek+=size;
}

void vIBuffer::isBuf(int size){
    if(stego_buff.size()<seek+size){
        seek+=size;
        if(!forig.seek(forig.pos()-size))
            signal->logMessage(ERROR, "QFile(orig)::seek: IODevice is not open");
        if (!fstego.seek(fstego.pos()-size))
            signal->logMessage(ERROR, "QFile(stego)::seek: IODevice is not open");            ;
        loadBuffer();
    }
}
