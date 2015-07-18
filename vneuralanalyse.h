#ifndef VNEURALANALYSE_H
#define VNEURALANALYSE_H

#include <QWidget>

namespace Ui {
class vNeuralAnalyse;
}

class vNeuralAnalyse : public QWidget
{
    Q_OBJECT

public:
    explicit vNeuralAnalyse(QWidget *parent = 0);
    ~vNeuralAnalyse();

private:
    Ui::vNeuralAnalyse *ui;
};

#endif // VNEURALANALYSE_H
