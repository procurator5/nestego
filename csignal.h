#ifndef CSIGNAL_H
#define CSIGNAL_H

#include <QObject>
#include "analyse.h"

enum Level{
    DEBUG,
    INFO,
    WARN,
    ERROR,
    CRITICAL
};

class CSignal : public QObject
{
    Q_OBJECT
public:
    explicit CSignal(QObject *parent = 0);
    void logMessage(Level level, QString message);
    void analyseMouseClicked(Analyse* analyse);

signals:
    void signalLogMessage(int level, QString message);
    void signalAnalyseMouseClicked(Analyse* analyse);

public slots:

};

#endif // CSIGNAL_H
