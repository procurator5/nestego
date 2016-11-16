#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsProxyWidget>
#include <QSettings>
#include <QFileDialog>
#include <QDebug>

#include "vlineeditdelegate.h"
#include "edge.h"
#include "vabufferform.h"
#include "vanalyserform.h"
#include "vedgedialog.h"

extern CSignal *signal;
extern vProject *currentProject;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->treeWidget->expandAll();
    ui->treeWidget->setColumnWidth(0, 200);
    ui->treeWidget->setItemDelegate(new vLineEditDelegate(ui->treeWidget));
    ui->treeWidget->setExpandsOnDoubleClick(true);
    ui->treeWidget->hideColumn(2);

    ui->graphicsView->setScene(&scene);

    connect(signal, SIGNAL(signalLogMessage(int, QString)), this, SLOT(logInfo(int, QString)));
    connect(signal, SIGNAL(signalAnalyseMouseClicked(Analyse*)), this, SLOT(showTrainPlot(Analyse*)));
    connect(signal, SIGNAL(signalBufferMouseClicked(Buffer*)), this, SLOT(showBufferData(Buffer*)));

    brain = new Brain(ui->graphicsView);
    scene.addItem(brain);
    brain->setPos(0, 50);

    //Готовим элементы для рисования графиков

    need_result = new QwtPlotCurve("Need Result");
    QPen pen_need_result(Qt::green);
    need_result->setPen(pen_need_result);


    output_before_train = new QwtPlotCurve("Out before train");
    QPen pen_higest(Qt::red);
    output_before_train->setPen(pen_higest);

    error1 = new QwtPlotCurve("Out before train");
    QPen pen_error1(Qt::blue);
    error1->setPen(pen_error1);

    error2 = new QwtPlotCurve("Out before train");
    QPen pen_error2(Qt::yellow);
    error2->setPen(pen_error2);

    //Тестим загрузку
    loadSourceSection();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_addAnalyze_triggered()
{
    vAnalyserForm af;
    af.exec();
 //   scene.addItem(new vGraphicsProxyWidget);

    Analyse *node = af.getAnalyse(ui->graphicsView);
    if(node!=NULL){
        scene.addItem(node);
        node->setPos(0, 0);
        nodeList.append(node);

    }
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(item->parent()!=NULL)
        return;

    if(item!=ui->treeWidget->topLevelItem(0) && item!=ui->treeWidget->topLevelItem(1))
        return;
    clearModeSelectin();
    QFont font = item->font(0);
    font.setBold(true);
    item->setFont(0,font);
    item->setText(2, "X");


}

