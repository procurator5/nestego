#ifndef VBUFFERVIEWFORM_H
#define VBUFFERVIEWFORM_H

#include <QDialog>
#include <QTableWidget>

namespace Ui {
class vBufferViewForm;
}

class vBufferViewForm : public QDialog
{
    Q_OBJECT

public:
    explicit vBufferViewForm(QWidget *parent = 0);
    ~vBufferViewForm();
    QTableWidget* getTableWidget();

public slots:
    void setWindowTitle(const QString& title);

private:
    Ui::vBufferViewForm *ui;
};

#endif // VBUFFERVIEWFORM_H
