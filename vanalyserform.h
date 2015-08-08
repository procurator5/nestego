#ifndef VANALYSERFORM_H
#define VANALYSERFORM_H

#include <QDialog>
#include <QGraphicsView>
#include <QList>

#include "analyse.h"
#include "edge.h"

namespace Ui {
class vAnalyserForm;
}

class vAnalyserForm : public QDialog
{
    Q_OBJECT

public:
    explicit vAnalyserForm(QWidget *parent = 0);
    ~vAnalyserForm();
    void setAnalyseName(QString name);
    bool isAcceptButtonClicked(){
        return AcceptButtonClicked;
    };

    /**
     * @brief ВОзвращает объект Analyse, который потом помещается в рабочую область
     * @return указатель на объект Analyse или NULL если объект не может быть создан
     */
    Analyse* getAnalyse(QGraphicsView* view);

    void setLibFannParametrs(int layers, int hidden_neurons,
                             fann_activationfunc_enum func);
    void setDataSource(QList<Edge *> edgeList);

private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_verticalSlider_sliderMoved(int position);

    void on_horizontalSlider_sliderMoved(int position);

    void on_spinBox_2_valueChanged(int arg1);

    void on_spinBox_3_valueChanged(int arg1);

    void on_buttonBox_accepted();

    void on_comboBox_5_activated(int index);

    void on_comboBox_5_currentIndexChanged(int index);

private:
    Ui::vAnalyserForm *ui;
    bool AcceptButtonClicked;
    unsigned input_size;
};

#endif // VANALYSERFORM_H
