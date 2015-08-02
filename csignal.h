#ifndef CSIGNAL_H
#define CSIGNAL_H

#include <QObject>

class CSignal : public QObject
{
    Q_OBJECT
public:
    explicit CSignal(QObject *parent = 0);

signals:

public slots:

};

#endif // CSIGNAL_H
