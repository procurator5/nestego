#ifndef VTRAINTHREAD_H
#define VTRAINTHREAD_H

#include <QThread>

class vTrainThread : public QThread
{
    Q_OBJECT
public:
    explicit vTrainThread(QObject *parent = 0);

signals:

public slots:

};

#endif // VTRAINTHREAD_H
