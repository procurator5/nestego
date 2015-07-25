#ifndef VEDGEDIALOG_H
#define VEDGEDIALOG_H

#include <QDialog>
#include <QMap>

#include <node.h>
#include "edge.h"

namespace Ui {
class vEdgeDialog;
}

class vEdgeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit vEdgeDialog(QWidget *parent = 0);
    ~vEdgeDialog();

    void loadNodesList(QMap<QString, Node*> map);
    Edge* getEdge();

private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::vEdgeDialog *ui;
    QMap<QString, Node *> elements;
};

#endif // VEDGEDIALOG_H
