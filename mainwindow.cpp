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

    Node *node1 = new Node(ui->graphicsView);
    Node *node2 = new Node(ui->graphicsView);
    Node *node3 = new Node(ui->graphicsView);
    Node *node4 = new Node(ui->graphicsView);
    Node *centerNode = new Node(ui->graphicsView);
    Node *node6 = new Node(ui->graphicsView);
    Node *node7 = new Node(ui->graphicsView);
    Node *node8 = new Node(ui->graphicsView);
    Node *node9 = new Node(ui->graphicsView);
    scene.addItem(node1);
    scene.addItem(node2);
    scene.addItem(node3);
    scene.addItem(node4);
    scene.addItem(centerNode);
    scene.addItem(node6);
    scene.addItem(node7);
    scene.addItem(node8);
    scene.addItem(node9);
    scene.addItem(new Edge(node1, node2));
    scene.addItem(new Edge(node2, node3));
    scene.addItem(new Edge(node2, centerNode));
    scene.addItem(new Edge(node3, node6));
    scene.addItem(new Edge(node4, node1));
    scene.addItem(new Edge(node4, centerNode));
    scene.addItem(new Edge(centerNode, node6));
    scene.addItem(new Edge(centerNode, node8));
    scene.addItem(new Edge(node6, node9));
    scene.addItem(new Edge(node7, node4));
    scene.addItem(new Edge(node8, node7));
    scene.addItem(new Edge(node9, node8));

    node1->setPos(-50, -50);
    node2->setPos(0, -50);
    node3->setPos(50, -50);
    node4->setPos(-50, 0);
    centerNode->setPos(0, 0);
    node6->setPos(50, 0);
    node7->setPos(-50, 50);
    node8->setPos(0, 50);
    node9->setPos(50, 50);


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
 //   scene.addItem(new vGraphicsProxyWidget);
    Node *node = new Node(ui->graphicsView);
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
        settings.endGroup();
    }
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
    //Отпределяем тип контрольного примера
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

    //Загружаем изображение буферов
    settings.beginGroup("buffers");
    QStringList chgr = settings.childGroups();
    int i = 0;
    for(QStringList::iterator it = chgr.begin(); it!=chgr.end();++it){
        settings.beginGroup(*it);
        Buffer* buf = new Buffer(ui->graphicsView);
        buf->setBufferSize(settings.value("size").toInt());
        scene.addItem(buf);
        buf->setPos(-50 + 40 * ++i, -50 );
        settings.endGroup();
        bufferList.append(buf);
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