void MainWindow::on_action_saveProject_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               QCoreApplication::applicationDirPath(),
                               tr("Nestego project (*.nes )"));
    if(fileName.isEmpty())
        return;

    QFileInfo fi(fileName);
    QString path = fi.absolutePath();

    QSettings settings(fileName, QSettings::IniFormat);
    settings.clear();
    settings.beginGroup("source");
    //Отпределяем тип контрольного примера
    if(ui->treeWidget->topLevelItem(0)->text(2)=="X"){
        settings.setValue("type", "program");
        QTreeWidgetItem * item = ui->treeWidget->topLevelItem(0);
        settings.setValue("no_stego", item->child(0)->text(1));
        settings.setValue("command", item->child(1)->text(1));
        settings.setValue("cache", item->child(2)->text(1));
        settings.setValue("cashe_folder", item->child(3)->text(1));

    }else{
        settings.setValue("type", "example");
        QTreeWidgetItem * item = ui->treeWidget->topLevelItem(1);
        settings.setValue("no_stego", item->child(0)->text(1));
        settings.setValue("stego", item->child(1)->text(1));
    }

    settings.endGroup();

    settings.beginGroup("common");
    QTreeWidgetItem * item = ui->treeWidget->topLevelItem(2);
    settings.setValue("buffer_size", item->child(0)->text(1));
    settings.setValue("step", item->child(1)->text(1));
    settings.setValue("stego_percent", item->child(2)->text(1));
    settings.endGroup();

    //Сохраняем конфигурацию буферов
    settings.beginGroup("buffers");
    int i=0;
    for(QList<Buffer*>::iterator it = bufferList.begin(); it!=bufferList.end(); ++it){
        settings.beginGroup(QString("buffer%1").arg(i++));
        settings.setValue("size", (*it)->getBufferSize());
        settings.setValue("x", (*it)->pos().x());
        settings.setValue("y", (*it)->pos().y());
        //Сохраняем выходные связи
        QList <Edge*> edges = (*it)->edges();
        for(QList<Edge*>::iterator et = edges.begin(); et!=edges.end(); ++et){
            Analyse* an = dynamic_cast <Analyse*> ((*et)->destNode());
            if(an!=NULL)
                settings.setValue("edge", an->getName());
        }
        settings.endGroup();
    }
    settings.endGroup();

    //Сохраняем конфигурацию анализаторов
    settings.beginGroup("nodes");
    for(QList<Analyse*>::iterator it = nodeList.begin(); it!=nodeList.end(); ++it){
        settings.beginGroup((*it)->getName());
        settings.setValue("x", (*it)->pos().x());
        settings.setValue("y", (*it)->pos().y());

        QList <train_result> train = (*it)->getTrainResult();
        QList <QVariant> teach;
    //Преобразуем выборку в сохраняемый формат QSettings
        for(QList <train_result>::iterator it = train.begin(); it!= train.end(); ++it){
            QList <QVariant> oneStep;
            oneStep<<QVariant((*it).output_before_train);
            oneStep<<QVariant((*it).output_after_train);
            oneStep<<QVariant((*it).need_result);
            oneStep<<QVariant((*it).error1);
            oneStep<<QVariant((*it).error2);
            teach.append(oneStep);
        }

        settings.setValue("teach", QVariant(teach));

        QList <Edge*> edges = (*it)->edges();
        for(QList<Edge*>::iterator et = edges.begin(); et!=edges.end(); ++et){
            //Сохраняем связи между анализаторами
            Analyse* an = dynamic_cast <Analyse*> ((*et)->destNode());
            if(an!=NULL){
                settings.setValue("edge", an->getName());
            }
            Brain* br = dynamic_cast <Brain*> ((*et)->destNode());
            if(br != NULL)
                settings.setValue("edge", "brain");

        }
        //Сохраняем веса анализаторов
        (*it)->saveAnalyse(path + "/" + (*it)->getName()+".fan");
        settings.endGroup();

    }
    settings.endGroup();

    settings.beginGroup("brain");
    settings.setValue("x", brain->pos().x());
    settings.setValue("y", brain->pos().y());
    settings.endGroup();

}

