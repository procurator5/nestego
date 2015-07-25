#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QModelIndex>
#include <QTreeWidgetItem>
#include <QList>

#include "node.h"
#include "buffer.h"
#include "vibuffer.h"
#include "brain.h"
#include "analyse.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_action_addAnalyze_triggered();

    void on_treeWidget_doubleClicked(const QModelIndex &index);

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_action_saveProject_triggered();

    void on_action_OpenProject_triggered();
    void bufferResized ( const QRectF & rect );

    /**
     * @brief Запуск процесса обучения
     */
    void on_pushButton_clicked();

    void on_action_addBuffer_triggered();

    void on_action_addEdge_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
    void clearModeSelectin();
    QGraphicsProxyWidget *b;
    QList <Analyse*> nodeList;
    QList <Buffer*> bufferList;
    vIBuffer ibuffer;

    /**
     * @brief Элемент, с которого снимается вход нейронной сети
     */
    Brain* brain;



};

#endif // MAINWINDOW_H
