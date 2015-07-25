#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsProxyWidget>
#include <QSettings>
#include <QFileDialog>
#include <QDebug>

#include "vgraphicsproxywidget.h"
#include "vlineeditdelegate.h"
#include "edge.h"
#include "vabufferform.h"
#include "vanalyserform.h"
#include "vedgedialog.h"


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

    connect(&scene, SIGNAL(sceneRectChanged( const QRectF & )), this, SLOT(bufferResized( const QRectF & )));

    brain = new Brain(ui->graphicsView);
    scene.addItem(brain);
    brain->setPos(0, 50);

}

void MainWindow::bufferResized ( const QRectF & rect ){
//    buffer->setFixedHeight(rect.height());
//    b->setPos( -rect.width() + buffer->width(), -rect.height()/2);

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
    Analyse *node = new Analyse(ui->graphicsView);
    scene.addItem(node);
    node->setPos(0, 0);
    nodeList.append(node);



}

void MainWindow::on_treeWidget_doubleClicked(const QModelIndex &index)
{
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
        settings.setValue("stego", item->child(0)->text(1));
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

    //Сохраняем конфигурацию буферов
    settings.beginGroup("nodes");
    for(QList<Analyse*>::iterator it = nodeList.begin(); it!=nodeList.end(); ++it){
        settings.beginGroup((*it)->getName());
        settings.setValue("x", (*it)->pos().x());
        settings.setValue("y", (*it)->pos().y());
        QList <Edge*> edges = (*it)->edges();
        for(QList<Edge*>::iterator et = edges.begin(); et!=edges.end(); ++et){
            Analyse* an = dynamic_cast <Analyse*> ((*et)->destNode());
            if(an!=NULL)
                settings.setValue("edge", an->getName());
            Brain* br = dynamic_cast <Brain*> ((*et)->destNode());
            if(br != NULL)
                settings.setValue("edge", "brain");
        }
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
    for(QList<Buffer*>::iterator it = bufferList.begin(); it!= bufferList.end(); ++it){
        (*it)->setDiff(ibuffer.getDiff((*it)->getBufferSize()));
        (*it)->update();
    }
//    buffer->loadBuffer();
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