void MainWindow::on_action_OpenProject_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
                            this,
                            "Select one file to open",
                            QCoreApplication::applicationDirPath(),
                            tr("Nestego project (*.nes )"));
    if(fileName.isEmpty())
        return;

    QFileInfo fi(fileName);
    QString path = fi.absolutePath();

    clearModeSelectin();
    QSettings settings(fileName, QSettings::IniFormat);
    settings.beginGroup("source");
    //Определяем тип контрольного примера
    if(settings.value("type", QVariant("undef"))=="program"){
        QTreeWidgetItem * item = ui->treeWidget->topLevelItem(0);
        item->setText(3, "X");
        QFont font = item->font(0);
        font.setBold(true);
        item->setFont(0,font);
        item->setText(2, "X");
        item->child(0)->setText(1, settings.value("no_stego", QVariant("undef")).toString());
        item->child(1)->setText(1, settings.value("command", QVariant("undef")).toString());
        item->child(2)->setText(1, settings.value("cache", QVariant("true")).toString());
        item->child(3)->setText(1, settings.value("cashe_folder", QVariant("true")).toString());


    }else{
        QTreeWidgetItem * item = ui->treeWidget->topLevelItem(1);
        item->setText(3, "X");
        QFont font = item->font(0);
        font.setBold(true);
        item->setFont(0,font);
        item->setText(2, "X");
        item->child(0)->setText(1, settings.value("no_stego", QVariant("undef")).toString());
        item->child(1)->setText(1, settings.value("stego", QVariant("undef")).toString());
    }
    settings.endGroup();
    settings.beginGroup("common");
    QTreeWidgetItem * item = ui->treeWidget->topLevelItem(2);
    item->child(0)->setText(1, settings.value("buffer_size", QVariant("undef")).toString());
    item->child(1)->setText(1, settings.value("step", QVariant("undef")).toString());
    item->child(2)->setText(1, settings.value("stego_percent", QVariant("true")).toString());
    settings.endGroup();

    bufferList.clear();

    scene.clear();

    brain = new Brain(ui->graphicsView);
    scene.addItem(brain);

    settings.beginGroup("brain");
    brain->setPos(settings.value("x").toReal(), settings.value("y").toReal());
    settings.endGroup();


    //Загружаем изображение буферов
    settings.beginGroup("buffers");
    QStringList chgr = settings.childGroups();
    for(QStringList::iterator it = chgr.begin(); it!=chgr.end();++it){
        settings.beginGroup(*it);
        Buffer* buf = new Buffer(ui->graphicsView);
        buf->setBufferSize(settings.value("size").toInt());
        scene.addItem(buf);
        buf->setPos(settings.value("x").toReal(), settings.value("y").toReal() );
        //
        settings.endGroup();
        bufferList.append(buf);
    }
    settings.endGroup();


    //Загружаем изображение анализаторов
    settings.beginGroup("nodes");
    chgr = settings.childGroups();
    for(QStringList::iterator it = chgr.begin(); it!=chgr.end();++it){
        settings.beginGroup(*it);
        Analyse* an = new Analyse(ui->graphicsView);
        an->setName(*it);
        scene.addItem(an);
        an->setPos(settings.value("x").toReal(), settings.value("y").toReal() );
        settings.endGroup();
        nodeList.append(an);
    }
    settings.endGroup();

    //Загружаем связи между элементами
    //Для буфферов
    settings.beginGroup("buffers");
    chgr = settings.childGroups();
    int i = 0;
    for(QStringList::iterator it = chgr.begin(); it!=chgr.end();++it){
        settings.beginGroup(*it);
        QString name = settings.value("edge").toString();
        for(QList <Analyse*>::iterator bt = nodeList.begin(); bt!=nodeList.end(); ++bt)
            if((*bt)->getName()==name){
                scene.addItem(new Edge(bufferList.at(i), *bt));
            }
        settings.endGroup();
        i++;
    }
    settings.endGroup();

    //Для анализаторов

    settings.beginGroup("nodes");
    chgr = settings.childGroups();
    for(QStringList::iterator it = chgr.begin(); it!=chgr.end();++it){
        settings.beginGroup(*it);
        QString name = settings.value("edge").toString();
        if(name == "brain"){
            //Устанавливаем выход на итоговый анализатор
            scene.addItem(new Edge(getAnalyseByName(*it), brain));
        }else
            scene.addItem(new Edge(getAnalyseByName(*it), getAnalyseByName(name)));

        //Читаем параметры НС
        if (!getAnalyseByName(*it)->loadAnalyseFromFile(path+"/"+(*it)+".fan")){
            signal->logMessage((Level)3, "!!Error load settings analyse "+*it+" from file "+path+"/"+*it+".fan");
            getAnalyseByName(*it)->createNeuralNetwork();
        }
        //Добавляем результаты обучения
        QList <QVariant> teach = settings.value("teach").toList();
        for(int i =0; i<teach.size(); i+=5){
            QVector <int> oneStep(5);
            for(int j=0; j<5; j++){
                float value = teach.at(i+j).toFloat()*255;
                oneStep[j] = value;
            }

            getAnalyseByName(*it)->setTrainResult(oneStep.at(0), oneStep.at(1), oneStep.at(2));
            oneStep.clear();
        }
        settings.endGroup();
    }
    settings.endGroup();
}


void MainWindow::clearModeSelectin(){
    for(QTreeWidgetItem* next = ui->treeWidget->topLevelItem(0);next!=NULL; next = ui->treeWidget->itemBelow(next)){
        if(next->parent()!=NULL)
            continue;
        QFont font = next->font(0);
        font.setBold(false);
        next->setFont(0,font);
        next->setText(2, "");
    }
}

void MainWindow::on_pushButton_clicked()
{
    ibuffer.loadConfigurationFromWidget(ui->treeWidget);
    ibuffer.loadBuffer();
/*
//Инициализируюся нейронные сети-анализаторы
    for(QList<Analyse*>::iterator it = nodeList.begin(); it!= nodeList.end(); ++it){
        (*it)->createNeuralNetwork();
    }

    for(QList<Buffer*>::iterator it = bufferList.begin(); it!= bufferList.end(); ++it){
        int  size = (*it)->getBufferSize();
        (*it)->setDiff(ibuffer.getDiff(size));
        (*it)->update();
    //Определяем связи буфера и отправляем полученные данные на "Обучение"
        QList<Edge*> edges = (*it)->edges();
        for(QList<Edge*>::iterator et = edges.begin(); et != edges.end(); ++et){
            Analyse *an = dynamic_cast<Analyse*> ((*et)->destNode());
            if(an != NULL)
                an->trainFann(&ibuffer);

        }
    }
//    buffer->loadBuffer();
*/
    //Делаем N одиночных шагов
    for(int i =0; i<ui->spinBox->value();i++)
        this->on_action_Debug_triggered();
}

