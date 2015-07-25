#ifndef VANALYSERFORM_H
#define VANALYSERFORM_H

#include <QDialog>

namespace Ui {
class vAnalyserForm;
}

class vAnalyserForm : public QDialog
{
    Q_OBJECT

public:
    explicit vAnalyserForm(QWidget *parent = 0);
    ~vAnalyserForm();

private:
    Ui::vAnalyserForm *ui;
};

#endif // VANALYSERFORM_H
