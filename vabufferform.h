#ifndef VABUFFERFORM_H
#define VABUFFERFORM_H

#include <QDialog>
#include <QGraphicsView>

#include "buffer.h"
#include "feedbackbuffer.h"

namespace Ui {
class vABufferForm;
}

class vABufferForm : public QDialog
{
    Q_OBJECT

public:
    explicit vABufferForm(QWidget *parent = 0);
    ~vABufferForm();
    Buffer* returnElement(QGraphicsView * view);

    FeedBackBuffer* returnFeedBackBuffer(QGraphicsView * view);

private:
    Ui::vABufferForm *ui;
};

#endif // VABUFFERFORM_H
