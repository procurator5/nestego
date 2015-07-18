#ifndef VCHOOSEDIRECTORY_H
#define VCHOOSEDIRECTORY_H

#include <QWidget>

namespace Ui {
class vChooseDirectory;
}

class vChooseDirectory : public QWidget
{
    Q_OBJECT

public:
    explicit vChooseDirectory(QWidget *parent = 0);
    ~vChooseDirectory();
    QString getText();
    void setText(QString);

private slots:
    void on_toolButton_clicked();

private:
    Ui::vChooseDirectory *ui;
};

#endif // VCHOOSEDIRECTORY_H
