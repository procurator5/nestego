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
#include "feedbackbuffer.h"

#include "csignal.h"
#include "vproject.h"

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    /**
     * @brief Запись информации о выполнении в лог (расположен в виджете ui->textBrowser)
     * @param level Уровень логгирования, расположен в перечислении Level
     * @param message Текст сообщения
     */
    void logInfo(int level, QString message);

private slots:
    void on_action_addAnalyze_triggered();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void slot_projectItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous);

    void on_action_saveProject_triggered();

    void on_action_OpenProject_triggered();

    /**
     * @brief Запуск процесса обучения
     */
    void on_pushButton_clicked();

    void on_action_addBuffer_triggered();

    void on_action_addEdge_triggered();

    /**
     * @brief Показывает график обучения нейронной сети
     * @param Analyse* идентификатор нейронной сети
     */
    void showTrainPlot(Analyse*);

    /**
     * @brief Показывает содержимое начальных буферов
     */
    void showBufferData(Buffer*buf);


    void on_checkBox_clicked(bool checked);

    void on_checkBox_2_clicked(bool checked);

    void on_checkBox_5_clicked(bool checked);

    void on_checkBox_6_clicked(bool checked);

    void on_pushButton_2_clicked();

    void on_action_feedBackBuffer_triggered();


    /**
     * @brief Слот пошагового анализа файла (режим отладки)
     */

    void on_action_Debug_triggered();

    /**
     * @brief Загружаются базовые данные в виджет из БД проекта
     */
    void loadSourceSection();

    /**
     * @brief Сохраняются базовые данные из виджета в БД
     */
    void saveSourceSection();

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
    void clearModeSelectin();
    QGraphicsProxyWidget *b;
    /**
     * @brief Список анализаторов, участвующих в работе стегодетектора
     */
    QList <Analyse*> nodeList;
    QList <Buffer*> bufferList;
    vIBuffer ibuffer;

    /**
     * @brief Элемент, с которого снимается вход нейронной сети
     */
    Brain* brain;

    QwtPlotCurve* need_result;
    QwtPlotCurve* output_before_train;
    QwtPlotCurve* error1;
    QwtPlotCurve* error2;

    Analyse *getAnalyseByName(QString name);

};

#endif // MAINWINDOW_H