void MainWindow::on_action_addBuffer_triggered()
{
    vABufferForm bf(this);
    bf.exec();

    Buffer *buffer = bf.returnElement(ui->graphicsView);
    bufferList.append(buffer);
}

#include <QMap>
void MainWindow::on_action_addEdge_triggered()
{
    vEdgeDialog ed;

    QMap <QString, Node*> nodes;
    int i=0;
    for(QList <Buffer*>::iterator it = bufferList.begin(); it!=bufferList.end();++it){
        nodes.insert(QString("Buffer%1 (%2)").arg(i++).arg((*it)->getBufferSize()), *it);
    }

    for(QList <Analyse*>::iterator it = nodeList.begin(); it!=nodeList.end();++it){
        nodes.insert((*it)->getName(), *it);
    }

    nodes.insert(QString::fromLocal8Bit("Выход анализатора") , brain);

    ed.loadNodesList(nodes);
    ed.exec();

    Edge *edge = ed.getEdge();
    scene.addItem(edge);

}

void MainWindow::logInfo(int lev, QString message){
    ui->textBrowser->append(message);

}

void MainWindow::showTrainPlot(Analyse *an){

//    ui->qwtPlot->clear();
    ui->toolBox->setCurrentIndex(2);
    //Рисуем график

    int i = 0;
    QList<train_result> list = an->getTrainResult();
    if(list.empty())
        return;


    QVector <double> output_before_train_vector(list.size());
    QVector <double> need_result_vector(list.size());
    QVector <double> error1_vector(list.size());
    QVector <double> error2_vector(list.size());
    QVector <double> x(list.size());

    double error1_sum=0;
    double error2_sum=0;

    for(QList<train_result>::iterator it = list.begin(); it!= list.end();++it){
        output_before_train_vector[i] = (*it).output_before_train;
        need_result_vector[i] = (*it).need_result;
        error1_sum += (*it).error1;
        error1_vector[i] = error1_sum/(i+1);

        error2_sum += (*it).error2;
        error2_vector[i] = error2_sum/(i+1);

        x[i] = i;
        i++;
    }
    /*
    output_before_train->setData(x, output_before_train_vector);
    need_result->setData(x, need_result_vector);
    error1->setData(x, error1_vector);
    error2->setData(x, error2_vector);
*/
    output_before_train->setSamples(x, output_before_train_vector);
    need_result->setSamples(x, need_result_vector);
    error1->setSamples(x, error1_vector);
    error2->setSamples(x, error2_vector);

    output_before_train->attach(ui->qwtPlot);
    need_result->attach(ui->qwtPlot);
    error1->attach(ui->qwtPlot);
    error2->attach(ui->qwtPlot);
    ui->qwtPlot->replot();
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    if(checked)
        need_result->setVisible(true);
    else
        need_result->setVisible(false);

    ui->qwtPlot->replot();
}

void MainWindow::on_checkBox_2_clicked(bool checked)
{
    if(checked)
        output_before_train->setVisible(true);
    else
        output_before_train->setVisible(false);

    ui->qwtPlot->replot();

}

void MainWindow::on_checkBox_5_clicked(bool checked)
{
    if(checked)
        error1->setVisible(true);
    else
        error1->setVisible(false);

    ui->qwtPlot->replot();
}

void MainWindow::on_checkBox_6_clicked(bool checked)
{
    if(checked)
        error2->setVisible(true);
    else
        error2->setVisible(false);

    ui->qwtPlot->replot();
}

void MainWindow::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               QCoreApplication::applicationDirPath(),
                               tr("Nestego project (*.csv )"));

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
        return;
    QTextStream stream( &file );
    QStringList text = ui->textBrowser->toPlainText().split("\n");
    int i=0;
    for(QStringList::iterator it = text.begin(); it!= text.end(); ++it){
        QString line  = *it;
        if(line.indexOf("Step")!= -1){
            stream<<"\r\n"<<++i<<",";
        }else{
            stream<<line.mid(line.indexOf(QRegExp("[0123456789]"))).toDouble()<<",";
        }

    }

}

void MainWindow::on_action_feedBackBuffer_triggered()
{
    vABufferForm bf(this);
    bf.exec();

    bf.returnFeedBackBuffer(ui->graphicsView);
}

void MainWindow::on_action_Debug_triggered()
{

    QTreeWidgetItem * item = ui->treeWidget->topLevelItem(2);
    int step = item->child(1)->text(1).toInt();

    //Инициализируем данные для обработки
    ibuffer.loadConfigurationFromWidget(ui->treeWidget);
    ibuffer.loadBuffer();

    //1. Загружаем данные в первичные буферы
    for(QList<Buffer*>::iterator it = bufferList.begin(); it!= bufferList.end(); ++it){
        int  size = (*it)->getBufferSize();
        (*it)->loadData(ibuffer.getStegoBuffer(size), ibuffer.getOriginBuffer(size));
        (*it)->update();
    }
    //2. Определяем, какие анализаторы работают напрямую с буферами обмена
    //Определяем связи буфера и отправляем полученные данные на "Обучение"

    for(QList<Buffer*>::iterator it = bufferList.begin(); it!= bufferList.end(); ++it){
        QList<Edge*> edges = (*it)->edges();
        QByteArray buf = (*it)->buffer();
        //Вычисляем ожидаемый выход НС
        int need_result = (*it)->getDiff();
        for(QList<Edge*>::iterator et = edges.begin(); et != edges.end(); ++et){
            Analyse *an = dynamic_cast<Analyse*> ((*et)->destNode());
            if(an != NULL){
                //Вычисляем выходы соответствующих анализаторов
                int before = an->getResult(buf);

                //Обучаем НС
                an->train(buf, need_result);

                //ПОлучаем результат НС после обучения
                int after = an->getResult(buf);
                an->setTrainResult(before, after, need_result);

            }
        }
    }

    ibuffer.moveBuffer(step);
}

Analyse* MainWindow::getAnalyseByName(QString name){
    for(QList <Analyse*>::iterator it = nodeList.begin(); it != nodeList.end(); ++it)
        if((*it)->getName() == name)
            return *it;
    return NULL;
}

#include "vbufferviewform.h"

void MainWindow::showBufferData(Buffer *buf){
    bool showForm = buf->haveForm();
    vBufferViewForm *bvf = buf->getBufferForm();
    if(!showForm)
        scene.addWidget(bvf, Qt::Window);

}

void MainWindow::loadSourceSection(){
    //Определяем тип контрольного примера
    if(currentProject->getSorceParam("type")=="program"){
        QTreeWidgetItem * item = ui->treeWidget->topLevelItem(0);
        item->setText(3, "X");
        QFont font = item->font(0);
        font.setBold(true);
        item->setFont(0,font);
        item->setText(2, "X");
        item->child(0)->setText(1, currentProject->getSorceParam("no_stego"));
        item->child(1)->setText(1, currentProject->getSorceParam("command"));
        item->child(2)->setText(1, currentProject->getSorceParam("cashe"));
        item->child(3)->setText(1, currentProject->getSorceParam("cashe_folder"));


    }else{
        QTreeWidgetItem * item = ui->treeWidget->topLevelItem(1);
        item->setText(3, "X");
        QFont font = item->font(0);
        font.setBold(true);
        item->setFont(0,font);
        item->setText(2, "X");
        item->child(0)->setText(1, currentProject->getSorceParam("no_stego"));
        item->child(1)->setText(1, currentProject->getSorceParam("stego"));
    }

    //Загружаем ноды из файла
    brain->loadFromProject();

    //Загружаем буферы
    QList <QString> listNodes = currentProject->getBuffers();

    for(QList <QString>::iterator it = listNodes.begin(); it!=listNodes.end(); ++it){
        qDebug()<<*it;
        Buffer* buf = new Buffer(ui->graphicsView);
        buf->setBufferSize(currentProject->getBufferSize(*it));
        scene.addItem(buf);
       // buf->setPos(settings.value("x").toReal(), settings.value("y").toReal() );
        //

    }

}

void MainWindow::saveSourceSection(){
    if(ui->treeWidget->topLevelItem(0)->text(2)=="X"){
        currentProject->setSourceParam("type", "program");
        QTreeWidgetItem * item = ui->treeWidget->topLevelItem(0);
        currentProject->setSourceParam("no_stego", item->child(0)->text(1));
        currentProject->setSourceParam("command", item->child(1)->text(1));
        currentProject->setSourceParam("cache", item->child(2)->text(1));
        currentProject->setSourceParam("cashe_folder", item->child(3)->text(1));

    }else{
        currentProject->setSourceParam("type", "example");
        QTreeWidgetItem * item = ui->treeWidget->topLevelItem(1);
        currentProject->setSourceParam("no_stego", item->child(0)->text(1));
        currentProject->setSourceParam("stego", item->child(0)->text(1));
    }
}
